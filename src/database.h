#ifndef DATABASE_H
#define DATABASE_H

#include "StorageValue.h"
#include <unordered_map>
#include <ctime>
#include <thread>
#include <mutex>
#include <atomic>

class Database {
private:
    std::unordered_map<std::string, StorageValue> data;
    std::unordered_map<std::string, time_t> ttls;
    mutable std::mutex data_mutex;
    mutable std::mutex ttl_mutex;
    bool is_expired(const std::string&key) const;

    std::atomic<bool> stop_cleanup = false;
    std::thread cleanup_thread;
    void cleanup_expired();
public: 
    Database();
    ~Database();

    std::string get_value(const std::string &key) const;
    std::string set_value(const std::string &key, const std::string &value);
    std::string del_entry(const std::string &key);
    std::string set_ttl(const std::string &key, const int ttl);
    std::string get_ttl(const std::string &key) const;
};

#endif // DATABASE_H