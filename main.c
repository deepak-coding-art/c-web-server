#include "server.h"

// Single CB method handles the routes

// method => GET, POST
// url => /api/user
// query => ?id=3&age=4
// headers => struct
// body => json, form-data

// match_uri(uri, "")
// match_method(method, "GET")
// get_query(query, "key", &buffer, size)
// response (200, "Content-Type: application/json\r\n...", "Json")

void call_back(Request *req, int client_fd)
{
    log_message(LOG_DEBUG, "URL: %s", req->url);
    if (match_url(req, "/api"))
    {
        http_response(client_fd, STATUS_OK, "Hello World\n");
    }
    http_serve_file(req, client_fd);
}

int main()
{
    return start_server(6025, &call_back);
}