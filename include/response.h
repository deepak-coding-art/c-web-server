#ifndef RESPONSE_H
#define RESPONSE_H
#include <stddef.h>
#include "../include/utils/http_things.h"

void create_response_line(HttpStatus code, char *buffer, size_t size);

int http_response(int client_fd, HttpStatus res_code, char *message);

#endif