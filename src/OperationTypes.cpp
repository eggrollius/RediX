#include "OperationTypes.h"
#include <string>
bool operation_is(const std::string& operation_str, const Operation& operation) {
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
    case Operation::ZADD:
      return operation_str == "ZADD";
    case Operation::ZREM:
      return operation_str == "ZREM";
    case Operation::ZRANGEBYSCORE:
      return operation_str == "ZRANGEBYSCORE";
    default:
      return false;
  }
}