#ifndef SHARED_TYPES
#define SHARED_TYPES
#include "../include/request_line_parser.h"

// query => ?id=3&age=4
// body => json, form-data
typedef struct
{
    char method[METHOD_MAX_LENGTH];
    char url[URI_MAX_LENGTH];
} Request;

typedef void (*CallBack)(Request *req, int client_fd);

#endif