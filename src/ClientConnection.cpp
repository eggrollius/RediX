#include "ClientConnection.h"

#include <string>
#include <cstring>
#include <cassert>

ClientConnection::ClientConnection(int sock_fd) {
  this->sock_fd = sock_fd;
  this->rbuf_size = 0;
  this->wbuf_size = 0;
  this->wbuf_sent = 0;
}

// TODO: This method needs to be improved
// Infinite loops, confusing etc.
bool ClientConnection::receive_request(std::string &msg) {
  while (true) {
    // Try to fill the buffer, ensuring that it is not overflowed
    assert(this->rbuf_size < sizeof(this->rbuf));
    ssize_t rv = 0;
    do {
        ssize_t cap = sizeof(this->rbuf) - this->rbuf_size;
        rv = read(this->sock_fd, &this->rbuf[this->rbuf_size], cap);
    } while (rv < 0 && errno == EINTR); // EINTR: call was interrupted by a signal

    if (rv < 0 && errno == EAGAIN) { // EAGAIN: this fd would block
        // implies empty buffer
        return false; 
    }

    if (rv <= 0) { 
        break; // Stop if read failed or EOF reached
    }

    this->rbuf_size += (size_t)rv;
    assert(this->rbuf_size <= sizeof(this->rbuf));

    // Try process requests one by one
    while (true) {
      // Try to parse a request from the buffer
      if (this->rbuf_size < 4) {
          // not enough data in the buffer. Will retry in the next iteration
          break;
      }
      
      uint32_t len = 0;
      memcpy(&len, &this->rbuf[0], 4);
      if (len > MAX_MSG) {
          break; // Length exceeds maximum allowed message size
      }
      
      if (4 + len > this->rbuf_size) {
          // not enough data in the buffer. Will retry in the next iteration
          break;
      }

      // Properly assign the message content to msg
      msg.assign((char*)&this->rbuf[4], len);

      // Remove the request from the buffer.
      size_t remain = this->rbuf_size - 4 - len;
      if (remain) {
          memmove(this->rbuf, &this->rbuf[4 + len], remain);
      }
      this->rbuf_size = remain;

      return true; // Successfully received and parsed a message
    }
  }
  return true;
}


void ClientConnection::send_response(std::string &response) {
    const char *response_c_str = response.c_str();
    char wbuf[4 + response.length()];

    uint32_t len = (uint32_t)strlen(response_c_str);
    memcpy(wbuf, &len, 4);
    memcpy(&wbuf[4], response_c_str, len);

    write_full(wbuf, 4 + len);
}

bool ClientConnection::write_full(char *buf, ssize_t n) {
    while (n > 0) {
        int rv = write(this->sock_fd, buf, n);
        if(rv <= 0) {
            return false;
        }

        n -= (ssize_t) rv;
        buf += (ssize_t) rv;
    }

    return true;
}
