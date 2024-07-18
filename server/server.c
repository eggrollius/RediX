#include "server.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Networking headers
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    // opening a socket
    int listening_sock_fd = socket(AF_INET, SOCK_STREAM, 0);

    // configure the socket for TCP
    // SO_REUSEADDR allows for the reuse of local addresses and ports
    // Enabling it allows for forcibly binding to a port when it is in the TIME_WAIT state
    // Allows for quick server restarts
    int val =1;
    setsockopt(listening_sock_fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

    // bind socket to 0.0.0.0:1234
    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(1234);
    addr.sin_addr.s_addr = ntohl(0); // wildcard address 0.0.0.0
    if (bind(listening_sock_fd, (const struct sockaddr *)&addr, sizeof(addr))) {
        perror("bind()");
        exit(EXIT_FAILURE);
    }

    // listen on the socket.
    // after listen(), the OS will handle all the TCP handshakes,
    // and place connections into a queue for us
    // SOMAXCONN is the max backlog size, on linux it is 128
    if (listen(listening_sock_fd, SOMAXCONN)) {
        perror("listen()");
        exit(EXIT_FAILURE);
    }

    // Accept connections from the listening socket
    while (1) {
        struct sockaddr_in client_addr = {};
        socklen_t addrlen = sizeof(client_addr);

        int conn_sock_fd = accept(listening_sock_fd, (struct sockaddr *)&client_addr, &addrlen);
        if (conn_sock_fd < 0) {
            perror("accept() returned negative value");
            continue; // error
        }

        // do something with the connection socket
        process_connection(conn_sock_fd);

        close(conn_sock_fd);
    }
    
    
    return 1;
}

// Takes the file descriptor of the connection socket
// and processes the connection
void process_connection(int fd) {
    char rbuf[64] = {};

    ssize_t n = read(fd, rbuf, sizeof(rbuf) - 1);
    if (n < 0) {
        perror("read() error");
        return;
    }

    printf("Client says: %s\n", rbuf);

    char wbuf[] = "world";
    write(fd, wbuf, strlen(wbuf));
}