#ifndef REQUEST_PROCESSOR_H
#define REQUEST_PROCESSOR_H
#include "../lib/shared_types.h"

#define READ_BUFFER_SIZE 8
#define ELEMENT_BUFFER_SIZE 1024

void process_request(int client_fd, CallBack fn);

#endif