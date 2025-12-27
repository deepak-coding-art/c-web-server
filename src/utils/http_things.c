#include "../../lib/utils/http_things.h"

const char *method_to_string(HttpMethod m)
{
    switch (m)
    {
    case METHOD_GET:
        return "GET";
    case METHOD_POST:
        return "POST";
    default:
        return "UNKNOWN";
    }
}

const char *code_to_reason(HttpStatus code)
{
    switch (code)
    {
    case STATUS_OK:
        return "OK";
    case STATUS_NOT_FOUND:
        return "Not Found";
    case STATUS_ERROR:
        return "Internal Server Error";
    case STATUS_BAD_REQ:
        return "Bad Request";
    default:
        return "Unknown";
    }
}