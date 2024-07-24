#include "client.h"
#include "../libshared/libshared.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

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

    // multiple requests
    int32_t err = query(fd, "hello1");
    if (err) {
        goto L_DONE;
    }
    err = query(fd, "hello2");
    if (err) {
        goto L_DONE;
    }
    err = query(fd, "hello3");
    if (err) {
        goto L_DONE;
    }

L_DONE:
    close(fd);
    return 0;
}

int32_t query(int fd, const char *text) {
    uint32_t wlen = (uint32_t)strlen(text);
    if (wlen > MAX_MSG) {
        return -1;
    }

    char wbuf[4 + MAX_MSG];
    memcpy(wbuf, &wlen, 4);  // assume little endian
    memcpy(&wbuf[4], text, wlen);

    int32_t err = write_full(fd, wbuf, 4 + wlen);
    if (err) {
        return err;
    }

    // 4 bytes header
    char rbuf[4 + MAX_MSG + 1];
    int errno = 0;
    err = read_full(fd, rbuf, 4);
    if (err) {
        if (errno == 0) {
            perror("EOF");
        } else {
            perror("read() error");
        }
        return err;
    }

    uint32_t rlen;
    memcpy(&rlen, rbuf, 4);  // assume little endian
    if (rlen > MAX_MSG) {
        perror("too long");
        return -1;
    }

    // reply body
    err = read_full(fd, &rbuf[4], rlen);
    if (err) {
        perror("read() error");
        return err;
    }

    // do something
    rbuf[4 + rlen] = '\0';
    printf("server says: %s\n", &rbuf[4]);
    return 0;
}
