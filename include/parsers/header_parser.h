#ifndef HEADERS_PARSER_H
#define HEADERS_PARSER_H

#include "string.h"

#define HEADER_KEY_MAX_LENGTH 128
#define HEADER_VALUE_MAX_LENGTH 1024

#define MAX_HEADERS 20

typedef struct
{
    char key[HEADER_KEY_MAX_LENGTH];
    char value[HEADER_VALUE_MAX_LENGTH];
} Header;

typedef struct
{
    Header headers[MAX_HEADERS];
    int size;
} Headers;

int parse_headers(char *str, size_t length, Headers *out);

#endif