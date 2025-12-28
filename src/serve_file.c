#include "serve_file.h"
#include "response.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

void http_serve_file(Request *req, int clinet_fd)
{
    printf("File from %s\n", req->url);

    char response_line_buffer[40] = {0};
    create_response_line(STATUS_OK, response_line_buffer, sizeof(response_line_buffer));

    char response_buffer[128] = {0};
    snprintf(response_buffer, sizeof(response_buffer), "%s\r\n", response_line_buffer);
    int length = strlen(response_buffer);
    ssize_t written_bytes = write(clinet_fd, response_buffer, length);
    if (written_bytes < 1)
    {
        printf("Failed to write response: %ld\n", (long)written_bytes);
    }
    int file_fd = open("./web/futurama-think.webp", O_RDONLY);
    if (file_fd == -1)
    {
        perror("File open error");
    }
    else
    {
        char read_buffer[1024] = {0};
        int read_bytes;
        while ((read_bytes = read(file_fd, read_buffer, sizeof(read_buffer))) > 0)
        {
            written_bytes = write(clinet_fd, read_buffer, read_bytes);
            if (written_bytes < 1)
            {
                printf("Failed to write response: %ld\n", (long)written_bytes);
            }
        }
    }
    printf("Client disconnected..\n");
}