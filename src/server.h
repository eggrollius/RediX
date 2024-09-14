#ifndef SERVER_H
#define SERVER_H

#include "ClientConnection.h"
#include "RequestHandler.h"
#include <vector>
#include <thread>
#include <atomic>

class Server {
private:
  std::string ip_addr;
  int port;
  int listening_sock_fd;
  int num_threads;
  std::vector<std::thread> threads;
  std::atomic<bool> stop_requested;
  
  std::vector<ClientConnection*> client_connections;
  RequestHandler request_handler;

  bool fd_set_nonblocking(const int fd);

  bool accept_client();
  void run();
  
public:
  Server(const std::string& ip_addr, const int port);
  bool start();
  bool stop();
};

#endif // SERVER_H