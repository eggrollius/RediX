#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <sstream>
#include <utility>
#include <cstddef>
#include <cstdint>
#include <vector>

class Client {
public:
    Client();
    Client(const std::string& addr, const int port);
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
    std::string ZAdd(const std::string key, const std::string element_key, const double score);
    std::string ZRem(const std::string key, const std::string element_key);
    std::string ZRangeByScore(const std::string key, const double min, const double max);


private:
    int socket_fd;
    std::string addr;
    int port;
    int32_t read_full(int fd, char *buf, size_t n);
    int32_t write_full(int fd, char * buf, size_t n);
    bool send_request(const std::string &req);
    std::string send_command(const std::string& command);
    std::string read_response();
    //std::vector<std::pair<std::string, double>> string_to_z_vec(const std::string& str) const;
};

#endif // CLIENT_H
