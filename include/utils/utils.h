#ifndef UTILS_H
#define UTILS_H
#include "request.h"

int match_url(Request *req, char *url);

int match_method(Request *req, char *method);

int is_safe_uri(char *path);

#endif