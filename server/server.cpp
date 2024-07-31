#include "server.h"
#include "../libshared/libshared.h"

#include "server.h"
#include "../libshared/libshared.h"

#include <iostream>
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
#include <iterator>

std::map<std::string, std::string> db;

int main() {
    // opening a socket
    int listening_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(listening_sock_fd < 0) {
        perror("socket() in main()");
        return -1;
    }

    // configure the socket for TCP
    // SO_REUSEADDR allows for the reuse of local addresses and ports
    // Enabling it allows for forcibly binding to a port when it is in the TIME_WAIT state
    // Allows for quick server restarts
    int val =1;
    setsockopt(listening_sock_fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

    // bind socket to 0.0.0.0:1234
    struct sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(1234);
    addr.sin_addr.s_addr = ntohl(0); // wildcard address 0.0.0.0
    if (bind(listening_sock_fd, (const struct sockaddr *)&addr, sizeof(addr))) {
        perror("bind()");
        exit(EXIT_FAILURE);
    }

    // listen on the socket.
    // after listen(), the OS will handle all the TCP handshakes,
    // and place connections into a queue for us
    // SOMAXCONN is the max backlog size, on linux it is 128
    if (listen(listening_sock_fd, SOMAXCONN)) {
        perror("listen()");
        exit(EXIT_FAILURE);
    }

    // A map of all client connections, keyed by the file descriptor
    std::vector<Conn *> fd_to_conn;

    // set the listen fd to non blocking
    fd_set_nonblocking(listening_sock_fd);

    // Below is an event loop
    std::vector<struct pollfd> poll_fds;

    while (1) {
        // empty all fds
        poll_fds.clear();
        // We put the listening socket fd in the first position
        struct pollfd poll_fd = {listening_sock_fd, POLLIN, 0};
        poll_fds.push_back(poll_fd);

        for(Conn *conn : fd_to_conn) {
            if(!conn) {
                continue;
            }
            // Create and configure a pollfd struct for this connection
            struct pollfd poll_fd = {};
            poll_fd.fd = conn->fd;
            poll_fd.events = (conn->state == STATE_REQ) ? POLLIN : POLLOUT;
            poll_fd.events = poll_fd.events | POLLERR;
            // add the pollfd struct to the vector
            poll_fds.push_back(poll_fd);
        }

        // poll for active fds
        int rv = poll(poll_fds.data(), (nfds_t)poll_fds.size(), 1000);
        if(rv < 0) {
            perror("poll() in main()");
            continue;
        }

        // process the active connections, exclude the listening socket fd (first one)
        for(size_t i = 1; i < poll_fds.size(); ++i) {
            if(poll_fds[i].revents) {
                Conn *conn = fd_to_conn[poll_fds[i].fd];
                connection_io(conn);
                if(conn->state == STATE_END) {
                    // end the connection
                    fd_to_conn[conn->fd] = NULL;
                    (void)close(conn->fd);
                    free(conn);
                }
            }
        }

        // Try to accept a new connection using the listening socket
        if(poll_fds[0].revents) {
            (void)accept_new_connection(fd_to_conn, listening_sock_fd);
        }
    }
    
    
    return 0;
}

/// @brief Adds the given Conn struct into the fd_to_conn vector using the fd as the key
/// @param fd_to_conn 
/// @param conn 
void conn_put(std::vector<Conn *> &fd_to_conn, struct Conn *conn) {
    // check if there is room for this new addition
    if(fd_to_conn.size() <= (size_t)conn->fd) {
        // resize if needed
        fd_to_conn.resize(conn->fd + 1);
    }
    // map the fd index to the connection struct
    fd_to_conn[conn->fd] = conn;
}

/// @brief Accepts a new connection, records the new connection in fd_to_conn,
/// uses the fd as a listening socket.
/// @param fd_to_conn 
/// @param fd 
/// @return -1 on error, 0 on success
int32_t accept_new_connection(std::vector<Conn*> &fd_to_conn, int fd) {
    struct sockaddr_in client_addr = {};
    socklen_t socklen = sizeof(client_addr);
    int connection_fd = accept(fd, (struct sockaddr *)&client_addr, &socklen);
    if(connection_fd < 0) {
        perror("accept() in accept_new_connection()");
        return -1;
    }

    // Set the new connection to nonblocking
    fd_set_nonblocking(connection_fd);
    // Create a conn struct for the connection
    struct Conn *conn = (struct Conn *)malloc(sizeof(struct Conn));
    if(!conn) {
        close(connection_fd);
        perror("error creating struct Conn in accept_new_connection()");
        return -1;
    }

    conn->fd = connection_fd;
    conn->state = STATE_REQ;
    conn->rbuf_size  = 0;
    conn->wbuf_size = 0;
    conn->wbuf_sent = 0;

    conn_put(fd_to_conn, conn);
    return 0;
}

void connection_io(Conn * conn) {
    if(conn->state == STATE_REQ) {
        state_req(conn);
    } else if(conn->state == STATE_RES) {
        state_res(conn);
    } else {
        assert(0); // not expected
    }
}

void state_req(Conn *conn) {
    while(try_fill_buffer(conn)) {}
}

/// @brief fills the read buffer with data
/// @param connn 
/// @return 
bool try_fill_buffer(Conn *conn) {
    // Try to fill the buffer, ensuring that it is not overflowed
    assert(conn->rbuf_size < sizeof(conn->rbuf));
    ssize_t rv = 0;
    do {
        ssize_t cap = sizeof(conn->rbuf) - conn->rbuf_size;
        rv = read(conn->fd, &conn->rbuf[conn->rbuf_size], cap);
    } while(rv < 0 && errno == EINTR); // EINTR: call was interupted by a signal - man read()

    if(rv < 0 && errno == EAGAIN) { // EAGAIN: this fd would block - man read()
        // implies empty buffer
        return false; 
    }

    if(rv < 0) {
        perror("read() error in try_fill_buffer()");
        return false;
    }

    if(rv == 0) {
        if(conn->rbuf_size > 0) {
            perror("unexpected EOF in try_fill_buffer()");
        } else {
            perror("EOF in try_fill_buffer()");
        }

        conn->state = STATE_END;
        return false;
    }

    conn->rbuf_size += (size_t)rv;
    assert(conn->rbuf_size <= sizeof(conn->rbuf));

    // Try process requests one by one
    while(try_one_request(conn)) {}
    return (conn->state == STATE_REQ);
}

// Takes the file descriptor of the connection socket
// and processes the connection
int process_connection(int fd) {
    // 4 bytes at the beggining represent a 32 bit integer,
    // this int represents the length of the msg to follow.
    char rbuf[4 + MAX_MSG + 1] = {}; // + 1 allows us to add the \0 at the end of the string

    int rv = read_full(fd, rbuf, 4);
    if (rv < 0) {
        perror("read_full()");
        return -1;
    }

    uint32_t msg_size = 0;
    memcpy(&msg_size, rbuf, 4);

    // ensure the msg is not to large
    if(msg_size > MAX_MSG) {
        perror("msg exceeds max size");
    }

    // now read the msg
    rv = read_full(fd, rbuf + 4, msg_size);
    if (rv < 0) {
        perror("read_full()");
        return -1;
    }

    // add the string terminator
    rbuf[4 + msg_size] = '\0';

    // print msg locally
    printf("Client says: %s\n", rbuf + 4);

    // send response
    char response[] = "world";
    char wbuf[4 + sizeof(response)];
    uint32_t len = (uint32_t)strlen(response);
    memcpy(wbuf, &len, 4);
    memcpy(&wbuf[4], response, len);

    return write_full(fd, wbuf, 4 + len);
}

bool try_one_request(Conn* conn) {
// try to parse a request from the buffer
    if (conn->rbuf_size < 4) {
        // not enough data in the buffer. Will retry in the next iteration
        return false;
    }
    uint32_t len = 0;
    memcpy(&len, &conn->rbuf[0], 4);
    if (len > MAX_MSG) {
        perror("too long");
        conn->state = STATE_END;
        return false;
    }
    if (4 + len > conn->rbuf_size) {
        // not enough data in the buffer. Will retry in the next iteration
        return false;
    }

    // got one request, do something with it
    std::string res_msg;
    printf("client says: %.*s\n", len, &conn->rbuf[4]);
    process_command(len, (char*)&conn->rbuf[4], res_msg);
    

    // generating echoing response
    memcpy(&conn->wbuf[0], &len, 4);
    memcpy(&conn->wbuf[4], &conn->rbuf[4], len);
    conn->wbuf_size = 4 + len;

    // remove the request from the buffer.
    size_t remain = conn->rbuf_size - 4 - len;
    if (remain) {
        memmove(conn->rbuf, &conn->rbuf[4 + len], remain);
    }
    conn->rbuf_size = remain;

    // change state
    conn->state = STATE_RES;
    state_res(conn);

    // continue the outer loop if the request was fully processed
    return (conn->state == STATE_REQ);
}

void state_res(Conn *conn) {
    while (try_flush_buffer(conn)) {}
}

bool try_flush_buffer(Conn *conn) {
    ssize_t rv = 0;
    do {
        size_t remain = conn->wbuf_size - conn->wbuf_sent;
        rv = write(conn->fd, &conn->wbuf[conn->wbuf_sent], remain);
    } while (rv < 0 && errno == EINTR);
    if (rv < 0 && errno == EAGAIN) {
        // got EAGAIN, stop.
        return false;
    }
    if (rv < 0) {
        perror("write() error");
        conn->state = STATE_END;
        return false;
    }
    conn->wbuf_sent += (size_t)rv;
    assert(conn->wbuf_sent <= conn->wbuf_size);
    if (conn->wbuf_sent == conn->wbuf_size) {
        // response was fully sent, change state back
        conn->state = STATE_REQ;
        conn->wbuf_sent = 0;
        conn->wbuf_size = 0;
        return false;
    }
    // still got some data in wbuf, could try to write again
    return true;
}

int32_t process_command(uint32_t len, char *cmdstr, std::string &res_msg) {
    std::vector<std::string> cmd; // tokenized cmd ex: ["get", "key", "value"...]
    parse_cmd_from_str(cmdstr, cmd); // tokenize the cmd str

    std::string operation = cmd[0];
    Db_return_code return_code;
    if(operation_is(operation, OP_GET) && cmd.size() == 2) {
        // do get
        return_code = db_get(cmd[1], res_msg);
    } else if(operation_is(operation, OP_SET) && cmd.size() == 3) {
        // do set
        return_code = db_set(cmd[1], cmd[2], res_msg);
    } else if(operation_is(operation, OP_DEL) && cmd.size() == 2) {
        // do del
        return_code = db_del(cmd[1], res_msg);
    } else {
       res_msg = "Unkown operation";
       return_code = RES_NX;
    }

    return return_code;
}

int32_t parse_cmd_from_str(char* cmdstr, std::vector<std::string> &out)  {
    char *cur = strtok(cmdstr, " ");
    while(cur != NULL) {
        out.push_back(cur);
        cur = strtok(NULL, " ");
    }

    return 0;
}

Db_return_code db_get(std::string key, std::string &res_msg) {
    printf("Getting %s\n", key.c_str());
    auto it = db.find(key);
    if (it != db.end()) {
        res_msg = it->second;
        return RES_OK;
    } else {
        res_msg = "Key not found";
        return RES_NX;
    }
}


Db_return_code db_set(std::string key, std::string value, std::string &res_msg) {
    printf("Setting %s to %s\n", key.c_str(), value.c_str());
    db[key] = value;
    res_msg = "Set successful";
    return RES_OK;
}

Db_return_code db_del(std::string key, std::string &res_msg) {
    printf("Deleting %s\n", key.c_str());
    auto it = db.find(key);
    if (it != db.end()) {
        db.erase(it);
        res_msg = "Delete successful";
        return RES_OK;
    } else {
        res_msg = "Key not found";
        return RES_NX;
    }
}