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

char *get_file_mime_type(char *ext)
{
    if (!ext)
        return "application/octet-stream";

    /* text */
    if (!strcmp(ext, "html"))
        return "text/html";
    if (!strcmp(ext, "css"))
        return "text/css";
    if (!strcmp(ext, "js"))
        return "application/javascript";
    if (!strcmp(ext, "txt"))
        return "text/plain";

    /* images */
    if (!strcmp(ext, "png"))
        return "image/png";
    if (!strcmp(ext, "jpg") || !strcmp(ext, "jpeg"))
        return "image/jpeg";
    if (!strcmp(ext, "gif"))
        return "image/gif";
    if (!strcmp(ext, "svg"))
        return "image/svg+xml";
    if (!strcmp(ext, "webp"))
        return "image/webp";
    if (!strcmp(ext, "ico"))
        return "image/x-icon";

    /* audio */
    if (!strcmp(ext, "mp3"))
        return "audio/mpeg";
    if (!strcmp(ext, "wav"))
        return "audio/wav";
    if (!strcmp(ext, "ogg"))
        return "audio/ogg";

    /* video */
    if (!strcmp(ext, "mp4"))
        return "video/mp4";
    if (!strcmp(ext, "webm"))
        return "video/webm";

    /* fonts */
    if (!strcmp(ext, "woff"))
        return "font/woff";
    if (!strcmp(ext, "woff2"))
        return "font/woff2";
    if (!strcmp(ext, "ttf"))
        return "font/ttf";

    /* misc */
    if (!strcmp(ext, "json"))
        return "application/json";
    if (!strcmp(ext, "pdf"))
        return "application/pdf";
    if (!strcmp(ext, "zip"))
        return "application/zip";

    return "application/octet-stream";
}

char *extract_file_ext(char *file_path)
{
    char *p = strrchr(file_path, '.');
    if (p)
        return p + 1;
    return p;
}

char *get_file_content_type_from_uri(char *uri)
{
    char *ext = extract_file_ext(uri);
    return get_file_mime_type(ext);
}

int ends_with_slash(char *uri)
{
    if (uri[strlen(uri) - 1] == '/')
    {
        return 1;
    }
    return 0;
}
