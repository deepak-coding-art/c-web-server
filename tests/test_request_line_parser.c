#include <assert.h>
#include <string.h>
#include "../lib/request_line_parser.h"

void should_parse_method()
{
    char *line = "GET / HTTP/1.1";
    RequestLine r = {0};

    int status = parse_request_line(line, strlen(line), &r);

    assert(status == 0);
    assert(strcmp(r.method, "GET") == 0);
}

void should_parse_uri()
{
    char *line = "GET /index.html HTTP/1.1";
    RequestLine r = {0};

    parse_request_line(line, strlen(line), &r);

    assert(strcmp(r.uri, "/index.html") == 0);
}

void should_parse_version()
{
    char *line = "GET / HTTP/1.1";
    RequestLine r = {0};

    parse_request_line(line, strlen(line), &r);

    assert(strcmp(r.version, "HTTP/1.1") == 0);
}

void should_fail_on_null_input()
{
    RequestLine r = {0};

    int status = parse_request_line(NULL, 0, &r);

    assert(status != 0);
}

void should_fail_on_invalid_format()
{
    char *line = "INVALID_LINE";
    RequestLine r = {0};

    int status = parse_request_line(line, strlen(line), &r);

    assert(status != 0);
}

int main()
{
    should_parse_method();
    should_parse_uri();
    should_parse_version();
    should_fail_on_null_input();
    should_fail_on_invalid_format();
    return 0;
}
