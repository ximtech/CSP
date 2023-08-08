#include "CSPTableString.h"

CspTableString *newTableString(uint32_t initCapacity) {
    if (initCapacity == 0) return NULL;
    CspTableString *str = HEAP_STRING_ALLOC(sizeof(struct CspTableString));
    if (str == NULL) {
        return NULL;
    }

    char *valueStr = HEAP_STRING_ALLOC(sizeof(char) * initCapacity + 1);
    if (valueStr == NULL) {
        free(str);
        return NULL;
    }
    memset(valueStr, 0, initCapacity + 1);

    str->length = 0;
    str->capacity = initCapacity;
    str->value = valueStr;
    return str;
}

void tableStringAdd(CspTableString *str, const char *text, uint32_t textLength) {
    if (str->length + textLength > str->capacity ) {
        str->capacity += textLength;
        char *reValue = HEAP_STRING_REALLOC(str->value, sizeof(char) * str->capacity + 1);
        if (reValue != NULL) {
            str->value = reValue;
        }
    }

    uint32_t textIndex = 0;
    for (uint32_t i = str->length; i < str->length + textLength; i++) {
        str->value[i] = text[textIndex];
        textIndex++;
    }
    str->length += textLength;
    str->value[str->length] = '\0';
}

void tableStringAddChar(CspTableString *str, char charToAdd) {
    if (str->length + 1 > str->capacity ) {
        str->capacity = (uint32_t) (str->capacity * TABLE_STR_CAPACITY_MULTIPLIER);
        char *reValue = HEAP_STRING_REALLOC(str->value, sizeof(char) * str->capacity + 1);
        if (reValue != NULL) {
            str->value = reValue;
        }
        memset(reValue + str->length, 0, (str->capacity - str->length) + 1);
    }
    str->value[str->length++] = charToAdd;
}

void deleteTableString(CspTableString *str) {
    if (str != NULL) {
        free(str->value);
        free(str);
    }
}
