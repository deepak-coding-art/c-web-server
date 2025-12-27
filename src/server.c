#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "../lib/request_processor.h"
#include "../lib/server.h"

// Single CB method handles the routes

// method => GET, POST
// url => /api/user
// query => ?id=3&age=4
// body => json, form-data

// match_uri(uri, "")
// match_method(method, "GET")
// get_query(query, "key", &buffer, size)
// response http_reply(c_fd, 200, "Content-Type: application/json\r\n...", "Json")

int create_server(int port)
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        perror("socket");
        return -1;
    }
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("setsockopt");
        close(server_fd);
        return -1;
    }
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;
    int bind_status = bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
    if (bind_status < 0)
    {
        perror("bind");
        return -1;
    }
    return server_fd;
}

int listen_to_server(int server_fd, CallBack fn)
{
    int listen_status = listen(server_fd, 10);
    if (listen_status < 0)
    {
        perror("listen");
        return -1;
    }
    while (1)
    {
        int client_fd = accept(server_fd, NULL, NULL);
        if (client_fd < 0)
        {
            perror("client");
            return -1;
        }
        process_request(client_fd, fn);
        shutdown(client_fd, SHUT_RDWR);
        close(client_fd);
    }

    return 0;
}

int start_server(int port, CallBack fn)
{
    int server_fd = create_server(port);
    if (server_fd < 0)
    {
        return -1;
    }
    int exit_code = listen_to_server(server_fd, fn);
    close(server_fd);
    return exit_code;
}
