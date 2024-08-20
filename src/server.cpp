#include "server.h"
#include "ClientConnection.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <fcntl.h>


Server::Server(int port) : port(port) {};

bool Server::start() {
  // Opening a socket
  this->listening_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  if(this->listening_sock_fd < 0) {
      return false;
  }

  // configure the socket for TCP
  // SO_REUSEADDR allows for the reuse of local addresses and ports
  // Enabling it allows for forcibly binding to a port when it is in the TIME_WAIT state
  // Allows for quick server restarts
  int val =1;
  setsockopt(this->listening_sock_fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

  // bind socket to 0.0.0.0:1234
  struct sockaddr_in addr = {};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(this->port);
  addr.sin_addr.s_addr = ntohl(0); // wildcard address 0.0.0.0
  if (bind(this->listening_sock_fd, (const struct sockaddr *)&addr, sizeof(addr))) {
      return false;
  }

  // listen on the socket.
  // after listen(), the OS will handle all the TCP handshakes,
  // and place connections into a queue for us
  // SOMAXCONN is the max backlog size, on linux it is 128
  if (listen(this->listening_sock_fd, SOMAXCONN)) {
      return false;
  }

  // set the listen fd to non blocking
  if(!fd_set_nonblocking(this->listening_sock_fd)) {
    return false;
  }

  return true;
}

bool Server::accept_client() {
  struct sockaddr_in client_addr = {};
  socklen_t socklen = sizeof(client_addr);
  int connection_fd = accept(this->listening_sock_fd, (struct sockaddr *)&client_addr, &socklen);
  if(connection_fd < 0) {
      return false;
  }

  // Set the new connection to nonblocking
  fd_set_nonblocking(connection_fd);

  this->client_connections.push_back(new ClientConnection(connection_fd));
  return true;
}

bool Server::stop() {
    // Close all client connections
    for (ClientConnection* client : this->client_connections) {
        if (client) {
            delete client;
        }
    }
    this->client_connections.clear();

    // Close the listening socket
    if (this->listening_sock_fd >= 0) {
        close(this->listening_sock_fd);
        this->listening_sock_fd = -1;
    }

    return true;
}

bool Server::fd_set_nonblocking(const int fd) {
    errno = 0;
    int flags = fcntl(fd, F_GETFL, 0);
    if(errno){
        return false;
    }

    flags |= O_NONBLOCK;
    
    errno = 0;
    (void)fcntl(fd, F_SETFL, flags);
    if(errno) {
        return false;
    }

    return true;
}

void Server::run() {
  for(size_t i = 0; i < this->client_connections.size(); ++i) {
    ClientConnection *client = this->client_connections[i];
    std::string client_msg;
    if(client->receive_request(client_msg)) {
      std::cout << "Client_msg='" << client_msg << "'." << std::endl;

      std::string response = this->request_handler.handle_request(client_msg);
      std::cout << "response_msg='" << response << "'." << std::endl;
      client->send_response(response);
    }
    
  }

  return;
}