/*** Include Guards ***/

#pragma once
#ifndef SERVER
#define SERVER

/*** Includes ***/

#include "../Headers/Header.hpp"

/*** Defines ***/

#define PORT 3000   

/*** Data ***/

class HTTP_Server {
    private:
        int                server_socket_fd = -1;
        int                client_socket_fd = -1;
        struct sockaddr_in server_addr;
        socklen_t          server_addr_len;
        HTTP_Response      Response;
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

#endif