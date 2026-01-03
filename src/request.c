#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "request.h"
#include "response.h"

int handle_request_line(char *element_buffer, size_t current_index, RequestLine *request_line, char *reset_element_buffer, char *bad_request)
{
    // return 1 to change state
    if (element_buffer[current_index] == '\n')
    {
        if (current_index > 0)
        {
            if (element_buffer[current_index - 1] == '\r')
            {
                element_buffer[current_index - 1] = '\0';
                element_buffer[current_index] = '\0';
                int status = parse_request_line(element_buffer, current_index, request_line);
                if (status < 0)
                {
                    printf("Malformed request line: %d\n", status);
                    *bad_request = 1;
                }
                *reset_element_buffer = 1;
                return 1;
            }
        }
        else
        {
            printf("Malformed request line\n");
            *bad_request = 1;
        }

        // Reset
        *reset_element_buffer = 1;
    }
    return 0;
}

int handle_header(char *element_buffer, size_t current_index, Request *request, char *reset_element_buffer, char *bad_request)
{
    if (current_index < 3)
        return 0;
    if (
        element_buffer[current_index - 3] == '\r' &&
        element_buffer[current_index - 2] == '\n' &&
        element_buffer[current_index - 1] == '\r' &&
        element_buffer[current_index] == '\n')
    {
        if (current_index < 7)
        {
            *reset_element_buffer = 1;
            return 1;
        };
        element_buffer[current_index - 1] = '\0';
        int status = parse_headers(element_buffer, current_index, &(request->headers));
        if (status < 0)
        {
            printf("Malformed headers: %d\n", status);
            *bad_request = 1;
        }
        *reset_element_buffer = 1;
        return 1;
    }
    return 0;
}

void process_request(int client_fd, CallBack fn)
{
    char stop = 0;
    char bad_request = 0;

    Request request = {0};
    RequestLine request_line = {0};
    char current_element_type = 'r'; // r: request-line, h: header, b: body
    char flag_parsing = 0;

    char element_buffer[ELEMENT_BUFFER_SIZE] = {0};
    size_t element_buffer_current_index = 0;

    char read_buffer[READ_BUFFER_SIZE] = {0};
    char read_bytes;
    while ((read_bytes = read(client_fd, &read_buffer, sizeof(read_buffer))) > 0)
    {
        if (element_buffer_current_index >= ELEMENT_BUFFER_SIZE)
            bad_request = 1;
        if (bad_request == 1)
        {
            int status = write_bad_req(client_fd);
            break;
        }
        if (stop == 1)
            break;
        // write(STDOUT_FILENO, read_buffer, read_bytes);
        for (int i = 0; i < read_bytes; i++)
        {
            char reset_element_buffer = 0;
            element_buffer[element_buffer_current_index] = read_buffer[i];
            switch (current_element_type)
            {
            case 'r':
                int req_line_change_state = handle_request_line(element_buffer, element_buffer_current_index, &request_line, &reset_element_buffer, &bad_request);
                if (req_line_change_state == 1)
                {
                    strcpy(request.method, request_line.method);
                    strcpy(request.url, request_line.uri);
                    current_element_type = 'h';
                }
                break;
            case 'h':
                int header_change_state = handle_header(element_buffer, element_buffer_current_index, &request, &reset_element_buffer, &bad_request);
                if (header_change_state == 1)
                {
                    fn(&request, client_fd);
                    current_element_type = 'b';
                }
                break;
            case 'b':
                stop = 1; // {TODO} remove
                break;
            default:
                stop = 1;
                break;
            }
            // Element Buffer
            if (reset_element_buffer == 1)
            {
                element_buffer_current_index = 0;
                element_buffer[element_buffer_current_index] = '\0';
            }
            else
            {
                element_buffer_current_index++;
            }
        }
    }

    if (read_bytes == -1)
    {
        perror("Read-request");
    }
}