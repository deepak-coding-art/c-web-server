#include "response.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>

void create_response_line(HttpStatus code, char *buffer, size_t size)
{
    snprintf(buffer, size, "HTTP/1.1 %d %s\r\n", code, code_to_reason(code));
}

int http_response(int client_fd, HttpStatus res_code, char *message)
{
    char response_line[128] = {0};
    create_response_line(res_code, response_line, sizeof(response_line));
    char response_buffer[1024] = {0};
    size_t buffer_size = sizeof(response_buffer);
    snprintf(response_buffer, buffer_size, "%s\r\n%s", response_line, message);
    int length = strlen(response_buffer);
    if (buffer_size < length)
    {
        length = (int)buffer_size;
    }
    int written = write(client_fd, response_buffer, length);
    if (written < 0)
    {
        perror("Failed to write to client");
    }
}