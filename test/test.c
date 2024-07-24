#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../libshared/libshared.h"
#include "../client/client.h"
#include "../server/server.h"

void test_read_full_write_full() {
    int fds[2];
    assert(socketpair(AF_UNIX, SOCK_STREAM, 0, fds) == 0);

    char write_buf[] = "test message";
    char read_buf[sizeof(write_buf)] = {0};

    assert(write_full(fds[0], write_buf, sizeof(write_buf)) == 0);
    assert(read_full(fds[1], read_buf, sizeof(write_buf)) == 0);

    assert(strcmp(write_buf, read_buf) == 0);

    close(fds[0]);
    close(fds[1]);
}

void test_query() {
    int fds[2];
    assert(socketpair(AF_UNIX, SOCK_STREAM, 0, fds) == 0);

    const char *msg = "hello";
    assert(query(fds[0], msg) == 0);

    char read_buf[4 + MAX_MSG + 1] = {0};
    uint32_t len;
    assert(read_full(fds[1], read_buf, 4) == 0);
    memcpy(&len, read_buf, 4);
    assert(len == strlen(msg));
    assert(read_full(fds[1], read_buf + 4, len) == 0);
    assert(strcmp(read_buf + 4, msg) == 0);

    close(fds[0]);
    close(fds[1]);
}

void test_process_connection() {
    int fds[2];
    assert(socketpair(AF_UNIX, SOCK_STREAM, 0, fds) == 0);

    const char *msg = "hello";
    uint32_t len = (uint32_t)strlen(msg);
    char write_buf[4 + MAX_MSG] = {0};
    memcpy(write_buf, &len, 4);
    memcpy(write_buf + 4, msg, len);

    assert(write_full(fds[0], write_buf, 4 + len) == 0);
    assert(process_connection(fds[1]) == 0);

    char read_buf[4 + MAX_MSG + 1] = {0};
    assert(read_full(fds[0], read_buf, 4) == 0);
    memcpy(&len, read_buf, 4);
    assert(len == 5); // length of "world"
    assert(read_full(fds[0], read_buf + 4, len) == 0);
    assert(strcmp(read_buf + 4, "world") == 0);

    close(fds[0]);
    close(fds[1]);
}

int main() {
    test_read_full_write_full();
    test_query();
    test_process_connection();

    printf("All tests passed!\n");
    return 0;
}