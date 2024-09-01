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

    std::string get(const std::string& key);
    std::string set(const std::string& key, const std::string& value);
    std::string del(const std::string& key);
    int ttl(const std::string& key);
    std::string expire(const std::string& key, int seconds);

    std::string lpush(const std::string& key, const std::string& value);
    std::string rpush(const std::string& key, const std::string& value);
    std::string lpop(const std::string& key);
    std::string rpop(const std::string& key);
    std::string llen(const std::string& key);

private:
    int socket_fd;
    int port;
    int32_t read_full(int fd, char *buf, size_t n);
    int32_t write_full(int fd, char * buf, size_t n);
    bool send_request(const std::string &req);
    std::string send_command(const std::string& command);
    std::string read_response();
};

#endif // CLIENT_H
