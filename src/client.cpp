#include "client.h"

#include <iostream>
#include <string>
#include <sstream>
#include <utility>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <vector>

#define MAX_MSG 4096

// Constructor
Client::Client() : socket_fd(-1), addr("127.0.0.1"), port(1234) {}

Client::Client(const std::string& addr, const int port) : socket_fd(-1), addr(addr), port(port) {}

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

    struct sockaddr_in server_addr = {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(this->port);

    // Convert the address from string to binary form
    if (inet_pton(AF_INET, this->addr.c_str(), &server_addr.sin_addr) <= 0) {
        close(this->socket_fd);
        return false;
    }

    if (connect(this->socket_fd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        close(this->socket_fd);
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

// Send a generic command to the server and get the response
std::string Client::send_command(const std::string& command) {
    std::string request = command;
    if (!send_request(request)) {
        std::cerr << "Error in Client method send_request()" << std::endl;
    }

    return read_response();
}

// Wrapper for GET command
std::string Client::get(const std::string& key) {
    return send_command("GET " + key);
}
// Wrapper for SET command
std::string Client::set(const std::string& key, const std::string& value) {
    std::string request = "SET " + key + " " + value;
    return send_command(request);
}

// Wrapper for DEL command
std::string Client::del(const std::string& key) {
    std::string request = "DEL " + key;
    return send_command(request);
}

// Wrapper for TTL command
int Client::ttl(const std::string& key) {
    std::string request = "TTL " + key;
    std::string response = send_command(request);
    try {
        return std::stoi(response); // Convert response to integer
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error converting ttl from string to int" << std::endl;
        return -3;
    }
}

// Wrapper for EXPIRE command
std::string Client::expire(const std::string& key, int seconds) {
    std::string request = "EXPIRE " + key + " " + std::to_string(seconds);
    return send_command(request);
}


std::string Client::lpush(const std::string& key, const std::string& value) {
    std::string request = "LPUSH " + key + " " + value;
    return send_command(request);
}

std::string Client::rpush(const std::string& key, const std::string& value) {
    std::string request = "RPUSH " + key + " " + value;
    return send_command(request);
}

std::string Client::lpop(const std::string& key) {
    std::string request = "LPOP " + key; 
    return send_command(request);
}

std::string Client::rpop(const std::string& key) {
    std::string request = "RPOP " + key; 
    return send_command(request);
}

std::string Client::llen(const std::string& key) {
    std::string request = "LLEN " + key;
    return send_command(request);
}

std::string Client::ZAdd(const std::string key, const std::string element_key, const double score) {
    std::string request = "ZADD " + key + " " + element_key + " " + std::to_string(score);
    return send_command(request);
}

std::string Client::ZRem(const std::string key, const std::string element_key) {
    std::string request = "ZREM " + key + " " + element_key;
    return send_command(request);
}


std::string Client::ZRangeByScore(const std::string key, const double min, const double max) {
    std::string request = "ZRANGEBYSCORE " + key + " " + std::to_string(min) + " " + std::to_string(max);
    return send_command(request);

}
