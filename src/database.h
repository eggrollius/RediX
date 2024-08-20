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
    int get_value(const std::string &key, std::string &res_msg) const;
    int set_value(const std::string &key, const std::string &value, std::string &res_msg);
    int del_entry(const std::string &key, std::string &res_msg);
    int set_ttl(const std::string &key, const int ttl, std::string &res_msg);
    int get_ttl(const std::string &key, std::string &res_msg) const;
};

#endif // DATABASE_H