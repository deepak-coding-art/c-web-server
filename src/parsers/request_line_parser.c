#include <stdio.h>
#include <string.h>
#include "parsers/request_line_parser.h"

#define DEBUG 0

int parse_request_line(char *str, int length, RequestLine *out)
{
    //  0 OK
    // -1 NULL input error
    // -2 for invalid method
    // -3 for invalid uri
    // -4 for invalid version
    if (!str)
    {
        return -1;
    }

    char *end = str + length;
    char current_element = 'm'; // m: Method, u: URI, v: Version

    int current_element_char_index = 0;
    char method[METHOD_MAX_LENGTH] = {0};
    char uri[URI_MAX_LENGTH] = {0};
    char version[VERSION_MAX_LENGTH] = {0};

    for (char *c = str; c < end; c++)
    {
        switch (current_element)
        {
        case 'm':
            if (current_element_char_index >= METHOD_MAX_LENGTH)
            {
                return -2;
            }
            if (*c == ' ')
            {
                current_element = 'u';
                method[current_element_char_index] = '\0';
                current_element_char_index = 0;
                break;
            }
            else
            {
                method[current_element_char_index] = *c;
            }
            current_element_char_index++;
            break;
        case 'u':
            if (current_element_char_index >= URI_MAX_LENGTH)
            {
                return -3;
            }
            if (*c == ' ')
            {
                current_element = 'v';
                uri[current_element_char_index] = '\0';
                current_element_char_index = 0;
                break;
            }
            else
            {
                uri[current_element_char_index] = *c;
            }
            current_element_char_index++;
            break;
        case 'v':
            if (current_element_char_index >= VERSION_MAX_LENGTH)
            {
                return -4;
            }
            if (*c == ' ')
            {
                version[current_element_char_index] = '\0';
                current_element_char_index = 0;
                break;
            }
            else
            {
                version[current_element_char_index] = *c;
            }
            current_element_char_index++;
            break;
        default:
            break;
        }
    }
    strcpy(out->method, method);
    strcpy(out->uri, uri);
    strcpy(out->version, version);

    if (DEBUG == 1)
    {
        printf("Method: %s\n", out->method);
        printf("URL: %s\n", out->uri);
        printf("Version: %s\n", out->version);
    }
    return 0;
}
