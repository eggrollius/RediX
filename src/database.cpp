#include "database.h"
#include "ResponseMessage.h"
#include <iostream>
#include <mutex>

// TODO: Make this a macro???
bool Database::is_expired(const std::string &key) const {
    bool expired = this->ttls.contains(key) && this->ttls.at(key) <= time(0);
    return expired;
}

std::string Database::set_value(const std::string &key, const std::string &value) {
    std::lock_guard<std::mutex> lock(data_mutex);
    // Remove all TTL entries for this key
    auto ttl_entry = this->ttls.find(key);
    if (ttl_entry != this->ttls.end()) {
        this->ttls.erase(ttl_entry);
    }

    // Set the new value
    this->data[key] = value;

    return Response::ToString(ResponseMessage::OK);
}

std::string Database::get_value(const std::string &key) const {
    std::lock_guard<std::mutex> lock(data_mutex);
    if (this->data.contains(key) && !this->is_expired(key)) {
        StorageValue data_value = this->data.at(key);
        // Check that it is a string
        if(!holds_alternative<std::string>(data_value)) {
            return Response::ToString(ResponseMessage::WRONGTYPE);
        }
        
        return std::get<std::string>(data_value);
    }
    
    return Response::ToString(ResponseMessage::NIL);

}

std::string Database::del_entry(const std::string &key) {
    std::lock_guard<std::mutex> lock(data_mutex);
    int num_deleted = 0;
    if (this->data.contains(key)) {
        this->data.erase(key);
        num_deleted += 1;
    }

    return std::to_string(num_deleted);
}

std::string Database::set_ttl(const std::string &key, const int ttl) {
    std::lock_guard<std::mutex> lock(data_mutex);
    if(!this->data.contains(key)) {
        // Key does not exist
        return Response::ToString(ResponseMessage::NIL);
    }
    if(!this->is_expired(key)) {
        // no existing ttl entry, so just add one
        this->ttls[key] = time(0) + ttl; // add ttl seconds to current time
        return Response::ToString(ResponseMessage::OK);
    } else {
        return Response::ToString(ResponseMessage::NIL);
    }
}

std::string Database::get_ttl(const std::string &key) const {
    std::lock_guard<std::mutex> lock(data_mutex);
    if (this->data.contains(key) && !is_expired(key)) {
        // The key exists
        if(this->ttls.contains(key)) {
            // There is an associated TTL
            return std::to_string(this->ttls.at(key) - time(0));
        } else {
            // There is no associated TTL
            return std::to_string(-1);
        }
    }
    // The key does not exist
    return std::to_string(-2);
}