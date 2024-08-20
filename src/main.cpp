#include "server.h"
int main() {
  Server *server = new Server(1234);
  server->start();
  while(true) {
    server->accept_client();
    server->run();
  }
  server->stop();
}