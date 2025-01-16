#include <iostream>
#include <csignal>

#include "Server.hpp"

using std::cout;
using std::endl;

HTTP_Server *server = nullptr;

void exit_signal_handle(int signal) {
    if (signal == SIGINT) {
        cout << "\nServer shutting down..." << endl;
        server->close_server();
        cout << "Server shutdown complete." << endl;
        exit(EXIT_SUCCESS);
    }
}

int main() {
    std::signal(SIGINT, exit_signal_handle);

    server = new HTTP_Server();

    server->init_server();
    server->start_server();
    server->accept_client();
    server->close_server();
}