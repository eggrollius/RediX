#ifndef CLIENT_H
#define CLIENT_H

#include <cstdint>

class Client {
public:
    Client();
    ~Client();

    bool connect_to_server(const char *ip, uint16_t port);
    void send_request(const char *text);
    void read_response();

private:
    int32_t send_req(const char *text);
    int32_t read_res();

    int socket_fd;
};

#endif // CLIENT_H
