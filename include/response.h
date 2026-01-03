#ifndef RESPONSE_H
#define RESPONSE_H
#include <stddef.h>
#include "parsers/header_parser.h"
#include "utils/http_things.h"

void create_response_line(HttpStatus code, char *buffer, size_t size);

void add_header(char *key, char *value, Headers *out);

void headers_to_buffer(Headers headers, char *buffer, size_t size);

int write_response_line(int client_fd, HttpStatus code);

int write_headers(int client_fd, Headers headers);

int http_response(int client_fd, HttpStatus res_code, char *message);

int write_not_found(int client_fd);

int write_bad_req(int client_fd);

#endif