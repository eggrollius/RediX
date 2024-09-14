#ifndef DATABASE_H
#define DATABASE_H

#include "StorageValue.h"
#include "LinkedList.cpp"
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
    std::string z_vec_to_string(std::vector<std::pair<std::string, double>>& vec) const;
public: 
    Database();
    ~Database();

    // string related operations
    std::string get_value(const std::string &key) const;
    std::string set_value(const std::string &key, const std::string &value);
    std::string del_entry(const std::string &key);
    std::string set_ttl(const std::string &key, const int ttl);
    std::string get_ttl(const std::string &key) const;

    // list related operations
    std::string left_push(const std::string& key, const std::string& value);
    std::string right_push(const std::string& key, const std::string& value);
    std::string left_pop(const std::string& key);
    std::string right_pop(const std::string& key);
    std::string list_length(const std::string& key) const;

    // Sorted Set related operations
    std::string z_add(const std::string& key, const std::string&element_key, const double score);
    std::string z_remove(const std::string& key, const std::string& element_key);
    std::string z_range_by_score(const std::string& key, const double min, const double max) const;
};

#endif // DATABASE_H