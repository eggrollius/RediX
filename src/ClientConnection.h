#ifndef CLIENTCONNECTION_H
#define CLIENTCONNECTION_H
#define MAX_MSG 4096
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
class ClientConnection {
private:
  int sock_fd;
  // buffer for reading
  uint8_t rbuf[4 + MAX_MSG];
  size_t rbuf_size; 
  // buffer for writing
  uint8_t wbuf[4 + MAX_MSG];
  size_t wbuf_size;
  size_t wbuf_sent;
  bool write_full(char *buf, ssize_t n);
public:
  ClientConnection(int sock_fd);
  bool receive_request(std::string& msg);
  void send_response(std::string &response);
};

#endif // CLIENTCONNECTION_H