#ifndef REQUEST_PARSER_H
#define REQUEST_PARSER_H

#define METHOD_MAX_LENGTH 10
#define URI_MAX_LENGTH 50 // 50 for starting
#define VERSION_MAX_LENGTH 10

typedef struct
{
    char method[METHOD_MAX_LENGTH]; // Only GET and POST for now
    char uri[URI_MAX_LENGTH];
    char version[VERSION_MAX_LENGTH]; // HTTP/1.1 for now
} RequestLine;

int parse_request_line(char *str, int length, RequestLine *out);

#endif