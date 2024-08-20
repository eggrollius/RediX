#ifndef STORAGEVALUE_H
#define STORAGEVALUE_H

#include <variant>
#include <string>
#include <vector>

using StorageValue = std::variant<
    std::string,
    std::vector<std::string>
>;

#endif // STORAGEVALUE_H