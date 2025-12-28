#ifndef REQUEST_H
#define REQUEST_H
#include "parsers/request_line_parser.h"

#define READ_BUFFER_SIZE 8
#define ELEMENT_BUFFER_SIZE 1024

// query => ?id=3&age=4
// body => json, form-data
typedef struct
{
    char method[METHOD_MAX_LENGTH];
    char url[URI_MAX_LENGTH];
} Request;

typedef void (*CallBack)(Request *req, int client_fd);

void process_request(int client_fd, CallBack fn);

#endif