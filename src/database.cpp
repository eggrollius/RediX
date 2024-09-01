#include "database.h"
#include "ResponseMessage.h"
#include <iostream>
#include <ctime>
#include <mutex>
#include <random>

Database::Database() {
    this->cleanup_thread = std::thread(&Database::cleanup_expired, this);
}

Database::~Database() {
    this->stop_cleanup = true;
    if (this->cleanup_thread.joinable()) {
        this->cleanup_thread.join();
    }
}

void Database::cleanup_expired() {
    // Random number generator setup
    std::random_device rd;  // Seed for random number generator
    std::mt19937 generator(rd());  // Mersenne Twister random number generator
    
    while (!stop_cleanup) {
        {
            // Lock and perform cleanup
            std::lock_guard<std::mutex> data_lock(data_mutex);
            std::lock_guard<std::mutex> ttl_lock(ttl_mutex);

            // Create a uniform distribution for random sampling
            std::uniform_int_distribution<> distribution(0, ttls.size() - 1);

            time_t now = std::time(nullptr); // current time
            std::vector<int> indices(ttls.size()/10);
            for(int i = 0; i < static_cast<int>(indices.size()); ++i) {
                indices[i] = distribution(generator);
            }
            
            std::vector<std::string> toDelete;
            for (int i = 0; i < static_cast<int>(indices.size()); ++i) { // Sample up to 100 items
                int index = indices[i];
                auto it = std::next(this->ttls.begin(), index); // Access the element by index
                if (now > it->second) {
                    // expired
                    toDelete.push_back(it->first);
                }
            }

            for (int i = 0; i < static_cast<int>(toDelete.size()); ++i) {
                this->ttls.erase(toDelete[i]);
                this->data.erase(toDelete[i]);
            }
        } // Release locks here

        // Sleep after releasing locks
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

bool Database::is_expired(const std::string &key) const {
    bool expired = this->ttls.contains(key) && this->ttls.at(key) <= time(0);
    return expired;
}

std::string Database::set_value(const std::string &key, const std::string &value) {
    std::lock_guard<std::mutex> data_lock(data_mutex);
    std::lock_guard<std::mutex> ttl_lock(ttl_mutex);
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
    std::lock_guard<std::mutex> data_lock(data_mutex);
    std::lock_guard<std::mutex> ttl_lock(ttl_mutex);
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
    std::lock_guard<std::mutex> data_lock(data_mutex);
    int num_deleted = 0;
    if (this->data.contains(key)) {
        this->data.erase(key);
        num_deleted += 1;
    }

    return std::to_string(num_deleted);
}

std::string Database::set_ttl(const std::string &key, const int ttl) {
    std::lock_guard<std::mutex> data_lock(data_mutex);
    std::lock_guard<std::mutex> ttl_lock(ttl_mutex);
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
    std::lock_guard<std::mutex> data_lock(data_mutex);
    std::lock_guard<std::mutex> ttl_lock(ttl_mutex);
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

std::string Database::left_push(const std::string& key, const std::string& value) {
    std::lock_guard<std::mutex> data_lock(data_mutex);
    bool keyExists = this->data.contains(key);
    // Check that the is not another data type
    if(keyExists && !holds_alternative<LinkedList<std::string>*>(this->data[key]) ) {
        return Response::ToString(ResponseMessage::WRONGTYPE);
    }

     // If list doesnt exist create one
    if(!keyExists) {
        this->data[key] = new LinkedList<std::string>();
    }

    LinkedList<std::string>* list = std::get<LinkedList<std::string>*>(this->data[key]);
    list->left_push(value);
    return std::to_string(list->length());
}

std::string Database::right_push(const std::string& key, const std::string& value) {
    std::lock_guard<std::mutex> data_lock(data_mutex);
    bool keyExists = this->data.contains(key);

    // Check that the is not another data type
    if(keyExists && !holds_alternative<LinkedList<std::string>*>(this->data[key]) ) {
        return Response::ToString(ResponseMessage::WRONGTYPE);
    }

    // If list doesnt exist create one
    if(!keyExists) {
        this->data[key] = new LinkedList<std::string>();
    }

    LinkedList<std::string>* list = std::get<LinkedList<std::string>*>(this->data[key]);
    list->right_push(value);

    return std::to_string(list->length());
}

std::string Database::left_pop(const std::string& key) {
    std::lock_guard<std::mutex> data_lock(data_mutex);
    bool keyExists = this->data.contains(key);

    // Check that the is not another data type
    if(keyExists && !holds_alternative<LinkedList<std::string>*>(this->data[key]) ) {
        return Response::ToString(ResponseMessage::WRONGTYPE);
    }

    // If list doesnt exist return nil
    if(!keyExists) {
        return Response::ToString(ResponseMessage::NIL);
    }

    LinkedList<std::string>* list = std::get<LinkedList<std::string>*>(this->data[key]);
    return list->left_pop();
}

std::string Database::right_pop(const std::string& key) {
    std::lock_guard<std::mutex> data_lock(data_mutex);
    bool keyExists = this->data.contains(key);

    // Check that the is not another data type
    if(keyExists && !holds_alternative<LinkedList<std::string>*>(this->data[key]) ) {
        return Response::ToString(ResponseMessage::WRONGTYPE);
    }

    // If list doesnt exist return nil
    if(!keyExists) {
        return Response::ToString(ResponseMessage::NIL);
    }

    LinkedList<std::string>* list = std::get<LinkedList<std::string>*>(this->data[key]);
    return list->right_pop();
}

std::string Database::list_length(const std::string& key) const {
    std::lock_guard<std::mutex> data_lock(data_mutex);

    if(!this->data.contains(key)) {
        return Response::ToString(ResponseMessage::NIL);
    }

    // Check that the is not another data type
    if(!holds_alternative<LinkedList<std::string>*>(this->data.at(key)) ) {
        return Response::ToString(ResponseMessage::WRONGTYPE);
    }

    LinkedList<std::string>* list = std::get<LinkedList<std::string>*>(this->data.at(key));
    return std::to_string(list->length());
}
