#include "RequestHandler.h"
#include "OperationTypes.h"
#include <string>
#include <vector>
#include <string>
#include <sstream>

// TODO: This needs to be more elegant, this is ugly
// consider using an unorder_map to map strings to operations
// consider creating a command class to delegate some of this work to
std::string RequestHandler::handle_request(const std::string &cmd_str) {
  std::vector<std::string> cmd;
  parse_cmd_from_str(cmd_str, cmd);

  std::string res_msg;

  std::string operation = cmd[0];
  if (operation_is(operation, Operation::GET) && cmd.size() == 2) {
    res_msg = this->database.get_value(cmd[1]);
  } else if (operation_is(operation, Operation::SET) && cmd.size() == 3) {
    res_msg = this->database.set_value(cmd[1], cmd[2]);
  } else if (operation_is(operation, Operation::DEL) && cmd.size() == 2) {
    res_msg = this->database.del_entry(cmd[1]);
  } else if (operation_is(operation, Operation::EXPIRE) && cmd.size() == 3) {
    res_msg = this->database.set_ttl(cmd[1], std::stoi(cmd[2]));
  } else if (operation_is(operation, Operation::TTL) && cmd.size() == 2) {
    res_msg = this->database.get_ttl(cmd[1]);
  } else if (operation_is(operation, Operation::LPUSH) && cmd.size() == 3) {
    res_msg = this->database.left_push(cmd[1], cmd[2]);
  } else if (operation_is(operation, Operation::RPUSH) && cmd.size() == 3) {
    res_msg = this->database.right_push(cmd[1], cmd[2]);
  } else if (operation_is(operation, Operation::LPOP) && cmd.size() == 2) {
      res_msg = this->database.left_pop(cmd[1]);
  } else if (operation_is(operation, Operation::RPOP) && cmd.size() == 2) {
      res_msg = this->database.right_pop(cmd[1]);
  } else if (operation_is(operation, Operation::LLEN) && cmd.size() == 2) {
    res_msg = this->database.list_length(cmd[1]);
  } else {
    res_msg = "Unknown operation";
  }


  return res_msg;
}

void RequestHandler::parse_cmd_from_str(const std::string &cmd_str, std::vector<std::string> &cmd) const {
    std::istringstream stream(cmd_str); // Create a string stream from the input string
    std::string token; // A string to hold each token as it’s extracted

    while (stream >> token) { // Extract tokens separated by whitespace
      cmd.push_back(token); // Add the token to the output vector
    }

    return; 
}

bool RequestHandler::operation_is(const std::string operation_str, const Operation operation) const {
  switch (operation) {
    case Operation::GET:
      return operation_str == "GET";
    case Operation::SET:
      return operation_str == "SET";
    case Operation::DEL:
      return operation_str == "DEL";
    case Operation::EXPIRE:
      return operation_str == "EXPIRE";
    case Operation::TTL:
      return operation_str == "TTL";
    case Operation::LPUSH:
      return operation_str =="LPUSH";
    case Operation::RPUSH:
      return operation_str == "RPUSH";
    case Operation::LPOP:
      return operation_str == "LPOP";
    case Operation::RPOP:
      return operation_str == "RPOP";
    case Operation::LLEN:
      return operation_str == "LLEN";
    default:
      return false;
  }
}
