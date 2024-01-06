/**
 * @file array.h
 * @author Michael Baumgarten
 * @brief Preprocessor macros for creating arrays of any type
 */

#pragma once
#include <stdint.h>

#define ARRAY_DEFINE(name, type) \
typedef struct name##Array name##Array; \
struct name##Array { \
    type** items; \
    int32_t size; \
    int32_t length; \
    int32_t index; \
}; \
void name##Array_init(name##Array* array, int32_t initialSize); \
void name##Array_append(name##Array* array, type* item); \
void name##Array_free(name##Array* array); \
type* name##Array_iterator(name##Array* array, int32_t* index); \


#define ARRAY_INIT(name, type) \
void name##Array_init(name##Array* array, int32_t initialSize) { \
    array->items = malloc(initialSize * sizeof(type*)); \
    array->size = initialSize; \
    array->length = 0; \
    array->index = 0; \
} \
void name##Array_append(name##Array* array, type* item) { \
    if (array->length == array->size) { \
        array->size *= 2; \
        array->items = realloc(array->items, array->size * sizeof(type*)); \
    } \
    array->items[array->length] = item; \
    array->length++; \
} \
void name##Array_free(name##Array* array) { \
    free(array->items); \
    array->size = 0; \
    array->length = 0; \
    array->index = 0; \
} \
type* name##Array_iterator(name##Array* array, int32_t* index) { \
    if (*index >= array->length) { \
        return NULL; \
    } \
    type* item = array->items[*index]; \
    *index += 1; \
    return item; \
}
