#ifndef SERVE_FILE_H
#define SERVE_FILE_H
#include "request.h"

#define DEFAULT_PUBLIC_DER "../web"

void http_serve_dir(int clinet_fd, char *root);

void http_serve_file(Request *req, int clinet_fd);

#endif