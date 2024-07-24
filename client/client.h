#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

// network headers
#include <sys/socket.h>
#include <netinet/in.h>

int main();
int32_t query(int fd, const char *text);