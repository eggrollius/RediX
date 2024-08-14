#include "client.h"
#include "../libshared/libshared.h"

#include <iostream>
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
    const char *queries[6] = {
        "SET key1 value1",
        "GET key1",
        "SET key2 value2",
        "GET key2",
        "DEL key1",
        "GET key1"
    };

    /*for (int i = 0; i < 6; ++i) {
        int32_t err = send_req(fd, queries[i]);
        if (err) {
            goto L_DONE;
        }
    }

    for (size_t i = 0; i < 6; ++i) {
        int32_t err = read_res(fd);
        if (err) {
            goto L_DONE;
        }
    }*/

    std::cout << "Testing EXPIRE" << std::endl;
    send_req(fd, "SET expiringkey helloworld");
    read_res(fd);

    send_req(fd, "EXPIRE expiringkey 5");
    read_res(fd);

    send_req(fd, "GET expiringkey"); //most likley should still exist within 3 seconds
    read_res(fd);

    send_req(fd, "TTL expiringkey"); // get the TTL of the key
    read_res(fd);

    sleep(5); // wait until it expires

    send_req(fd, "GET expiringkey");
    read_res(fd);
    goto L_DONE;

L_DONE:
    close(fd);
    return 0;
}

int32_t send_req(int fd, const char *text) {
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

    return 0;
}

int32_t read_res(int fd) {
    // 4 bytes header
    char rbuf[4 + MAX_MSG + 1];
    int32_t err = read_full(fd, rbuf, 4);
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