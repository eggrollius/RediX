#ifndef STORAGEVALUE_H
#define STORAGEVALUE_H

#include <variant>
#include "LinkedList.cpp"
#include "SortedSet.cpp"
#include <string>
#include <vector>

using StorageValue = std::variant<
    std::string,
    LinkedList<std::string>*,
    SortedSet<std::string, double>*
>;

#endif // STORAGEVALUE_H