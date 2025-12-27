#ifndef UTILS_H
#define UTILS_H
#include "../lib/shared_types.h"

int match_url(Request *req, char *url);

int match_method(Request *req, char *method);

#endif