#include "response.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>

int write_headers(int clinet_fd, Headers headers)
{
    char header_buffer[MAX_HEADERS * sizeof(Header)] = {0};
    headers_to_buffer(headers, header_buffer, sizeof(header_buffer));
    int header_buffer_len = strlen(header_buffer);
    snprintf(header_buffer + header_buffer_len, sizeof(header_buffer) - header_buffer_len, "\r\n");
    header_buffer_len = strlen(header_buffer);
    ssize_t written_header_bytes = write(clinet_fd, header_buffer, header_buffer_len);
    if (written_header_bytes < 1)
    {
        printf("Failed to write response headers: %ld\n", (long)written_header_bytes);
        return -1;
    }
    return 0;
}

int write_response_line(int clinet_fd, HttpStatus code)
{
    char response_line_buffer[40] = {0};
    create_response_line(code, response_line_buffer, sizeof(response_line_buffer));
    int length = strlen(response_line_buffer);
    ssize_t written_bytes = write(clinet_fd, response_line_buffer, length);
    if (written_bytes < 1)
    {
        printf("Failed to write response line: %ld\n", (long)written_bytes);
        return -1;
    }
    return 0;
}

void add_header(char *key, char *value, Headers *out)
{
    strcpy(out->headers[out->size].key, key);
    strcpy(out->headers[out->size].value, value);
    out->size = out->size + 1;
}

void headers_to_buffer(Headers headers, char *buffer, size_t size)
{
    size_t off = 0;
    for (int i = 0; i < headers.size; i++)
    {
        int n = snprintf(buffer + off, size - off, "%s: %s\r\n", headers.headers[i].key, headers.headers[i].value);
        if (n < 0 || (size_t)n >= size - off)
            break;
        off += n;
    }
}

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