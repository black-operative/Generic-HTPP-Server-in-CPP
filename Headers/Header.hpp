/*** Include Guards***/

#pragma once
#ifndef HEADER
#define HEADER

/*** Includes c++ ***/

#include <iostream>
#include <fstream>
#include <string>

/*** Includes C ***/

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <csignal>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/*** Defines ***/

#define BUFFER_SIZE          16384                                   // 16 Kilo Bytes
#define CONTENT_HTML         "text/html"                             // HTTP Request : content Type HTML
#define CONTENT_CSS          "text/css"                              // HTTP Request : content Type CSS    
#define HTTP_STATUS_OK_200   "HTTP/1.1 200 OK\r\n"                   // HTTP 200 OK Message Response
#define HTTP_STATUS_ERR_404  "HTTP/1.1 404 NOT FOUND\r\n"            // HTTP 404 Not Found Message Response
#define HTTP_STATUS_ERR_405  "HTTP/1.1 405 METHOD NOT ALLOWED\r\n"   // HTTP 405 Method Not Allowed

/*** Using directives ***/

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::string;
using std::ifstream;

/*** Data ***/

typedef struct {
    string header;
    string content_type;
    string body;
} HTTP_Response;

#endif