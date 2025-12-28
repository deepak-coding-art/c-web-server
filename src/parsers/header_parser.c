#include "parsers/header_parser.h"

int parse_headers(char *str, size_t length, Headers *out)
{
    //  0 ok
    // -1 malformed
    out->size = 0;

    Header current_header = {0};

    char reset = 0;
    char element = 'k'; // k: key, v: value;
    int current_element_index = 0;
    char *end = str + length;
    for (char *c = str; c < end; c++)
    {
        switch (element)
        {
        case 'k':
            if (current_element_index >= HEADER_KEY_MAX_LENGTH)
                return -1;
            if (*c == ':')
            {
                if (current_header.key[current_element_index - 1] == ' ')
                {
                    return -1;
                }
                current_header.key[current_element_index] = '\0';
                reset = 1;
                element = 'v';
                break;
            }
            if (*c == '\r')
                return -1;
            if (*c == '\n')
                return -1;
            current_header.key[current_element_index] = *c;
            break;
        case 'v':
            if (current_element_index >= HEADER_VALUE_MAX_LENGTH)
                return -1;
            if (current_element_index == 0)
            {
                if (*c != ' ')
                    return -1;
                break;
            }
            if (*c == '\n')
            {
                if (current_header.value[current_element_index - 2] != '\r')
                    return -1;
                current_header.value[current_element_index - 2] = '\0';
                // Next header {TODO}
                strcpy(out->headers[out->size].key, current_header.key);
                strcpy(out->headers[out->size].value, current_header.value);
                out->size = out->size + 1;
                reset = 1;
                element = 'k';
                break;
            }
            current_header.value[current_element_index - 1] = *c;
            break;
        }
        if (reset == 1)
        {
            current_element_index = 0;
            reset = 0;
        }
        else
        {
            current_element_index++;
        }
    }

    return 0;
}