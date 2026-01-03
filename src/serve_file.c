#include "serve_file.h"
#include "response.h"
#include "utils/utils.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>

int dir_has_index_html(char *path)
{
    DIR *dir = opendir(path);
    struct dirent *entry;
    if (!dir)
        return 0;
    while ((entry = readdir(dir)) != NULL)
    {
        char *name = entry->d_name;
        if (strcmp(name, "index.html") == 0)
        {
            closedir(dir);
            return 1;
        }
    }
    closedir(dir);
    return 0;
}

void serve_file(char *file_path, int client_fd, int file_fd)
{
    printf("REQ URI: %S\n", file_path);
    int response_line_status = write_response_line(client_fd, STATUS_OK);
    if (response_line_status == -1)
        return;
    char *content_type = get_file_content_type_from_uri(file_path);
    Headers headers = {0};
    add_header("Content-Type", content_type, &headers);
    add_header("Transfer-Encoding", "chunked", &headers);
    int header_status = write_headers(client_fd, headers);
    if (header_status == -1)
        return;
    char read_buffer[FILE_READ_BUFFER_SIZE] = {0};
    ssize_t read_bytes;
    while ((read_bytes = read(file_fd, read_buffer, sizeof(read_buffer))) > 0)
    {
        char chunk_size_buf[70] = {0};
        int buff_len = snprintf(chunk_size_buf, sizeof(chunk_size_buf), "%x\r\n", read_bytes);
        ssize_t written_bytes = write(client_fd, chunk_size_buf, buff_len);
        if (written_bytes < 1)
        {
            printf("Failed to write response: %ld\n", (long)written_bytes);
        }

        written_bytes = write(client_fd, read_buffer, read_bytes);
        if (written_bytes < 1)
        {
            printf("Failed to write response: %ld\n", (long)written_bytes);
        }

        written_bytes = write(client_fd, "\r\n", 2);
        if (written_bytes < 1)
        {
            printf("Failed to write response: %ld\n", (long)written_bytes);
        }
    }
    ssize_t written_bytes = write(client_fd, "0\r\n\r\n", 5);
    if (written_bytes < 1)
    {
        printf("Failed to write response: %ld\n", (long)written_bytes);
    }
}

void http_serve_file(Request *req, int client_fd)
{
    struct stat st;
    printf("File from %s\n", req->url);
    char file_path[URI_MAX_LENGTH] = {0};
    int file_path_buffer_size = sizeof(file_path);

    snprintf(file_path, file_path_buffer_size, "./web%s", req->url);
    int file_fd = open(file_path, O_RDONLY);
    if (file_fd == -1)
    {
        perror("File open error");
        int status = write_not_found(client_fd);
        return;
    }
    else
    {

        if (fstat(file_fd, &st) != 0)
        {
            perror("fstat failed");
            write_bad_req(client_fd);
            return;
        }
        if (S_ISDIR(st.st_mode))
        {
            if (dir_has_index_html(file_path) == 1)
            {
                if ((strlen(file_path) + 11) >= URI_MAX_LENGTH)
                {
                    perror("Too big url");
                    int status = write_bad_req(client_fd);
                    return;
                }
                else
                {
                    snprintf(file_path + strlen(file_path), file_path_buffer_size, "/index.html");
                    close(file_fd);
                    file_fd = open(file_path, O_RDONLY);
                    if (file_fd == -1)
                    {
                        perror("File open error");
                        int status = write_not_found(client_fd);
                        return;
                    }
                }
            }
        }
        else if (!S_ISREG(st.st_mode))
        {
            write_bad_req(client_fd);
            return;
        }
        serve_file(file_path, client_fd, file_fd);
        close(file_fd);
    }
}