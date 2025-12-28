#ifndef SERVER_H
#define SERVER_H
#include "request.h"
#include "response.h"
#include "serve_file.h"
#include "utils/utils.h"

int start_server(int port, CallBack fn);

#endif