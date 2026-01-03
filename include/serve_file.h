#ifndef SERVE_FILE_H
#define SERVE_FILE_H
#include "request.h"

#define DEFAULT_PUBLIC_DER "../web"
#define FILE_READ_BUFFER_SIZE 128

void http_serve_dir(int client_fd, char *root);

void http_serve_file(Request *req, int client_fd);

int dir_has_index_html(char *path);

#endif