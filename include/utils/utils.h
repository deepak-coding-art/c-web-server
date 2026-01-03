#ifndef UTILS_H
#define UTILS_H
#include "request.h"

int match_url(Request *req, char *url);

int match_method(Request *req, char *method);

int is_safe_uri(char *path);

char *get_file_mime_type(char *ext);
char *extract_file_ext(char *file_path);
char *get_file_content_type_from_uri(char *uri);
int ends_with_slash(char *uri);

#endif