#include "utils/utils.h"
#include <string.h>

int match_method(Request *req, char *method)
{
    if (!req->method)
    {
        return 0;
    }
    if (!method)
    {
        return 0;
    }
    if (strcmp(req->method, method) == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int match_url(Request *req, char *url)
{
    if (!req->url)
    {
        return 0;
    }
    if (!url)
    {
        return 0;
    }
    if (strcmp(req->url, url) == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int is_safe_uri(char *path)
{
    // return 1 ok 0 bad
    if (strstr(path, "..") != NULL)
        return 0;
    if (strstr(path, "%2e%2e") != NULL)
        return 0;
    if (strstr(path, "%00") != NULL)
        return 0;
    return 1;
}
