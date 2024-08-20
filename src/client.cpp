#include "client.h"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

// Constructor
Client::Client() : socket_fd(-1) {}

// Destructor
Client::~Client() {
    if (socket_fd >= 0) {
        close(socket_fd);
    }
}

// Connect to the server
bool Client::connect_to_server(const char *ip, uint16_t port) {
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_fd < 0) {
        perror("socket()");
        return false;
    }

    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(port);
    addr.sin_addr.s_addr = inet_addr(ip);

    if (connect(socket_fd, (const struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("connect()");
        return false;
    }

    return true;
}

// Send a request to the server
void Client::send_request(const char *text) {
    if (send_req(text) < 0) {
        std::cerr << "Failed to send request: " << text << std::endl;
    }
}

// Read a response from the server
void Client::read_response() {
    if (read_res() < 0) {
        std::cerr << "Failed to read response from server" << std::endl;
    }
}

// Private function to send request
int32_t Client::send_req(const char *text) {
    uint32_t wlen = (uint32_t)strlen(text);
    if (wlen > MAX_MSG) 
