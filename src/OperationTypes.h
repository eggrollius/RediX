// OperationTypes.h
#ifndef OPERATION_TYPES_H
#define OPERATION_TYPES_H
#include <string>
enum class Operation {
    GET,
    SET,
    DEL,
    EXPIRE,
    TTL,
    LPUSH,
    RPUSH,
    LPOP,
    RPOP,
    LLEN,
    ZADD,
    ZREM,
    ZRANGEBYSCORE
};

bool operation_is(const std::string& operation_str, const Operation& operation);

#endif // OPERATION_TYPES_H
