// OperationTypes.h
#ifndef OPERATION_TYPES_H
#define OPERATION_TYPES_H

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
    LLEN
};

#endif // OPERATION_TYPES_H
