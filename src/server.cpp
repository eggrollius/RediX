#include "server.h"
#include "ClientConnection.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fcntl.h>
#include <thread>
#include <vector>
#include <cstring> // For strerror

Server::Server(int port) : port(port), listening_sock_fd(-1), num_threads(1), stop_requested(true) {}

bool Server::start() {
  // Opening a socket
  listening_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (listening_sock_fd < 0) {
    std::cerr << "Socket creation failed: " << strerror(errno) << std::endl;
    return false;
  }

  // Configure the socket for TCP
  int val = 1;
  if (setsockopt(listening_sock_fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) < 0) {
    std::cerr << "setsockopt failed: " << strerror(errno) << std::endl;
    close(listening_sock_fd);
    return false;
  }

  // Bind socket to 0.0.0.0:port
  struct sockaddr_in addr = {};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = INADDR_ANY; // wildcard address 0.0.0.0
  if (bind(listening_sock_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    std::cerr << "Bind failed: " << strerror(errno) << std::endl;
    close(listening_sock_fd);
    return false;
  }

  // Listen on the socket
  if (listen(listening_sock_fd, SOMAXCONN) < 0) {
    std::cerr << "Listen failed: " << strerror(errno) << std::endl;
    close(listening_sock_fd);
    return false;
  }

  // Set the listen fd to non-blocking
  if (!fd_set_nonblocking(listening_sock_fd)) {
    close(listening_sock_fd);
    return false;
  }

  this->stop_requested = false;

  // Start worker threads
  for (int i = 0; i < num_threads; ++i) {
    threads.push_back(std::thread(&Server::run, this));
  }

  return true;
}

bool Server::accept_client() {
  struct sockaddr_in client_addr = {};
  socklen_t socklen = sizeof(client_addr);
  int connection_fd = accept(listening_sock_fd, (struct sockaddr *)&client_addr, &socklen);
  if (connection_fd < 0) {
    return false;
  }

  // Set the new connection to non-blocking
  if (!fd_set_nonblocking(connection_fd)) {
    close(connection_fd);
    return false;
  }

  client_connections.push_back(new ClientConnection(connection_fd));
  return true;
}

bool Server::stop() {
  this->stop_requested = true;

  // Join all threads
  for (std::thread &t : threads) {
    if (t.joinable()) {
      t.join();
    }
  }

  // Close all client connections
  for (ClientConnection* client : client_connections) {
    if (client) {
      delete client;
    }
  }
  client_connections.clear();

  // Close the listening socket
  if (listening_sock_fd >= 0) {
    close(listening_sock_fd);
    listening_sock_fd = -1;
  }

  return true;
}

bool Server::fd_set_nonblocking(const int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0) {
      std::cerr << "fcntl(F_GETFL) failed: " << strerror(errno) << std::endl;
      return false;
    }

    flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) < 0) {
      std::cerr << "fcntl(F_SETFL) failed: " << strerror(errno) << std::endl;
      return false;
    }

    return true;
}

void Server::run() {
  while (!this->stop_requested) {
    this->accept_client();

    for (size_t i = 0; i < client_connections.size(); ++i) {
      ClientConnection* client = client_connections[i];
      std::string client_msg;
      if (client->receive_request(client_msg)) {
        std::string response = request_handler.handle_request(client_msg);
        client->send_response(response);
      }
    }
  }
}
