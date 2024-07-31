#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

// network headers
#include <sys/socket.h>
#include <netinet/in.h>

int main();
int32_t send_req(int fd, const char *text);
int32_t read_res(int fd);