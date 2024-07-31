#include "libshared.h"

#include <stdlib.h>
#include <stdint.h>
#include <cerrno>
#include <fcntl.h>


bool operation_is(std::string operation, Operation_code op_code) {
    switch (op_code) {
        case OP_GET:
            return operation == "get";
        case OP_SET:
            return operation == "set";
        case OP_DEL:
            return operation == "del";
        default:
            return false;
    }
}

/// @brief reads n chars into buf, ensures all n chars are read
/// @param fd 
/// @param buf 
/// @param n 
/// @return returns -1 on error, 0 on success
int32_t read_full(int fd, char *buf, ssize_t n) {
    while (n > 0) {
        int rv = read(fd, buf, n);
        if(rv <= 0) {
            perror("read() in read_full() failed");
            return -1;
        }

        n -= (ssize_t)rv;
        buf += rv;
    }

    return 0;
}

/// @brief writes n chars from buf into the fd, ensures all n chars are written
/// @param fd 
/// @param buf 
/// @param n 
/// @return -1 on error, 0 on success
int32_t write_full(int fd, char *buf, ssize_t n) {
    while (n > 0) {
        int rv = write(fd, buf, n);
        if(rv <= 0) {
            perror("write() in write_full() failed");
            return -1;
        }

        n -= (ssize_t) rv;
        buf += (ssize_t) rv;
    }

    return 0;
}

/// @brief Sets the file to non blocking mode
/// @param fd 
void fd_set_nonblocking(int fd) {
    errno = 0;
    int flags = fcntl(fd, F_GETFL, 0);
    if(errno){
        perror("fnctl error in fd_set_nonblocking()");
        return;
    }

    flags |= O_NONBLOCK;
    
    errno = 0;
    (void)fcntl(fd, F_SETFL, flags);
    if(errno) {
        perror("fcntl error in fd_set_nonblocking()");
    }
}