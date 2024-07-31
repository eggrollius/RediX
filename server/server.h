#ifndef SERVER_H
#define SERVER_H

#include "../libshared/libshared.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <cassert>
#include <cerrno>
#include <sys/socket.h>
#include <netinet/in.h>
#include <vector>
#include <map>
#include <string>


int main();
int process_connection(int fd);
void conn_put(std::vector<Conn *> &fd_to_conn, struct Conn *conn);
int32_t accept_new_connection(std::vector<Conn*> &fd_to_conn, int fd);
void state_req(Conn *conn);
void connection_io(Conn * conn);
bool try_fill_buffer(Conn *connn);
bool try_one_request(Conn* conn) ;
void state_res(Conn *conn);
bool try_flush_buffer(Conn *conn);
int32_t process_command(uint32_t len, char *cmdstr, std::string &res_msg);
int32_t parse_cmd_from_str(char* cmdstr, std::vector<std::string> &out);
Db_return_code db_get(std::string key, std::string &res_msg);
Db_return_code db_set(std::string key, std::string value, std::string &res_msg);
Db_return_code db_del(std::string key, std::string &res_msg);

#endif // SERVER_H