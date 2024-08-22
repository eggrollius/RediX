#ifndef RESPONSE_MESSAGE_H
#define RESPONSE_MESSAGE_H

#include <string>

enum class ResponseMessage {
  OK,
  NIL,
  WRONGTYPE
};

class Response {
public:
  static std::string ToString(ResponseMessage message);
};

#endif // RESPONSE_MESSAGE_H