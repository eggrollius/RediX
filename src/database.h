#ifndef DATABASE_H
#define DATABASE_H

#include "StorageValue.h"
#include <unordered_map>
#include <ctime>

class Database {
private:
    std::unordered_map<std::string, StorageValue> data;
    std::unordered_map<std::string, time_t> ttls;

    bool is_expired(const std::string&key) const;
public: 
    std::string get_value(const std::string &key) const;
    std::string set_value(const std::string &key, const std::string &value);
    std::string del_entry(const std::string &key);
    std::string set_ttl(const std::string &key, const int ttl);
    std::string get_ttl(const std::string &key) const;
};

#endif // DATABASE_H