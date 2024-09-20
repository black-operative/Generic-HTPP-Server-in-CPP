#include "../Headers/Server.hpp"
#include "../Headers/Errors.hpp"

int HTTP_Server::init_server() {
    try {
        this->server_socket_fd = socket(
            AF_INET, 
            SOCK_STREAM, 
            0
        );
        if (this->server_socket_fd < 0) { throw ERROR_CODE::SOCKET_CREATE; }
        
        this->server_addr                 = {};
        this->server_addr_len             = sizeof(server_addr);
        this->server_addr.sin_family      = AF_INET;
        this->server_addr.sin_port        = htons(PORT);
        this->server_addr.sin_addr.s_addr = INADDR_ANY;

    } catch(const ERROR_CODE& error) { 
        cerr << GET_ERROR_MESSAGE(error) << endl; 
        return -1;
    }
    return 0;
}

int HTTP_Server::start_server() {
    try {
        if (
            bind(
                this->server_socket_fd, 
                (struct sockaddr *) &this->server_addr, 
                sizeof(this->server_addr)
            ) < 0) { throw ERROR_CODE::SOCKET_BIND; }
        
        if (listen(this->server_socket_fd, 5) < 0) { throw ERROR_CODE::SOCKET_LISTEN; }
        
        cout << "Server started on port : " << PORT << endl;

    } catch(const ERROR_CODE& error) { 
        cerr << GET_ERROR_MESSAGE(error) << endl; 
        return -1;
    }
    return 0;
}

void HTTP_Server::accept_client() {
    try {
        while (true) {
            this->client_socket_fd = accept(
                this->server_socket_fd, 
                0, 
                0
            );
            if (this->client_socket_fd < 0) { throw ERROR_CODE::SOCKET_ACCEPT; }

            cout << "\nClient Connected\n";
            this->handle_client();   
            close(this->client_socket_fd);   
        }
    } catch (const ERROR_CODE& error) {
        cerr << GET_ERROR_MESSAGE(ERROR_CODE::SOCKET_ACCEPT); 
        return;
    }
    this->close_server();
}

void HTTP_Server::handle_client() {
    try {
        char buffer[BUFFER_SIZE] = {};
        int byte_count = recv(this->client_socket_fd, buffer, BUFFER_SIZE, 0);
        if (byte_count < 0) { throw ERROR_CODE::SOCKET_RECV; }

        buffer[byte_count] = '\0';
        cout << "\n------------------------------------------------------------\n" << endl;
        cout << buffer << endl;
        cout << "\n------------------------------------------------------------\n" << endl;

        char method[5]   = {};
        char path[256]   = {};
        char version[10] = {};

        sscanf(buffer, "%s %s %s", method, path, version);
        
        string s_method  = string(method);
        string s_path    = string(path);
        string s_version = string(version);

        if (s_method != "GET") { this->illegal_method_res(); }

        if (s_path == "/" || s_path == "/index.html") {
            s_path.clear();
            s_path = "../Demo-Files/index.html";
            this->file_path = s_path;
        } else if (s_path == "/style.css") {
            s_path.clear();
            s_path = "../Demo-Files/style.css";
            this->file_path = s_path;
        } else if (s_path == "/app.js") {
            s_path.clear();
            s_path = "../Demo-Files/index.js";
            this->file_path = s_path;
        }

        this->send_file_res();

    } catch (const ERROR_CODE& error) { cerr << GET_ERROR_MESSAGE(error) << endl; }
}

int HTTP_Server::send_response() {
    try {
        char response_buffer[BUFFER_SIZE] = {};

        snprintf(
            response_buffer, 
            sizeof(response_buffer), 
            "%sContent-Type: %s\r\nContent=Length: %d\r\n\r\n",
            this->response.header.c_str(),
            this->response.content_type.c_str(), 
            (int)this->response.body.size()
        );

        int byte_count_1 = send(
            this->client_socket_fd, 
            response_buffer, 
            strlen(response_buffer), 
            0
        );

        int byte_count_2 = send(
            this->client_socket_fd, 
            this->response.body.c_str(), 
            this->response.body.size(), 
            0
        );

        if (byte_count_1 < 0 || byte_count_2 < 0) { throw ERROR_CODE::SOCKET_SEND; }
        else { return 0; } 
    } catch (const ERROR_CODE& error) {
        cerr << GET_ERROR_MESSAGE(error) << endl;
        return -1;
    }
}

int HTTP_Server::send_file_res() {
    try {
        if (this->file_path.empty()) { throw ERROR_CODE::FILE_NOT_FOUND; }

        this->file_reader.open(this->file_path);
        if (!this->file_reader.is_open()) { 
            if (this->not_found_res() < 0) { throw ERROR_CODE::SERVER_NOT_FOUND_RESPONSE; }
            throw ERROR_CODE::FILE_NOT_OPEN; 
        }

        string file_content;
        string line;     
        while (getline(this->file_reader, line)) { file_content += line; }

        string file_extension = this->file_path.substr(this->file_path.find("."));
        string file_content_type;

        if      (file_extension == ".html") { file_content_type = "text/html";              } 
        else if (file_extension == ".css")  { file_content_type = "text/css";               } 
        else if (file_extension == ".js")   { file_content_type = "application/javascript"; }

        this->response              = {};
        this->response.header       = HTTP_STATUS_OK_200;
        this->response.content_type = file_content_type;
        this->response.body         = file_content;

        if (this->send_response() < 0) {
            this->file_reader.close();
            throw ERROR_CODE::SERVER_SEND_FILE_RESPONSE;
        }
        this->file_reader.close();
        return 0;
    } catch (const ERROR_CODE& error) {
        cerr << GET_ERROR_MESSAGE(error) << endl;
        return -1;
    }
}

int HTTP_Server::not_found_res() {
    try {
        this->response              = {};
        this->response.header       = HTTP_STATUS_ERR_404,
        this->response.content_type = "text/html";
        this->response.body         = "<!DOCTYPE html><html lang=\"en\"><head></head><body><h1>404 NOT FOUND</h1></body></html>";

        int status_code = this->send_response();
        if (status_code < 0) { throw ERROR_CODE::SERVER_SEND_RESPONSE; }
        return 0;
    } catch (const ERROR_CODE& error) {
        cerr << GET_ERROR_MESSAGE(error) << endl;
        return -1;
    }
}

int HTTP_Server::illegal_method_res() {
    try {
        this->response              = {};
        this->response.header       = HTTP_STATUS_ERR_405,
        this->response.content_type = "text/html";
        this->response.body         = "<!DOCTYPE html><html lang=\"en\"><head></head><body><h1>405 METHOD NOT ALLOWED</h1></body></html>";

        int status_code = this->send_response();
        if (status_code < 0) { throw ERROR_CODE::SERVER_SEND_RESPONSE; }
        return 0;
    } catch (const ERROR_CODE& error) {
        cerr << GET_ERROR_MESSAGE(error) << endl;
        return -1;
    }
    return 0;
}

void HTTP_Server::close_server() { 
    close(this->client_socket_fd);
    close(this->server_socket_fd); 
    this->server_addr = {};
    if (this->file_reader.is_open()) { this->file_reader.close(); }
}
