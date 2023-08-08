#pragma once

#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#ifndef HEAP_STRING_ALLOC
#define HEAP_STRING_ALLOC malloc
#endif

#ifndef HEAP_STRING_REALLOC
#define HEAP_STRING_REALLOC realloc
#endif

#ifndef TABLE_STR_CAPACITY_MULTIPLIER
#define TABLE_STR_CAPACITY_MULTIPLIER 1.5
#endif

typedef struct CspTableString {
    char *value;
    uint32_t length;
    uint32_t capacity;
} CspTableString;


CspTableString *newTableString(uint32_t initCapacity);

void tableStringAdd(CspTableString *str, const char *value, uint32_t length);
void tableStringAddChar(CspTableString *str, char charToAdd);

void deleteTableString(CspTableString *str);
