#ifndef LIBSHARED_H
#define LIBSHARED_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string> 
#define MAX_MSG 4096

enum {
    STATE_REQ = 0,  // mark the connection for reading
    STATE_RES = 1,  // mark the connection for writing
    STATE_END = 2,  // mark the connection for deletion
};

enum Operation_code {
    OP_GET = 0, // opcode fore get operation
    OP_SET = 1, // opcode fore set operation
    OP_DEL = 2, // opcode fore del operation
};

bool operation_is(std::string operation, Operation_code code);

enum Db_return_code {
    RES_OK = 0,
    RES_ERR = 1,
    RES_NX = 2,
};

/// @brief structure to represent a connection
struct Conn {
    int fd = -1;
    uint32_t state = 0;     // either STATE_REQ or STATE_RES
    // buffer for reading
    size_t rbuf_size = 0;
    uint8_t rbuf[4 + MAX_MSG];
    // buffer for writing
    size_t wbuf_size = 0;
    size_t wbuf_sent = 0;
    uint8_t wbuf[4 + MAX_MSG];
};

int32_t read_full(int fd, char *buf, ssize_t n);
int32_t write_full(int fd, char * buf, ssize_t n);
void fd_set_nonblocking(int fd);

#endif // LIBSHARED_H