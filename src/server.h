#ifndef SERVER_H
#define SERVER_H

#include "ClientConnection.h"
#include "RequestHandler.h"
#include <vector>

class Server {
private:
  int port;
  int listening_sock_fd;
  bool fd_set_nonblocking(const int fd);
  std::vector<ClientConnection*> client_connections;
  RequestHandler request_handler;
public:
  Server(int);
  bool start();
  bool stop();
  bool accept_client();
  void run();
};

#endif // SERVER_H