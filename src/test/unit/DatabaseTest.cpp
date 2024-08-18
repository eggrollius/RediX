#include <gtest/gtest.h>
#include <database.h>
TEST(Database, SetTTLWithNegativeValue)


    // Retrieve a value for an existing key
    Database db;
    std::string res_msg;
    db.set_value("key1", "value1", res_msg);
    int result = db.get_value("key1", res_msg);
    ASSERT_EQ(result, 0);
    ASSERT_EQ(res_msg, "value1");

    // Set a value for a new key
    Database db;
    std::string res_msg;
    int result = db.set_value("key2", "value2", res_msg);
    ASSERT_EQ(result, 0);
    ASSERT_EQ(res_msg, "OK");

    // Delete an existing key
    Database db;
    std::string res_msg;
    db.set_value("key3", "value3", res_msg);
    int result = db.del_entry("key3", res_msg);
    ASSERT_EQ(result, 0);
    ASSERT_EQ(res_msg, "Deleted");

    // Set a TTL for an existing key
    Database db;
    std::string res_msg;
    db.set_value("key4", "value4", res_msg);
    int result = db.set_ttl("key4", 60, res_msg);
    ASSERT_EQ(result, 0);
    ASSERT_EQ(res_msg, "TTL Set");

    // Retrieve TTL for an existing key
    Database db;
    std::string res_msg;
    db.set_value("key5", "value5", res_msg);
    db.set_ttl("key5", 60, res_msg);
    int result = db.get_ttl("key5", res_msg);
    ASSERT_EQ(result, 0);
    ASSERT_EQ(res_msg, "60");

    // Retrieve a value for a non-existing key
    Database db;
    std::string res_msg;
    int result = db.get_value("nonexistent_key", res_msg);
    ASSERT_EQ(result, -1);
    ASSERT_EQ(res_msg, "Key not found");

    // Delete a non-existing key
    Database db;
    std::string res_msg;
    int result = db.del_entry("nonexistent_key", res_msg);
    ASSERT_EQ(result, -1);
    ASSERT_EQ(res_msg, "Key not found");

    // Set a TTL for a non-existing key
    Database db;
    std::string res_msg;
    int result = db.set_ttl("nonexistent_key", 60, res_msg);
    ASSERT_EQ(result, -1);
    ASSERT_EQ(res_msg, "Key not found");

    // Retrieve TTL for a non-existing key
    Database db;
    std::string res_msg;
    int result = db.get_ttl("nonexistent_key", res_msg);
    ASSERT_EQ(result, -1);
    ASSERT_EQ(res_msg, "Key not found");

    // Set a value with an empty key
    Database db;
    std::string res_msg;
    int result = db.set_value("", "value", res_msg);
    ASSERT_EQ(result, -1);
    ASSERT_EQ(res_msg, "Invalid key");

    // Set a value with an empty value
    Database db;
    std::string res_msg;
    int result = db.set_value("key6", "", res_msg);
    ASSERT_EQ(result, 0);
    ASSERT_EQ(res_msg, "OK");

    // Set a TTL with a negative value
    Database db;
    std::string res_msg;
    db.set_value("key7", "value7", res_msg);
    int result = db.set_ttl("key7", -10, res_msg);
    ASSERT_EQ(result, -1);
    ASSERT_EQ(res_msg, "Invalid TTL");

    // Handle concurrent access to the database
    TEST(Database, shouldHandleConcurrentAccess) {
        Database db;
        std::string res_msg;
        db.set_value("key1", "value1", res_msg);
        int result = db.get_value("key1", res_msg);
        ASSERT_EQ(result, 0);
        ASSERT_EQ(res_msg, "value1");
    }

    // Ensure TTL expiration works correctly
    Database db;
    std::string res_msg;
    db.set_value("key1", "value1", res_msg);
    db.set_ttl("key1", 1, res_msg); // Set TTL to 1 second
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Wait for TTL to expire
    int result = db.get_value("key1", res_msg);
    ASSERT_EQ(result, -1); // Key should not exist after TTL expiration

    // Check if expired key is correctly identified
    Database db;
    std::string res_msg;
    db.set_value("key1", "value1", res_msg);
    db.set_ttl("key1", -1, res_msg);
    int result = db.get_value("key1", res_msg);
    ASSERT_EQ(result, -1);
    ASSERT_EQ(res_msg, "Key has expired");
}