#include "server.h"
#include "../libshared/libshared.h"

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
int process_connection(int fd) {
    // 4 bytes at the beggining represent a 32 bit integer,
    // this int represents the length of the msg to follow.
    char rbuf[4 + MAX_MSG + 1] = {}; // + 1 allows us to add the \0 at the end of the string

    int rv = read_full(fd, rbuf, 4);
    if (rv < 0) {
        perror("read_full()");
        return -1;
    }

    uint32_t msg_size = 0;
    memcpy(&msg_size, rbuf, 4);

    // ensure the msg is not to large
    if(msg_size > MAX_MSG) {
        perror("msg exceeds max size");
    }

    // now read the msg
    rv = read_full(fd, rbuf + 4, msg_size);
    if (rv < 0) {
        perror("read_full()");
        return -1;
    }

    // add the string terminator
    rbuf[4 + msg_size] = '\0';

    // print msg locally
    printf("Client says: %s\n", rbuf + 4);

    // send response
    char response[] = "world";
    char wbuf[4 + sizeof(response)];
    uint32_t len = (uint32_t)strlen(response);
    memcpy(&wbuf, &len, sizeof(len));
    memcpy(&wbuf[4], response, len);

    return write_full(fd, wbuf, 4 + len);
}
