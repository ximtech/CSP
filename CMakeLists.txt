cmake_minimum_required(VERSION 3.16)
project(CSP VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)

include(cmake/CPM.cmake)

CPMAddPackage(
        NAME FileUtils
        GITHUB_REPOSITORY ximtech/FileUtils
        GIT_TAG origin/main)

set(SOURCE_FILES
        include/CSPTemplate.h
        include/CSPTokener.h
        include/CSPReport.h
        include/CSPInterpreter.h
        include/CSPRenderer.h
        include/CSPTableString.h
        include/CSPDataUtils.h
        include/CSPCompiler.h
        include/CSPValue.h

        CSPTemplate.c
        CSPTokener.c
        CSPReport.c
        CSPInterpreter.c
        CSPRenderer.c
        CSPTableString.c
        CSPCompiler.c
        CSPValue.c)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

target_include_directories(${PROJECT_NAME} PUBLIC
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)


find_library(MATH_LIBRARY m)
if(MATH_LIBRARY)
    target_link_libraries(${PROJECT_NAME} PUBLIC ${MATH_LIBRARY} FileUtils)
else ()
    target_link_libraries(${PROJECT_NAME} PUBLIC FileUtils)
endif()

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}.h
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/${PROJECT_NAME})

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Export
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/${PROJECT_NAME})

install(EXPORT ${PROJECT_NAME}Export
        FILE ${PROJECT_NAME}-config.cmake
        DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/${PROJECT_NAME})
