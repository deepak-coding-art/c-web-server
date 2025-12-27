#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "../lib/request_processor.h"
#include "../lib/request_line_parser.h"

int handle_request_line(char *element_buffer, int current_index, RequestLine *request_line, char *reset_element_buffer, char *stop)
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
                    *stop = 1;
                }
                return 1;
            }
        }
        else
        {
            printf("Malformed request line\n");
            *stop = 1;
        }

        // Reset
        *reset_element_buffer = 1;
    }
    return 0;
}

void handle_header(char *element_buffer, int current_index, RequestLine *request_line, char *reset_element_buffer, char *stop)
{
}

void process_request(int client_fd, CallBack fn)
{
    char stop = 0;

    Request request = {0};
    RequestLine request_line = {0};
    char current_element_type = 'r'; // r: request-line, h: header, b: body
    char flag_parsing = 0;

    char element_buffer[ELEMENT_BUFFER_SIZE] = {0};
    int element_buffer_current_index = 0;

    char read_buffer[READ_BUFFER_SIZE] = {0};
    char read_bytes;
    while ((read_bytes = read(client_fd, &read_buffer, sizeof(read_buffer))) > 0)
    {
        if (element_buffer_current_index >= ELEMENT_BUFFER_SIZE)
            stop = 1;
        if (stop == 1)
            break;
        for (int i = 0; i < read_bytes; i++)
        {
            char reset_element_buffer = 0;
            element_buffer[element_buffer_current_index] = read_buffer[i];
            switch (current_element_type)
            {
            case 'r':
                int change_state = handle_request_line(element_buffer, element_buffer_current_index, &request_line, &reset_element_buffer, &stop);
                if (change_state == 1)
                {
                    strcpy(request.method, request_line.method);
                    strcpy(request.url, request_line.uri);
                    fn(&request, client_fd);
                    current_element_type = 'h';
                }
                break;
            case 'h':
                stop = 1;
                // handle_header(element_buffer, element_buffer_current_index, &request_line, &reset_element_buffer, &stop);
                break;
            case 'b':
                break;
            default:
                break;
            }
            // Element Buffer
            if (reset_element_buffer == 1)
            {
                element_buffer_current_index = 0;
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