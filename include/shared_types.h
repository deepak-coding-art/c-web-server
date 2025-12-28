#ifndef SHARED_TYPES
#define SHARED_TYPES
#include "string.h"

typedef struct
{
    char *data;
    size_t size;
    size_t capacity;
} String;

#endif