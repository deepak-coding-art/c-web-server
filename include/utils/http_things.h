#ifndef HTTP_THINGS
#define HTTP_THINGS

typedef enum
{
    METHOD_GET = 0,
    METHOD_POST,
} HttpMethod;

typedef enum
{
    STATUS_OK = 200,
    STATUS_NOT_FOUND = 404,
    STATUS_ERROR = 500,
    STATUS_BAD_REQ = 400
} HttpStatus;

const char *method_to_string(HttpMethod m);
const char *code_to_reason(HttpStatus code);

#endif