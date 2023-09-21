#pragma once

#include <stdio.h>  /* defines FILENAME_MAX */

#if defined(_WIN32) || defined(_WIN64)
    #include <direct.h>
    #define GET_CURRENT_DIR _getcwd
#else
    #include <unistd.h>
    #define GET_CURRENT_DIR getcwd
#endif

#define FILE_SEP FILE_NAME_SEPARATOR_STR
#define RESOURCE_DIR "resources"
#define TEST_FILE_NAME "test_case_file.csp"
#define TEMPLATE_FILE_NAME "test_template.csp"

#include "BaseTestTemplate.h"
#include "CSPRenderer.h"


typedef struct CspTestCase {
    const char *content;
    const char *result;
} CspTestCase;


static const CspTestCase TEST_CASE_ARRAY[] = {
        // Param binding
        {"<p>No params</p>",                                                          "<p>No params</p>"},
        {"<h1>${strParam}</h1><h2>${intParam}</h2><h3>${floatParam}</h3>",            "<h1>Hello World!!!</h1><h2>12345</h2><h3>12.345</h3>"},
        {"<h1>${obj.str}</h1><h2>${obj.int}</h2><h3>${obj.decimal}</h3>",             "<h1>test</h1><h2>2222</h2><h3>3.141593</h3>"},
        {"<h1>${obj.obj.str}</h1><h2>${obj.obj.int}</h2><h3>${obj.obj.decimal}</h3>", "<h1>test2</h1><h2>3333</h2><h3>1.123456</h3>"},
        {"<h1>${obj.obj.obj}</h1><h2>${obj.obj.unknown}</h2>",                        "<h1>[str:test3, innerList:[Inner array, 321, 0.123456]]</h1><h2>NULL</h2>"},
        {"<h1>${list}</h1><h2>${list2}</h2>",                                         "<h1>[Danger, 123, 1.1]</h1><h2>NULL</h2>"},
        {"<h1>${bool_1} ${bool_2} ${int} ${nil} ${decimal} ${str}</h1>",              "<h1>true false 777 NULL 7.0 one</h1>"},
        {"<h1>${obj.obj.obj.innerList}</h1>",                                         "<h1>[Inner array, 321, 0.123456]</h1>"},
        {"<h1>${valList}</h1>",                                                       "<h1>[true, false, NULL, 888, 8.0, two]</h1>"},
        {"<h1>${arrayWithObj}</h1>",                                                  "<h1>[Array with map, [str:test4]]</h1>"},
        {"<h1>${arrayWithArray}</h1>",                                                "<h1>[1, 2, 3, [one, two, three]]</h1>"},

        // Evaluation
        {"${1}",                                                                      "1"},
        {"${-1}",                                                                     "-1"},
        {"${- 1}",                                                                    "-1"},
        {"${ - 1}",                                                                   "-1"},
        {"${ - -1}",                                                                  "1"},
        {"${- - 1}",                                                                  "1"},
        {"${- - - -1}",                                                               "1"},
        {"${- - - -1 - 2}",                                                           "-1"},
        {"${+1}",                                                                     "1"},
        {"${+1.1}",                                                                   "1.1"},
        {"${-1.1}",                                                                   "-1.1"},
        {"${-true}",                                                                  "false"},
        {"${-false}",                                                                 "true"},
        {"${-null}",                                                                  "NULL"},
        {"${+ 1}",                                                                    "1"},
        {"${ + 1}",                                                                   "1"},
        {"${ + +1}",                                                                  "1"},
        {"${ + +-1}",                                                                 "-1"},
        {"${ + +-+ +- -1}",                                                           "-1"},
        {"${-+-+-+-1 - 2}",                                                           "-1"},
        {"${(1)}",                                                                    "1"},
        {"${(1))}",                                                                   "1"},
        {"${-+1}",                                                                    "-1"},
        {"${--1}",                                                                    "1"},
        {"${NULL}",                                                                   "NULL"},
        {"${null}",                                                                   "NULL"},
        {"${1+}",                                                                     "NULL"},
        {"${1++}",                                                                    "NULL"},
        {"${1--}",                                                                    "NULL"},
        {"${1+-}",                                                                    "NULL"},
        {"${1.0}",                                                                    "1.0"},
        {"${-1.0}",                                                                   "-1.0"},
        {"${1.234000}",                                                               "1.234"},
        {"${1.230050}",                                                               "1.23005"},
        {"${1.000000}",                                                               "1.0"},
        {"${\"hello\"}",                                                              "hello"},
        {"${'hello'}",                                                                "hello"},
        {"${'hel\\nlo'}",                                                             "hel\\nlo"},
        {"${'hello' + 1}",                                                            "hello1"},
        {"${'hello' - 1}",                                                            "hello"},
        {"${'hello ' * 3}",                                                           "hello hello hello "},
        {"${'hello ' * 3.0}",                                                         "hello hello hello "},
        {"${'foo' + ' ' + 'baz'}",                                                    "foo baz"},
        {"${'foo' != 'baz'}",                                                         "true"},
        {"${'foo' > 'baz'}",                                                          "true"},
        {"${'foo' < 'baz'}",                                                          "false"},
        {"${3 * 2.22 + 'foo' + 3 * 12 + 4 * 5}",                                      "6.660000foo3620"},
        {"${'foo' + ' says' +':'+' '+ strParam}",                                     "foo says: Hello World!!!"},
        {"${1 + 1 - 0.5}",                                                            "1.5"},
        {"${2*4}",                                                                    "8"},
        {"${4/2}",                                                                    "2"},
        {"${1 > 2}",                                                                  "false"},
        {"${1 > 2 || 3 > 2}",                                                         "true"},
        {"${2 > 3}",                                                                  "false"},
        {"${3 > 2 || (2 > 3 && 1 < 2)}",                                              "true"},
        {"${(1 < 2 && 3 > 2) + 10}",                                                  "11"},
        {"${999 + 777 * (888 + (0.5 + 1.5)) * (0.5 + true)}",                         "1038294.0"},
        {"${999 + 777 * (888 / 0.456) / true}",                                       "1514104.25"},
        {"${22 * true}",                                                              "22"},
        {"${22 * false}",                                                             "0"},
        {"${22.2 * false}",                                                           "0.0"},
        {"${1 != 2}",                                                                 "true"},
        {"${1 >= 2}",                                                                 "false"},
        {"${2 <= 4}",                                                                 "true"},
        {"${4 <= 2}",                                                                 "false"},
        {"${1 == 2}",                                                                 "false"},
        {"${1 = 2}",                                                                  "false"},
        {"${2 = 2}",                                                                  "true"},
        {"${false == true}",                                                          "false"},
        {"${true && false}",                                                          "false"},
        {"${true || false}",                                                          "true"},
        {"${1 || false}",                                                             "1"},
        {"${0 || false}",                                                             "false"},
        {"${1 + - 2}",                                                                "-1"},
        {"${-1 + 2}",                                                                 "1"},
        {"${10 % 2}",                                                                 "0"},
        {"${10 % 3}",                                                                 "1"},
        {"${(1 || (2 > 5)) && (4 < 5 || 5 < 4)}",                                     "true"},
        {"${unknownVar + 2}",                                                         "NULL"},
        {"${true == !!true}",                                                         "true"},
        {"${true == !!true == !false}",                                               "true"},
        {"${true == ! ! true == !false}",                                             "true"},
        {"${true == ! ! true == ! ( 1 == 2 ) }",                                      "true"},
        {"${1 + 4}",                                                                  "5"},
        {"${1 - 4}",                                                                  "-3"},
        {"${2 * 4}",                                                                  "8"},
        {"${2 / 4}",                                                                  "0"},
        {"${2 / 4.0}",                                                                "0.5"},
        {"${10 % 3}",                                                                 "1"},
        {"${10 < 3}",                                                                 "false"},
        {"${10 <= 3}",                                                                "false"},
        {"${10 > 3}",                                                                 "true"},
        {"${10 >= 3}",                                                                "true"},
        {"${10 == 3}",                                                                "false"},
        {"${10 != 3}",                                                                "true"},
        {"${10 && 0}",                                                                "0"}, // should be false?
        {"${10 || 3}",                                                                "10"},
        {"${true ? 1 : 2}",                                                           "1"},
        {"${false ? 1 : 2}",                                                          "2"},
        {"${false ? 1 : true ? 2 : 3}",                                               "2"},
        {"${false ? 1 : false ? 2 : 3}",                                              "3"},
        {"${2 <= 3 ? 'foo' + ' ' + 'baz' : 'not working'}",                           "foo baz"},
        {"${5*2-10 ? 1 : (3*3-9 < 1 || 6+6-12 ? 8 : false) ? 2 : 3}",                 "2"},
        {"${variable ? 1 : 2}",                                                       "2"},
        {"${variable == null ? 1 : 2}",                                               "1"},
        {"${variable ?: 2}",                                                          "2"},
        {"${null ?: 23}",                                                             "23"},
        {"${intParam ?: 2}",                                                          "12345"},
        {"${false ?: 2}",                                                             "2"},
        {"${true ?: 2}",                                                              "true"},
        {"${1 + 2 ?: 2}",                                                             "3"},
        {"${1 - 1 ?: 2}",                                                             "2"},
        {"${1 != 2 > 1 != 1}",                                                        "true"},
        {"${1 != 2 < 1 != 1}",                                                        "false"},
        {"${1 != 1 < 2 != 1}",                                                        "true"},
        {"${(1 != 2) > (1 != 1)}",                                                    "true"},
        {"${(1 != 2) < (1 != 1)}",                                                    "false"},
        {"${(1 != 1) < (2 != 1)}",                                                    "true"},
        {"${'1' == 1}",                                                               "true"},
        {"${'2' == 1}",                                                               "false"},
        {"${'aaa' == 'aaa'}",                                                         "true"},
        {"${'Aa' != 'aa'}",                                                           "true"},
        {"${null == null}",                                                           "true"},
        {"${3 ** 2}",                                                                 "9"},
        {"${3 ** 2.0}",                                                               "9.0"},
        {"${3.0 ** 2}",                                                               "9.0"},
        {"${3.0 ** 2.0}",                                                             "9.0"},
        {"${null ** 2}",                                                              "NULL"},
        {"${3 ** null}",                                                              "NULL"},
        {"${(-1 + 2) * 3 - -4}",                                                      "7"},
        {"${!(5 - 4 > 3 * 2)}",                                                       "true"},
        {"${22 + 23}",                                                                "45"},
        {"${2.0 + true}",                                                             "3.0"},
        {"${false + 1.0}",                                                            "1.0"},
        {"${2 + 1.0}",                                                                "3.0"},
        {"${1.0 + 2}",                                                                "3.0"},
        {"${1 + true}",                                                               "2"},
        {"${1 + false}",                                                              "1"},
        {"${false + 1}",                                                              "1"},
        {"${true + 1}",                                                               "2"},
        {"${true + false}",                                                           "1"},
        {"${false + false}",                                                          "0"},
        {"${1 + 'baz'}",                                                              "1baz"},
        {"${null + null}",                                                            "NULLNULL"},
        {"${null + 'baz'}",                                                           "NULLbaz"},
        {"${'baz' + null}",                                                           "bazNULL"},
        {"${obj.obj.str + ' ' + 'baz'}",                                              "test2 baz"},
        {"${list + 'baz' + 1 + 2.2}",                                                 "[Danger, 123, 1.1, baz, 1, 2.2]"},
        {"${3.0 - 2.0}",                                                              "1.0"},
        {"${3.33 - 2.22}",                                                            "1.11"},
        {"${3.0 - 1}",                                                                "2.0"},
        {"${3 - 1.0}",                                                                "2.0"},
        {"${3 - 1}",                                                                  "2"},
        {"${null - null}",                                                            "NULL"},
        {"${1.0 - null}",                                                             "NULL"},
        {"${1 - null}",                                                               "NULL"},
        {"${null - 1.0}",                                                             "NULL"},
        {"${null - 1}",                                                               "NULL"},
        {"${'foo' - 'baz'}",                                                          "foo"},
        {"${2.0 * 3.22}",                                                             "6.44"},
        {"${2.0 * 3}",                                                                "6.0"},
        {"${2 * 3.0}",                                                                "6.0"},
        {"${2 * 3}",                                                                  "6"},
        {"${2 * true}",                                                               "2"},
        {"${2 * false}",                                                              "0"},
        {"${true * 2}",                                                               "2"},
        {"${false * 2}",                                                              "0"},
        {"${true * 2.0}",                                                             "2.0"},
        {"${false * 2.0}",                                                            "0.0"},
        {"${2.1 * true}",                                                             "2.1"},
        {"${2.1 * false}",                                                            "0.0"},
        {"${2.1 * null}",                                                             "NULL"},
        {"${null * 2}",                                                               "NULL"},
        {"${null * null}",                                                            "NULL"},
        {"${4.0 / 2.0}",                                                              "2.0"},
        {"${4.0 / 2}",                                                                "2.0"},
        {"${4 / 2.0}",                                                                "2.0"},
        {"${4 / 2}",                                                                  "2"},
        {"${4.0 / true}",                                                             "4.0"},
        {"${false / 2.0}",                                                            "0.0"},
        {"${true / 2.0}",                                                             "0.5"},
        {"${4 / true}",                                                               "4"},
        {"${true / 2}",                                                               "0.5"},
        {"${false / 2}",                                                              "0.0"},
        {"${null / 2}",                                                               "NULL"},
        {"${null / null}",                                                            "NULL"},
        {"${2.0 / null}",                                                             "NULL"},
        {"${10.0 % 2.0}",                                                             "0.0"},
        {"${10.0 % 3}",                                                               "1.0"},
        {"${10 % 3.0}",                                                               "1.0"},
        {"${10 % 3}",                                                                 "1"},
        {"${null % 3}",                                                               "NULL"},
        {"${10 % null}",                                                              "NULL"},
        {"${1.0 == 1.0}",                                                             "true"},
        {"${1.0 == 2.0}",                                                             "false"},
        {"${1.0 == 1}",                                                               "true"},
        {"${1.0 == 2}",                                                               "false"},
        {"${1 == 1.0}",                                                               "true"},
        {"${2 == 1.0}",                                                               "false"},
        {"${2 == 2}",                                                                 "true"},
        {"${2 == 3}",                                                                 "false"},
        {"${null == 3}",                                                              "false"},
        {"${null == null}",                                                           "true"},
        {"${'foo' == 'baz'}",                                                         "false"},
        {"${'foo' == 'foo'}",                                                         "true"},
        {"${'1234' == 1234}",                                                         "true"},
        {"${'123' == 1234}",                                                          "false"},
        {"${123 == '1234'}",                                                          "false"},
        {"${123 == '123'}",                                                           "true"},
        {"${1.0 == '1.0'}",                                                           "true"},
        {"${1.0 == '1.1'}",                                                           "false"},
        {"${'1.0' == 1.0}",                                                           "true"},
        {"${'1.0' == 1.1}",                                                           "false"},
        {"${'true' == true}",                                                         "true"},
        {"${'true' == false}",                                                        "false"},
        {"${'baz' == false}",                                                         "false"},
        {"${1.0 == true}",                                                            "true"},
        {"${1.0 == false}",                                                           "false"},
        {"${0.0 == false}",                                                           "true"},
        {"${0 == false}",                                                             "true"},
        {"${1 == true}",                                                              "true"},
        {"${1 == false}",                                                             "false"},
        {"${2 == false}",                                                             "false"},
        {"${2 == true}",                                                              "true"},
        {"${true == true}",                                                           "true"},
        {"${true == 'true'}",                                                         "true"},
        {"${true == 'false'}",                                                        "false"},
        {"${true == 'baz'}",                                                          "false"},
        {"${true == 1.0}",                                                            "true"},
        {"${false == 1.0}",                                                           "false"},
        {"${false == 0.0}",                                                           "true"},
        {"${false == 0}",                                                             "true"},
        {"${true == 1}",                                                              "true"},
        {"${false == 1}",                                                             "false"},
        {"${false == 2}",                                                             "false"},
        {"${true == 2}",                                                              "true"},
        {"${true == true}",                                                           "true"},
        {"${2.0 > 1.0}",                                                              "true"},
        {"${0.5 > 1.0}",                                                              "false"},
        {"${5.5 > 1}",                                                                "true"},
        {"${5.5 > 7}",                                                                "false"},
        {"${5 > 4.9}",                                                                "true"},
        {"${5 > 5.9}",                                                                "false"},
        {"${5 > 5}",                                                                  "false"},
        {"${6 > 5}",                                                                  "true"},
        {"${null > 5.0}",                                                             "false"},
        {"${6 > null}",                                                               "false"},
        {"${null > null}",                                                            "false"},
        {"${true > false}",                                                           "true"},
        {"${false > false}",                                                          "false"},
        {"${true > true}",                                                            "false"},
        {"${false > true}",                                                           "false"},
        {"${2.0 >= 1.0}",                                                             "true"},
        {"${2.0 >= 2.0}",                                                             "true"},
        {"${0.5 >= 1.0}",                                                             "false"},
        {"${5.5 >= 1}",                                                               "true"},
        {"${5.5 >= 7}",                                                               "false"},
        {"${5 >= 4.9}",                                                               "true"},
        {"${5 >= 5.9}",                                                               "false"},
        {"${5 >= 5}",                                                                 "true"},
        {"${6 >= 5}",                                                                 "true"},
        {"${null >= 5.0}",                                                            "false"},
        {"${6 >= null}",                                                              "false"},
        {"${null >= null}",                                                           "true"},
        {"${true >= false}",                                                          "true"},
        {"${false >= false}",                                                         "true"},
        {"${true >= true}",                                                           "true"},
        {"${false >= true}",                                                          "false"},
        {"${2.0 < 1.0}",                                                              "false"},
        {"${2.0 < 2.0}",                                                              "false"},
        {"${0.5 < 1.0}",                                                              "true"},
        {"${5.5 < 1}",                                                                "false"},
        {"${5.5 < 7}",                                                                "true"},
        {"${5 < 4.9}",                                                                "false"},
        {"${5 < 5.9}",                                                                "true"},
        {"${5 < 5}",                                                                  "false"},
        {"${6 < 5}",                                                                  "false"},
        {"${null < 5.0}",                                                             "false"},
        {"${6 < null}",                                                               "false"},
        {"${null < null}",                                                            "false"},
        {"${true < false}",                                                           "false"},
        {"${false < false}",                                                          "false"},
        {"${true < true}",                                                            "false"},
        {"${false < true}",                                                           "true"},
        {"${2.0 <= 1.0}",                                                             "false"},
        {"${2.0 <= 2.0}",                                                             "true"},
        {"${0.5 <= 1.0}",                                                             "true"},
        {"${5.5 <= 1}",                                                               "false"},
        {"${5.5 <= 7}",                                                               "true"},
        {"${5 <= 4.9}",                                                               "false"},
        {"${5 <= 5.9}",                                                               "true"},
        {"${5 <= 5}",                                                                 "true"},
        {"${6 <= 5}",                                                                 "false"},
        {"${null <= 5.0}",                                                            "false"},
        {"${6 <= null}",                                                              "false"},
        {"${null <= null}",                                                           "false"},
        {"${true <= false}",                                                          "false"},
        {"${false <= false}",                                                         "true"},
        {"${true <= true}",                                                           "true"},
        {"${false <= true}",                                                          "true"},
        {"${!'foo'}",                                                                 "false"},
        {"${!''}",                                                                    "true"},
};

static const CspTestCase ERROR_TEST_CASE_ARRAY[] = {
        {"${}",                                                                      "Empty template parameter"},
        {"${",                                                                       "Unclosed template parameter"},
        {"${<p></p>",                                                                "Unclosed template parameter"},
        {"<csp:unknown tag/>",                                                       "Unknown tag after [<csp: ]"},
        {"<csp:if test=\"${1}\">",                                                   "Template contains unclosed tag"},
        {"<csp:if test=\"${1}\"></csp:fi>",                                          "Unknown closing tag after [</csp:]"},
        {"<!-- test --",                                                             "Unclosed Html comment tag"},
        {"<csp:if></csp:if>",                                                        "Mandatory attribute for [if] tag: [test] not found"},
        {"<csp:if test=\"${1}\"></csp:else><csp:elseif test=\"${2}\"></csp:elseif>", "Invalid '<csp:elseif' branching"},
        {"<csp:elseif test=\"${2}\"></csp:elseif>",                                  "Invalid '<csp:elseif' branching"},
        {"<csp:else></csp:else>",                                                    "Invalid '<csp:else' branching"},
        {"<csp:set></csp:set>",                                                      "Mandatory attribute for [set] tag: [var] not found"},
        {"<csp:set var=\"test\"></csp:set>",                                         "Mandatory attribute for [set] tag: [value] not found"},
        {"<csp:loop test=\"test\"></csp:loop>",                                      "Mandatory attribute for [loop] tag: [in] not found"},
        {"<csp:loop var=\"val\" in=\"obj\"><!--${param}--></csp:loop>",              "Unsupported value type. Only arrays is allowed"},
        {"<csp:render test=\"test\"/>",                                              "Mandatory attribute for [render] tag: [template] not found"},
        {"<csp:render template=\"unclosed/>",                                        "Unterminated attribute value"},
        {"<csp:render template=\"${param\"/>",                                       "No closing '}' found for parameter tag"},
        {"<csp:render template=\"not_exist.csp\"/>",                                 "Can't open template file"},
        {"${1 != 2 ? 3 , 5}",                                                        "Unterminated ternary expression"},
        {"${1 | 2}",                                                                 "Unsupported binary OR operators only Logical OR is supported"},
        {"${1 & 2}",                                                                 "Unsupported binary AND operators only Logical AND is supported"},
        {"${1VarName}",                                                              "Invalid variable name identifier"},
        {"${#varName}",                                                              "Unexpected lexer character: [#]"},
        {"${%varName}",                                                              "Expect expression"},
        {"${'varName}",                                                              "Unterminated string"},
        {"${\"varName}",                                                             "Unterminated string"},
        {"${['a', 'b', 'c'}",                                                        "Unterminated collection. Missing end bracket ']'"},
        {"${['a':'v1', 'b':'v2', 'c':'v3'}",                                         "Unterminated collection. Missing end bracket ']'"},
        {"${['a':'v1', b:'v2', 'c':'v3']}",                                          "Map Object: Only string as a key allowed"},
        {"${['a':'v1', 'b''v2', 'c':'v3']}",                                         "Map Object: Missing key value separator ':'"},
        {"${[!]}",                                                                   "Invalid literal token type"},
        {"${(1+2}",                                                                  "Expect ')' after expression."},
        {"${1 / 0}",                                                                 "Division by zero"},
        {"${1.0 / 0}",                                                               "Division by zero"},
        {"${1.0 / 0.0}",                                                             "Division by zero"},
        {"${1 / 0.0}",                                                               "Division by zero"},
        {"${1 / false}",                                                             "Division by zero"},
        {"${true / 0.0}",                                                            "Division by zero"},
        {"${1 / false}",                                                             "Division by zero"},
        {"${['one'...5]}",                                                           "Invalid range from value! Only int type allowed"},
        {"${[1...val]}",                                                             "Invalid range to value! Only int type allowed"},
        {"${[8...6]}",                                                               "Invalid range! From value can't be greater than to value"},

};

static CspObjectMap *getPredefinedParams() {
    CspObjectMap *paramMap = newCspParamObjMap(32);
    cspAddStrToMap(paramMap, "strParam", "Hello World!!!");
    cspAddStrToMap(paramMap, "name", "SomeName");
    cspAddStrToMap(paramMap, "message", "Some Message");
    cspAddStrToMap(paramMap, "drop", "enabled");
    cspAddIntToMap(paramMap, "intParam", 12345);
    cspAddFloatToMap(paramMap, "floatParam", 12.345f);

    CspObjectMap *innerObj_1 = newCspParamObjMap(8);
    cspAddStrToMap(innerObj_1, "str", "test");
    cspAddIntToMap(innerObj_1, "int", 2222);
    cspAddIntToMap(innerObj_1, "int1", 1);
    cspAddIntToMap(innerObj_1, "int2", 2);
    cspAddIntToMap(innerObj_1, "int3", 3);
    cspAddFloatToMap(innerObj_1, "decimal", 3.14159265359f);
    cspAddMapToMap(innerObj_1, paramMap, "obj");

    CspObjectMap *innerObj_2 = newCspParamObjMap(8);
    cspAddStrToMap(innerObj_2, "str", "test2");
    cspAddIntToMap(innerObj_2, "int", 3333);
    cspAddFloatToMap(innerObj_2, "decimal", 1.123456f);
    cspAddMapToMap(innerObj_2, innerObj_1, "obj");

    CspObjectMap *innerObj_3 = newCspParamObjMap(8);
    cspAddStrToMap(innerObj_3, "str", "test3");
    cspAddMapToMap(innerObj_3, innerObj_2, "obj");

    CspObjectArray *innerArray = newCspParamObjArray(8);
    cspAddStrToArray(innerArray, "Inner array");
    cspAddIntToArray(innerArray, 321);
    cspAddFloatToArray(innerArray, 0.123456f);
    cspAddVecToMap(innerArray, innerObj_3, "innerList");

    CspObjectArray *paramArray = newCspParamObjArray(8);
    cspAddStrToArray(paramArray, "Danger");
    cspAddIntToArray(paramArray, 123);
    cspAddFloatToArray(paramArray, 1.1f);
    cspAddVecToMap(paramArray, paramMap, "list");

    // CSP values to map
    cspAddValToMap(paramMap, "bool_1", CSP_BOOL_TRUE_VALUE());
    cspAddValToMap(paramMap, "bool_2", CSP_BOOL_FALSE_VALUE());
    cspAddValToMap(paramMap, "nil", CSP_NULL_VALUE());
    cspAddValToMap(paramMap, "int", CSP_INT_VALUE(777));
    cspAddValToMap(paramMap, "decimal", CSP_FLOAT_VALUE(7));
    cspAddValToMap(paramMap, "str", CSP_STR_VALUE("one"));

    // CSP values to array
    CspObjectArray *valArray = newCspParamObjArray(8);
    cspAddValToArray(valArray, CSP_BOOL_TRUE_VALUE());
    cspAddValToArray(valArray, CSP_BOOL_FALSE_VALUE());
    cspAddValToArray(valArray, CSP_NULL_VALUE());
    cspAddValToArray(valArray, CSP_INT_VALUE(888));
    cspAddValToArray(valArray, CSP_FLOAT_VALUE(8));
    cspAddValToArray(valArray, CSP_STR_VALUE("two"));
    cspAddVecToMap(valArray, paramMap, "valList");

    // Array with map
    CspObjectArray *arrayWithMap = newCspParamObjArray(8);
    cspAddStrToArray(arrayWithMap, "Array with map");
    CspObjectMap *innerObj_4 = newCspParamObjMap(8);
    cspAddStrToMap(innerObj_4, "str", "test4");
    cspAddMapToArray(innerObj_4, arrayWithMap);
    cspAddVecToMap(arrayWithMap, paramMap, "arrayWithObj");

    // Array in array
    CspObjectArray *arrayWithArray = newCspParamObjArray(8);
    cspAddIntToArray(arrayWithArray, 1);
    cspAddIntToArray(arrayWithArray, 2);
    cspAddIntToArray(arrayWithArray, 3);
    CspObjectArray *innerArray_2 = newCspParamObjArray(8);
    cspAddStrToArray(innerArray_2, "one");
    cspAddStrToArray(innerArray_2, "two");
    cspAddStrToArray(innerArray_2, "three");
    cspAddArrayToArray(innerArray_2, arrayWithArray);
    cspAddVecToMap(arrayWithArray, paramMap, "arrayWithArray");
    return paramMap;
}

static MunitResult testHappyPath(const MunitParameter params[], void *data) {
    printf("\n");
    char currentDir[FILENAME_MAX];
    GET_CURRENT_DIR(currentDir, FILENAME_MAX);
    BufferString *rootPath = SUBSTRING_CSTR(512, currentDir, 0, lastIndexOfCStr(currentDir, FILE_SEP));
    BufferString *testFilePath = STRING_FORMAT_512("%S" FILE_SEP "CSP" FILE_SEP RESOURCE_DIR FILE_SEP TEST_FILE_NAME, rootPath);
    File *testFile = NEW_FILE(testFilePath->value);

    for (int i = 0; i < ARRAY_SIZE(TEST_CASE_ARRAY); i++) {
        CspTestCase testCase = TEST_CASE_ARRAY[i];
        printf("Test case No. %d. Pattern [%s]\n", i + 1, testCase.content);
        writeCharsToFile(testFile, testCase.content, strlen(testCase.content), false);

        CspTemplate *cspTemplate = newCspTemplate(testFilePath->value);
        CspRenderer *renderer = NEW_CSP_RENDERER(cspTemplate, getPredefinedParams());
        CspTableString *str = renderCspTemplate(renderer);
        if (!isCspTemplateOk(cspTemplate)) {
            printf("%s\n", cspTemplateErrorMessage(cspTemplate));
            deleteCspRenderer(renderer);
            deleteCspTemplate(cspTemplate);
            return MUNIT_FAIL;
        }

        assert_string_equal(str->value, testCase.result);
        deleteCspRenderer(renderer);
        deleteCspTemplate(cspTemplate);
    }

    return MUNIT_OK;
}

static MunitResult testErrorMessages(const MunitParameter params[], void *data) {
    printf("\n");
    char currentDir[FILENAME_MAX];
    GET_CURRENT_DIR(currentDir, FILENAME_MAX);
    BufferString *rootPath = SUBSTRING_CSTR(512, currentDir, 0, lastIndexOfCStr(currentDir, FILE_SEP));
    BufferString *testFilePath = STRING_FORMAT_512("%S" FILE_SEP "CSP" FILE_SEP RESOURCE_DIR FILE_SEP TEST_FILE_NAME, rootPath);
    File *testFile = NEW_FILE(testFilePath->value);

    for (int i = 0; i < ARRAY_SIZE(ERROR_TEST_CASE_ARRAY); i++) {
        CspTestCase testCase = ERROR_TEST_CASE_ARRAY[i];
        writeCharsToFile(testFile, testCase.content, strlen(testCase.content), false);

        CspTemplate *cspTemplate = newCspTemplate(testFilePath->value);
        CspRenderer *renderer = NEW_CSP_RENDERER(cspTemplate, getPredefinedParams());
        renderCspTemplate(renderer);

        char *errorMessage = cspTemplateErrorMessage(cspTemplate);
        BufferString *message = SUBSTRING_CSTR_AFTER(1024, errorMessage != NULL ? errorMessage : "", "]:");
        printf("Error test case No. %d. Pattern [%s]. Message:%s\n", i + 1, testCase.content, message->value);
        if (isCspTemplateOk(cspTemplate)) {
            deleteCspRenderer(renderer);
            deleteCspTemplate(cspTemplate);
            return MUNIT_FAIL;
        }

        assert_true(strstr(errorMessage, testCase.result) != NULL);
        deleteCspRenderer(renderer);
        deleteCspTemplate(cspTemplate);
    }

    return MUNIT_OK;
}

static MunitResult fullFeatureTemplateTest(const MunitParameter params[], void *data) {
    printf("\n");
    char currentDir[FILENAME_MAX];
    GET_CURRENT_DIR(currentDir, FILENAME_MAX);
    BufferString *rootPath = SUBSTRING_CSTR(512, currentDir, 0, lastIndexOfCStr(currentDir, FILE_SEP));
    BufferString *testFilePath = STRING_FORMAT_512("%S" FILE_SEP "CSP" FILE_SEP RESOURCE_DIR FILE_SEP TEMPLATE_FILE_NAME, rootPath);
    File *testFile = NEW_FILE(testFilePath->value);
    File *resultFile = NEW_FILE("../CSP/resources/result.html");

    CspTemplate *cspTemplate = newCspTemplate(testFile->path);
    CspRenderer *renderer = NEW_CSP_RENDERER(cspTemplate, getPredefinedParams());
    CspTableString *str = renderCspTemplate(renderer);
    if (!isCspTemplateOk(cspTemplate)) {
        printf("%s\n", cspTemplateErrorMessage(cspTemplate));
        deleteCspRenderer(renderer);
        deleteCspTemplate(cspTemplate);
        return MUNIT_FAIL;
    }
    writeCharsToFile(resultFile, str->value, str->length, false);

    assert_not_null(strstr(str->value, "Test Title"));
    assert_not_null(strstr(str->value, "<h1>Href Attribute Example</h1>"));
    assert_not_null(strstr(str->value, "<h1 class=\"display-2\">Some Message</h1>"));
    assert_not_null(strstr(str->value, "<li class=\"list-group-item active\">Danger</li>"));
    assert_not_null(strstr(str->value, "<li class=\"list-group-item\">123</li>"));
    assert_not_null(strstr(str->value, "<li class=\"list-group-item\">1.1</li>"));
    assert_not_null(strstr(str->value, "<h2>[one, Some Message, 123, 1.23, some, 1, 2, 3, 4, 5]</h2>"));
    assert_not_null(strstr(str->value, "<h2>[0, 1, 2, 3, 4, 5, 6, 7, 8, 9]</h2>"));
    assert_not_null(strstr(str->value, "<h2>[1, 2, 3, 12345, enabled]</h2>"));
    assert_not_null(strstr(str->value, "k1:Some Message"));
    assert_not_null(strstr(str->value, "<h2>Hello World!!!</h2>"));
    assert_not_null(strstr(str->value, "<h3>[1, 2, 3, 4, 5]</h3>"));
    assert_not_null(strstr(str->value, "<p>Not empty value == test2!</p>"));
    assert_not_null(strstr(str->value, "<p>Hello test3 visible!</p>"));
    assert_not_null(strstr(str->value, "<p>This 'Else if' should be visible</p>"));
    assert_not_null(strstr(str->value, "Empty msg dropdown"));
    assert_not_null(strstr(str->value, "<a class=\"dropdown-item disabled\" href=\"#\">1. Danger </a>"));
    assert_not_null(strstr(str->value, "<a class=\"dropdown-item \" href=\"#\">2. 123 </a>"));
    assert_not_null(strstr(str->value, "<a class=\"dropdown-item \" href=\"#\">3. 1.1 </a>"));
    assert_not_null(strstr(str->value, "<li class=\"list-group-item\">1</li>"));
    assert_not_null(strstr(str->value, "<li class=\"list-group-item\">2</li>"));
    assert_not_null(strstr(str->value, "<li class=\"list-group-item\">3</li>"));
    assert_not_null(strstr(str->value, "<li class=\"list-group-item\">3.141593</li>"));
    assert_not_null(strstr(str->value, "<li class=\"list-group-item\">Some Message</li>"));
    assert_not_null(strstr(str->value, "<li class=\"list-group-item\">6</li>"));
    assert_not_null(strstr(str->value, "<li class=\"list-group-item\">v3</li>"));
    assert_not_null(strstr(str->value, "<li class=\"list-group-item\">4.4</li>"));
    assert_not_null(strstr(str->value, "<li class=\"list-group-item\">1</li>"));
    assert_not_null(strstr(str->value, "<li class=\"list-group-item\">2</li>"));
    assert_not_null(strstr(str->value, "<li class=\"list-group-item\">3</li>"));
    assert_not_null(strstr(str->value, "<li class=\"list-group-item\">one</li>"));
    assert_not_null(strstr(str->value, "<li class=\"list-group-item\">Some Message</li>"));
    assert_not_null(strstr(str->value, "<li class=\"list-group-item\">123</li>"));
    assert_not_null(strstr(str->value, "<li class=\"list-group-item\">1.23</li>"));
    assert_not_null(strstr(str->value, "<li class=\"list-group-item\">some</li>"));

    // Should not be present in result
    assert_null(strstr(str->value, "<p>Hello not visible!</p>"));
    assert_null(strstr(str->value, "<p>This 'Else if' should NOT be visible even if true</p>"));
    assert_null(strstr(str->value, "<p>Logic not working!!!</p>"));
    assert_null(strstr(str->value, "<p>Logic works for nesting!!!</p>"));
    assert_null(strstr(str->value, "<p>You test are not welcome here</p>"));

    return MUNIT_OK;
}

static MunitTest cspTests[] = {
        {.name =  "Happy path - should correctly bind params and evaluate expressions", .test = testHappyPath},
        {.name =  "Error handling - should correctly handle errors", .test = testErrorMessages},
        {.name =  "Render test - should correctly render template", .test = fullFeatureTemplateTest},
        END_OF_TESTS
};

static const MunitSuite cspTestSuite = {
        .prefix = "\nCSP: ",
        .tests = cspTests,
        .suites = NULL,
        .iterations = 1,
        .options = MUNIT_SUITE_OPTION_NONE
};
