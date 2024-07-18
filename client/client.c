#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// network headers
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    // Make a socket
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket()");
        exit(EXIT_FAILURE);
    }

    // Set the target address:port, and connect
    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(1234);
    addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK); // 127.0.01
    if (connect(fd, (const struct sockaddr *)&addr, sizeof(addr))) {
        perror("connect()");
        exit(EXIT_FAILURE);
    }

    // Send a msg
    char msg[] = "Hello";
    write(fd, msg, strlen(msg));

    // Recieve the response
    char rbuf[64]= {};
    int rv = read(fd, rbuf, sizeof(rbuf) - 1);
    if (rv < 0) {
        perror("read()");
        exit(EXIT_FAILURE);
    }

    printf("Server says: %s\n", rbuf);
    close(fd);
}