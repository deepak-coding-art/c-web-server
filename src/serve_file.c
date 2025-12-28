#include "serve_file.h"
#include <stdio.h>

void http_serve_file(Request *req, int clinet_fd)
{
    printf("File from %s\n", req->url);
}