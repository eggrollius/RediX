#ifndef DATABASE_H
#define DATABASE_H

#include "../libshared/libshared.h"
#include <unordered_map>
#include <ctime>

class Database {
private:
    std::unordered_map<std::string, std::string> data;
    std::unordered_map<std::string, time_t> ttls;

    bool is_expired(const std::string&key) const;
public: 
    Db_return_code get_value(const std::string &key, std::string &res_msg) const;
    Db_return_code set_value(const std::string &key, const std::string &value, std::string &res_msg);
    Db_return_code del_entry(const std::string &key, std::string &res_msg);
    Db_return_code set_ttl(const std::string &key, const int ttl, std::string &res_msg);
    Db_return_code get_ttl(const std::string &key, std::string &res_msg) const;
};

#endif // DATABASE_H