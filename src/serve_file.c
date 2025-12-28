#include "serve_file.h"
#include "response.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

void http_serve_file(Request *req, int clinet_fd)
{
    printf("File from %s\n", req->url);

    int response_line_status = write_response_line(clinet_fd, STATUS_OK);
    if (response_line_status == -1)
        return;

    Headers headers = {0};
    add_header("Content-Type", "text/html", &headers);
    add_header("Transfer-Encoding", "chunked", &headers);
    int header_status = write_headers(clinet_fd, headers);
    if (header_status == -1)
        return;

    int file_fd = open("./web/index.html", O_RDONLY);
    if (file_fd == -1)
    {
        perror("File open error");
    }
    else
    {
        char read_buffer[FILE_READ_BUFFER_SIZE] = {0};
        ssize_t read_bytes;
        while ((read_bytes = read(file_fd, read_buffer, sizeof(read_buffer))) > 0)
        {
            ssize_t written_bytes = dprintf(clinet_fd, "%x\r\n", read_bytes);
            if (written_bytes < 1)
            {
                printf("Failed to write response: %ld\n", (long)written_bytes);
            }

            written_bytes = write(clinet_fd, read_buffer, read_bytes);
            if (written_bytes < 1)
            {
                printf("Failed to write response: %ld\n", (long)written_bytes);
            }

            written_bytes = write(clinet_fd, "\r\n", 2);
            if (written_bytes < 1)
            {
                printf("Failed to write response: %ld\n", (long)written_bytes);
            }
        }
        ssize_t written_bytes = write(clinet_fd, "0\r\n\r\n", 5);
        if (written_bytes < 1)
        {
            printf("Failed to write response: %ld\n", (long)written_bytes);
        }
    }
    printf("Client disconnected..\n");
}