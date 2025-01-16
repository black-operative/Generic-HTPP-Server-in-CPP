#pragma once

/*** Includes ***/

#include <string>
#include <fstream>
#include <netinet/in.h>

/*** Defines ***/

constexpr auto PORT                = 3000;                                    // TCP Port 3000 will be used to host the server
constexpr auto BUFFER_SIZE         = 65536;                                   // 64 KibiBytes
constexpr auto CONTENT_HTML        = "text/html";                             // HTTP Request : content Type HTML
constexpr auto CONTENT_CSS         = "text/css";                              // HTTP Request : content Type CSS
constexpr auto CONTENT_JS          = "application/javascript";                // HTTP Request : content Type JS    
constexpr auto HTTP_STATUS_OK_200  = "HTTP/1.1 200 OK\r\n";                   // HTTP 200 OK Response Header
constexpr auto HTTP_STATUS_ERR_404 = "HTTP/1.1 404 NOT FOUND\r\n";            // HTTP 404 Not Found Response Header
constexpr auto HTTP_STATUS_ERR_405 = "HTTP/1.1 405 METHOD NOT ALLOWED\r\n";   // HTTP 405 Method Not Allowed Response Header

/*** Using directives ***/

using std::string;
using std::ifstream;

/*** Data ***/

struct HTTP_Response {
    string header;
    string content_type;
    string body;
};

class HTTP_Server {
    private:
        int                server_socket_fd = -1;
        int                client_socket_fd = -1;
        struct sockaddr_in server_addr;
        socklen_t          server_addr_len;
        HTTP_Response      response;
        ifstream           file_reader;
        string             file_path;

    public:
        int  init_server();
        int  start_server();
        void accept_client();
        void handle_client();
        int  send_response();
        int  send_file_res();
        int  not_found_res();
        int  illegal_method_res();
        void close_server();
};