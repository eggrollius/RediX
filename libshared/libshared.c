#include "libshared.h"

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