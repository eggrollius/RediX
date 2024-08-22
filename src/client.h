#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <cstddef>
#include <cstdint>

class Client {
public:
    Client();
    Client(int port);
    ~Client();

    bool connect_to_server();
    bool send_request(const std::string &req);
    std::string read_response();

private:
    int socket_fd;
    int port;
    int32_t read_full(int fd, char *buf, size_t n);
    int32_t write_full(int fd, char * buf, size_t n);
};

#endif // CLIENT_H
