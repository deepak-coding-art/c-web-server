#include "response.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "logger.h"

int write_headers(int client_fd, Headers headers)
{
    char header_buffer[MAX_HEADERS * sizeof(Header)] = {0};
    headers_to_buffer(headers, header_buffer, sizeof(header_buffer));
    int header_buffer_len = strlen(header_buffer);
    snprintf(header_buffer + header_buffer_len, sizeof(header_buffer) - header_buffer_len, "\r\n");
    header_buffer_len = strlen(header_buffer);
    ssize_t written_header_bytes = write(client_fd, header_buffer, header_buffer_len);
    if (written_header_bytes < 1)
    {
        log_message(LOG_ERROR, "Failed to write response headers: %ld", (long)written_header_bytes);
        return -1;
    }
    return 0;
}

int write_response_line(int client_fd, HttpStatus code)
{
    char response_line_buffer[40] = {0};
    create_response_line(code, response_line_buffer, sizeof(response_line_buffer));
    int length = strlen(response_line_buffer);
    ssize_t written_bytes = write(client_fd, response_line_buffer, length);
    if (written_bytes < 1)
    {
        log_message(LOG_ERROR, "Failed to write response line: %ld", (long)written_bytes);
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
    int response_line_status = write_response_line(client_fd, res_code);
    if (response_line_status < 0)
        return -1;
    int message_length = strlen(message);
    char content_length_str[128] = {0};
    snprintf(content_length_str, sizeof(content_length_str), "%d", message_length);
    Headers headers = {0};
    add_header("Content-Type", "text/html", &headers);
    add_header("Content-Length", content_length_str, &headers);
    int header_status = write_headers(client_fd, headers);
    if (header_status < 0)
        return -1;
    int written = write(client_fd, message, message_length);
    if (written < 0)
    {
        perror("Failed to write to client");
    }
}

int write_not_found(int client_fd)
{
    int status = http_response(client_fd, STATUS_NOT_FOUND, "Not Found\n");
    if (status < 0)
        return -1;
}

int write_bad_req(int client_fd)
{
    int status = http_response(client_fd, STATUS_BAD_REQ, "Bad Request\n");
    if (status < 0)
        return -1;
}
