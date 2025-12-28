#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "parsers/header_parser.h"

// Host: example.com
// User-Agent: Mozilla/5.0
// Accept: text/html,application/json
// Accept-Encoding: gzip, deflate, br
// Accept-Language: en-US,en;q=0.9
// Authorization: Bearer <token>
// Cookie: sessionid=abc123

// Content-Type: text/html; charset=utf-8
// Content-Length: 1234
// Cache-Control: no-cache
// Date: Sun, 28 Dec 2025 12:00:00 GMT
// Server: nginx
// Set-Cookie: sessionid=abc123; Path=/; HttpOnly
// Location: https://example.com/login

void should_parse_correct_header()
{
    char header_lines[] = "Host: example.com\r\n";
    size_t length = strlen(header_lines);
    Headers headers = {0};
    int status = parse_headers(header_lines, length, &headers);
    assert(status == 0);

    assert(headers.size == 1);

    Header h0 = headers.headers[0];
    assert(strcmp("Host", h0.key) == 0);
    assert(strcmp("example.com", h0.value) == 0);
}

void should_parse_correct_headers()
{
    char header_lines[] = "Host: example.com\r\nUser-Agent: Mozilla/5.0\r\nAccept: text/html,application/json\r\nContent-Type: text/html; charset=utf-8\r\nDate: Sun, 28 Dec 2025 12:00:00 GMT\r\n";
    int length = strlen(header_lines);
    Headers headers = {0};
    int status = parse_headers(header_lines, length, &headers);
    assert(status == 0);

    assert(headers.size == 5);

    Header h0 = headers.headers[0];
    assert(strcmp("Host", h0.key) == 0);
    assert(strcmp("example.com", h0.value) == 0);

    Header h1 = headers.headers[1];
    assert(strcmp("User-Agent", h1.key) == 0);
    assert(strcmp("Mozilla/5.0", h1.value) == 0);

    Header h2 = headers.headers[2];
    assert(strcmp("Accept", h2.key) == 0);
    assert(strcmp("text/html,application/json", h2.value) == 0);

    Header h3 = headers.headers[3];
    assert(strcmp("Content-Type", h3.key) == 0);
    assert(strcmp("text/html; charset=utf-8", h3.value) == 0);

    Header h4 = headers.headers[4];
    assert(strcmp("Date", h4.key) == 0);
    assert(strcmp("Sun, 28 Dec 2025 12:00:00 GMT", h4.value) == 0);
}

void should_return_negative_on_long_header_key()
{
    char buffer[HEADER_KEY_MAX_LENGTH + 1] = {0};
    for (int i = 0; i <= HEADER_KEY_MAX_LENGTH; i++)
    {
        buffer[i] = 's';
    }
    char header_lines[HEADER_KEY_MAX_LENGTH + 25] = {0};
    snprintf(header_lines, HEADER_KEY_MAX_LENGTH + 25, "%s: application/json", buffer);
    int length = strlen(header_lines);
    Headers headers = {0};
    int status = parse_headers(header_lines, length, &headers);
    assert(status == -1);
}

void should_return_negative_on_long_header_value()
{
    char buffer[HEADER_VALUE_MAX_LENGTH + 1] = {0};
    for (int i = 0; i <= HEADER_VALUE_MAX_LENGTH; i++)
    {
        buffer[i] = 's';
    }
    char header_lines[HEADER_VALUE_MAX_LENGTH + 25] = {0};
    snprintf(header_lines, HEADER_VALUE_MAX_LENGTH + 25, "Host: %s", buffer);
    int length = strlen(header_lines);
    Headers headers = {0};
    int status = parse_headers(header_lines, length, &headers);
    assert(status == -1);
}

void should_return_negative_on_no_collan()
{
    char header_lines[] = "Host example.com\r\n";
    int length = strlen(header_lines);
    Headers headers = {0};
    int status = parse_headers(header_lines, length, &headers);
    assert(status == -1);

    char header_lines_2[] = "Host: example.com\r\nCache-Control no-cache\r\nDate: Sun, 28 Dec 2025 12:00:00 GMT\r\n";
    int length_2 = strlen(header_lines_2);
    Headers headers_2 = {0};
    int status_2 = parse_headers(header_lines_2, length_2, &headers_2);
    assert(status_2 == -1);
}

void should_return_negative_on_space_after_key()
{
    char header_lines[] = "Host :example.com\r\n";
    int length = strlen(header_lines);
    Headers headers = {0};
    int status = parse_headers(header_lines, length, &headers);
    assert(status == -1);
}

void should_return_negative_on_no_space_after_collan()
{
    char header_lines[] = "Host:example.com\r\n";
    int length = strlen(header_lines);
    Headers headers = {0};
    int status = parse_headers(header_lines, length, &headers);
    assert(status == -1);
}

void should_return_negative_on_no_new_line()
{
    char header_lines[] = "Host:example.com\r";
    int length = strlen(header_lines);
    Headers headers = {0};
    int status = parse_headers(header_lines, length, &headers);
    assert(status == -1);
}

int main()
{
    should_parse_correct_header();
    should_parse_correct_headers();
    should_return_negative_on_long_header_key();
    should_return_negative_on_long_header_value();
    should_return_negative_on_no_collan();
    should_return_negative_on_space_after_key();
    should_return_negative_on_no_space_after_collan();
    should_return_negative_on_no_new_line();
    return 0;
}