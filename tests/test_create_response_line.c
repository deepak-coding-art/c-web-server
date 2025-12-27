#include <assert.h>
#include <string.h>
#include "../include/response.h"

void should_format_response_line_for_code()
{
    char response_line_buffer[128] = {0};
    create_response_line(STATUS_OK, response_line_buffer, sizeof(response_line_buffer));
    int code = strcmp("HTTP/1.1 200 OK\r\n", response_line_buffer);
    assert(code == 0);
}

void should_not_format_incorrect_response_line_for_code()
{
    char response_line_buffer[128] = {0};
    create_response_line(STATUS_OK, response_line_buffer, sizeof(response_line_buffer));
    assert(strcmp("HTTP/1.1 400 OK\r\n", response_line_buffer) != 0);
}

int main()
{
    should_format_response_line_for_code();
    should_not_format_incorrect_response_line_for_code();
    return 0;
}