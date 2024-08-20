#include "database.h"
#include <iostream>

bool Database::is_expired(const std::string &key) const {
    std::cout << "Checking if key '" << key << "' is expired." << std::endl;
    bool expired = this->ttls.contains(key) && this->ttls.at(key) <= time(0);
    std::cout << "Key '" << key << "' expired status: " << expired << std::endl;
    return expired;
}

int Database::set_value(const std::string &key, const std::string &value, std::string &res_msg) {
    std::cout << "Setting value for key '" << key << "' to '" << value << "'." << std::endl;
    
    // Remove all TTL entries for this key
    auto ttl_entry = this->ttls.find(key);
    if (ttl_entry != this->ttls.end()) {
        std::cout << "Removing existing TTL entry for key '" << key << "'." << std::endl;
        this->ttls.erase(ttl_entry);
    }

    // Before setting, make sure that there is no key 
    // existing with value of a non-string datatype
    if(this->data.contains(key) && !this->is_expired(key)) {
        StorageValue existing_value = this->data[key];
        // Check its type
        if(!std::holds_alternative<std::string>(existing_value)) {
            res_msg = "A non-string key with that name already exists, cannot SET it to a string";
            return -1;
        }
    }

    // Set the new value
    this->data[key] = value;
    std::cout << "Value set for key '" << key << "'." << std::endl;
    res_msg = "Set operation successful";
    std::cout << "Set operation successful for key '" << key << "'." << std::endl;

    return 1;
}

int Database::get_value(const std::string &key, std::string &res_msg) const {
    std::cout << "Getting value for key '" << key << "'." << std::endl;
    if (this->data.contains(key) && !this->is_expired(key)) {
        StorageValue data_value = this->data.at(key);
        // Check that it is a string
        if(!holds_alternative<std::string>(data_value)) {
            res_msg = "ERROR: Cannot GET a non-string data type";
            return -1;
        }
        std::cout << "GET operation successful for key '" << key << "'." << std::endl;
        return 1;
    } else {
        res_msg = "GET operation unsuccessful, key not found or expired";
        std::cout << "GET operation unsuccessful for key '" << key << "'." << std::endl;
        return 0;
    }
}

int Database::del_entry(const std::string &key, std::string &res_msg) {
    std::cout << "Deleting entry for key '" << key << "'." << std::endl;
    if (this->data.contains(key)) {
        this->data.erase(key);
        res_msg = "Delete successful";
        std::cout << "Delete operation successful for key '" << key << "'." << std::endl;
        return 1;
    } else {
        res_msg = "Key not found";
        std::cout << "Delete operation unsuccessful for key '" << key << "'." << std::endl;
        return 0;
    }
}

int Database::set_ttl(const std::string &key, const int ttl, std::string &res_msg) {
    std::cout << "Setting TTL for key '" << key << "' with ttl value: " << ttl << "." << std::endl;
    if(!this->is_expired(key)) {
        // no existing ttl entry, so just add one
        this->ttls[key] = time(0) + ttl; // add ttl seconds to current time
        res_msg = "ttl set successfully";
        std::cout << "TTL set successfully for key '" << key << "'." << std::endl;
        return 0;
    } else {
        res_msg = "could not set time-to-live, key not found";
        std::cout << "TTL set operation unsuccessful for key '" << key << "'." << std::endl;
        return 0;
    }
}

int Database::get_ttl(const std::string &key, std::string &res_msg) const {
    std::cout << "Getting TTL for key '" << key << "'." << std::endl;
    if (this->data.contains(key) && !is_expired(key)) {
        // The key exists
        if(this->ttls.contains(key)) {
            // There is an associated TTL
            res_msg = std::to_string(this->ttls.at(key) - time(0));
            std::cout << "Get TTL operation successful for key '" << key << "'." << std::endl;
            return 1;
        } else {
            // There is no associated TTL
            res_msg = "The key exists but has no associated expire";
            return 0;
        }
    } else {
        // The key does not exist
        res_msg = "get ttl operation unsuccessful, key: " + key + " not found";
        std::cout << "Get TTL operation unsuccessful for key '" << key << "'." << std::endl;
        return 0;
    }
}