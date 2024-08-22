#include "client.h"

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define MAX_MSG 4096

// Constructor
Client::Client() : socket_fd(-1), port(1234) {}

Client::Client(int port) : socket_fd(-1), port(port) {}

// Destructor
Client::~Client() {
    if (this->socket_fd >= 0) {
        close(this->socket_fd);
    }
}

// Connect to the server
bool Client::connect_to_server() {
    this->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->socket_fd < 0) {
        return false;
    }

    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(this->port);
    addr.sin_addr.s_addr = ntohl(0); // 0.0.0

    if (connect(this->socket_fd, (const struct sockaddr *)&addr, sizeof(addr)) < 0) {
        return false;
    }

    return true;
}

// Send a request to the server
bool Client::send_request(const std::string& req) {
    const char* text = req.c_str();
    uint32_t wlen = (uint32_t)strlen(text);
    if (wlen > MAX_MSG) {
        return false;
    }

    char wbuf[4 + MAX_MSG];
    memcpy(wbuf, &wlen, 4);  // assume little endian
    memcpy(&wbuf[4], text, wlen);

    int32_t err = write_full(this->socket_fd, wbuf, 4 + wlen);
    if (err) {
        return false;
    }

    return true;
}

// Read a response from the server
std::string Client::read_response() {
    // 4 bytes header
    char rbuf[4 + MAX_MSG + 1];
    int32_t err = read_full(this->socket_fd, rbuf, 4);
    if (err) {
        if (errno == 0) {
            // perror("EOF");
        } else {
            // perror("read() error");
        }
        return "";
    }

    uint32_t rlen;
    memcpy(&rlen, rbuf, 4);  // assume little endian
    if (rlen > MAX_MSG) {
        // perror("too long");
        return "";
    }

    // reply body
    err = read_full(this->socket_fd, &rbuf[4], rlen);
    if (err) {
        // perror("read() error");
        return "";
    }

    
    rbuf[4 + rlen] = '\0';
    return std::string(&rbuf[4]);
}

int32_t Client::read_full(int fd, char *buf, size_t n) {
    while (n > 0) {
        int rv = read(fd, buf, n);
        if(rv <= 0) {
            // perror("read() in read_full() failed");
            return -1;
        }

        n -= (size_t)rv;
        buf += rv;
    }

    return 0;
}

/// @brief writes n chars from buf into the fd, ensures all n chars are written
/// @param fd 
/// @param buf 
/// @param n 
/// @return -1 on error, 0 on success
int32_t Client::write_full(int fd, char *buf, size_t n) {
    while (n > 0) {
        int rv = write(fd, buf, n);
        if(rv <= 0) {
            // perror("write() in write_full() failed");
            return -1;
        }

        n -= (size_t) rv;
        buf += (size_t) rv;
    }

    return 0;
}