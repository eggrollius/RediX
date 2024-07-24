#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_MSG 4096

int32_t read_full(int fd, char *buf, ssize_t n);
int32_t write_full(int fd, char * buf, ssize_t n);