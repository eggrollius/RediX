#include <gtest/gtest.h>
#include "database.h"
#include "ResponseMessage.h"
#include <string>
#include <thread>
#include <random>

#define MAX_MSG 4096

/// @brief This class is for unit testing the Database class
class DatabaseTest : public ::testing::Test {
protected:
  Database database;
  std::string default_key;
  std::string default_value;
  double default_score;

  DatabaseTest() : default_key("key"), default_value("value"), default_score(10.0) {}

  /// @brief Generates a random string for a key
  /// @return a random string
  std::string generate_random_string() {
    int length = MAX_MSG - 1;  // Set length to be less than MAX_MSG
    const std::string characters = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::string randomString;
    
    std::random_device rd;  // Seed for random number generator
    std::mt19937 generator(rd());  // Mersenne Twister random number generator
    std::uniform_int_distribution<> distribution(0, characters.size() - 1);

    for (int i = 0; i < length; ++i) {
      randomString += characters[distribution(generator)];
    }

    return randomString;
  }
};

/// @brief Set and then Get a key-value pair
TEST_F(DatabaseTest, SetGet) {
  std::string setResponse = database.set_value(default_key, default_value);
  EXPECT_EQ(setResponse, Response::ToString(ResponseMessage::OK));

  std::string result = database.get_value(default_key);
  ASSERT_EQ(result, default_value);
}

/// @brief Set and then Get a key-value pair with an empty-string value
TEST_F(DatabaseTest, SetGetEmptyStringValue) {
  std::string setResponse = database.set_value(default_key, "");
  EXPECT_EQ(setResponse, Response::ToString(ResponseMessage::OK));

  std::string result = database.get_value(default_key);
  ASSERT_EQ(result, "");
}

/// @brief Set and then Get a key-value pair with an empty-string key
TEST_F(DatabaseTest, SetGetEmptyStringKey) {
  std::string setResponse = database.set_value("", default_value);
  EXPECT_EQ(setResponse, Response::ToString(ResponseMessage::OK));

  std::string result = database.get_value("");
  ASSERT_EQ(result, default_value);
}

/// @brief Set and Get a key multiple times
TEST_F(DatabaseTest, SetGetMultipleTimes) {
  int num_of_modifications = 1000;
  for(int i = 0; i < num_of_modifications; ++i) {
    std::string key = generate_random_string();
    std::string value = generate_random_string();

    std::string setResponse = database.set_value(default_key, default_value);
    EXPECT_EQ(setResponse, Response::ToString(ResponseMessage::OK));

    std::string result = database.get_value(default_key);
    EXPECT_EQ(result, default_value);
  }
}

/// @brief  Delete a key, ensure it cannot be retrieved,
/// and that its TTL cannot be retrieved
TEST_F(DatabaseTest, DeleteAKey) {
  std::string set_result = database.set_value(default_key, default_value);
  EXPECT_EQ(set_result, Response::ToString(ResponseMessage::OK));

  std::string set_ttl_result = database.set_ttl(default_key, 60);
  EXPECT_EQ(set_ttl_result, Response::ToString(ResponseMessage::OK));

  std::string delete_result = database.del_entry(default_key);
  EXPECT_EQ(delete_result, "1");

  std::string get_result = database.get_value(default_key);
  EXPECT_EQ(get_result, Response::ToString(ResponseMessage::NIL));

  int get_ttl_result = std::stoi(database.get_ttl(default_key));
  EXPECT_EQ(get_ttl_result, -2);

}

/// @brief Delete non-existent key
TEST_F(DatabaseTest, DeleteNonExistingKey) {
  std::string delete_result = database.del_entry(default_key);
  ASSERT_EQ(delete_result, "0");
}

/// @brief Retrieve value for a key after expiration
TEST_F(DatabaseTest, RetrieveExpiredKey) {
  std::string set_result = database.set_value(default_key, default_value);
  EXPECT_EQ(set_result, Response::ToString(ResponseMessage::OK));

  std::string set_ttl_result = database.set_ttl(default_key, 2);
  EXPECT_EQ(set_ttl_result, Response::ToString(ResponseMessage::OK));

  // wait for this key to expire
  std::this_thread::sleep_for(std::chrono::seconds(3));

  std::string get_response = database.get_value(default_key);
  ASSERT_EQ(get_response, Response::ToString(ResponseMessage::NIL));
}

/// @brief Retrieve value for a key before expiration
TEST_F(DatabaseTest, RetrieveNonExpiredKey) {
  std::string set_result = database.set_value(default_key, default_value);
  EXPECT_EQ(set_result, Response::ToString(ResponseMessage::OK));

  std::string set_ttl_result = database.set_ttl(default_key, 3);
  EXPECT_EQ(set_ttl_result, Response::ToString(ResponseMessage::OK));

  // wait a second, the key should not expire in this time
  std::this_thread::sleep_for(std::chrono::seconds(1));

  std::string get_response = database.get_value(default_key);
  ASSERT_EQ(get_response, default_value);
}

/// @brief Set TTL for non-existent key
TEST_F(DatabaseTest, SetTTLForNonExistentKey) {
  std::string set_ttl_result = database.set_ttl(default_key, 60);
  EXPECT_EQ(set_ttl_result, Response::ToString(ResponseMessage::NIL));
}

/// @brief Retrieve TTL
TEST_F(DatabaseTest, RetrieveTTL) {
  std::string set_result = database.set_value(default_key, default_value);
  EXPECT_EQ(set_result, Response::ToString(ResponseMessage::OK));

  std::string set_ttl_result = database.set_ttl(default_key, 60);
  EXPECT_EQ(set_ttl_result, Response::ToString(ResponseMessage::OK));

  int get_ttl_result = std::stoi(database.get_ttl(default_key));
  EXPECT_GE(get_ttl_result, 59); // We allow a bit of range due to time passing
  EXPECT_LE(get_ttl_result, 60); 
}

  /// @brief Create a key, give it an expiration, delete it.
  /// Then create another key of the same name, verify that 
  /// this new key does not inherit the old ones traits.
  TEST_F(DatabaseTest, ReuseKeyName) {
    // Create a key with a TTL then delete
    {
      std::string set_result = database.set_value(default_key, default_value);
      ASSERT_EQ(set_result, Response::ToString(ResponseMessage::OK));

      std::string set_ttl_result = database.set_ttl(default_key, 60);
      ASSERT_EQ(set_ttl_result, Response::ToString(ResponseMessage::OK));

      std::string delete_result = database.del_entry(default_key);
      ASSERT_EQ(delete_result, "1");
    }
    // Create a key with the same name as the previous
    // and verify that it did NOT inherit the traits of the previous
    {
      std::string set_result = database.set_value(default_key, default_value);
      ASSERT_EQ(set_result, Response::ToString(ResponseMessage::OK));

      int get_ttl_result = std::stoi(database.get_ttl(default_key));
      ASSERT_EQ(get_ttl_result, -1);
    }
  }

/// @brief Perofrm many set operations concurrently using threads
/// Then check data consistency 
TEST_F(DatabaseTest, SetGetThreadSafety) {
  int num_sets = 10000;
  std::vector<std::thread> threads;

  // Concurrent set operations
  // NOTE: This strange syntax is a lambda expression(anonymous function)
  for (int i = 0; i < num_sets; ++i) {
    threads.emplace_back([this, i](){
      std::string key = "key" + std::to_string(i);
      std::string value = "value" + std::to_string(i);
      this->database.set_value(key, value);
    });
  }

  for (std::thread &thread : threads) {
    thread.join();
  }

  // Check data consistency
  for (int i = 0; i < num_sets; ++i) {
    std::string key = "key" + std::to_string(i);
    std::string expectedValue = "value" + std::to_string(i);
    std::string result = database.get_value(key);
    EXPECT_EQ(result, expectedValue);
  }
}


/// @brief Perform many delete operations concurrently using threads
/// Then check that all keys are removed
TEST_F(DatabaseTest, DelThreadSafety) {
    int num_entries = 10000;
    std::vector<std::thread> threads;

    // Setup initial data
    for (int i = 0; i < num_entries; ++i) {
        std::string key = "key" + std::to_string(i);
        std::string value = "value" + std::to_string(i);
        database.set_value(key, value);
    }

    // Concurrent delete operations
    for (int i = 0; i < num_entries; ++i) {
        threads.emplace_back([this, i]() {
            std::string key = "key" + std::to_string(i);
            this->database.del_entry(key);
        });
    }

    for (std::thread &thread : threads) {
        thread.join();
    }

    // Check that all keys are removed
    for (int i = 0; i < num_entries; ++i) {
        std::string key = "key" + std::to_string(i);
        std::string result = database.get_value(key);
        EXPECT_EQ(result, Response::ToString(ResponseMessage::NIL)); 
    }
}

/// @brief Perform many set_ttl operations concurrently using threads
/// Then check that expired keys are correctly removed
TEST_F(DatabaseTest, SetTTLAndExpireThreadSafety) {
    int num_entries = 10000;
    int num_threads = 10;
    std::vector<std::thread> threads;
    std::vector<std::string> keys;

    // Setup initial data and TTLs
    for (int i = 0; i < num_entries; ++i) {
        std::string key = "key" + std::to_string(i);
        std::string value = "value" + std::to_string(i);
        database.set_value(key, value);
        keys.push_back(key);
    }

    // Function to set TTLs for a range of keys
    auto set_ttl_for_range = [this, &keys](int start, int end) {
        for (int i = start; i < end; ++i) {
            this->database.set_ttl(keys[i], 1); // Set a short TTL
        }
    };

    // Divide work among threads
    int range_size = (num_entries + num_threads - 1) / num_threads; // Divide entries among threads
    for (int i = 0; i < num_threads; ++i) {
        int start = i * range_size;
        int end = std::min(start + range_size, num_entries);
        threads.emplace_back(set_ttl_for_range, start, end);
    }

    // Join all threads to ensure they complete
    for (std::thread &thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    // Wait for TTLs to expire
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Check that all keys are expired
    for (const std::string& key : keys) {
        std::string result = database.get_value(key);
        EXPECT_EQ(result, Response::ToString(ResponseMessage::NIL)); 
    }
}

// Testing List Functions
TEST_F(DatabaseTest, LeftPushAndPopOnList) {
  database.left_push(default_key, default_value);
  std::string result = database.left_push(default_key, default_value);
  EXPECT_EQ(result, "2");

  ASSERT_EQ(database.left_pop(default_key), default_value);
}

TEST_F(DatabaseTest, RightPushAndPopOnList) {
  database.right_push(default_key, default_value);
  std::string result = database.right_push(default_key, default_value);
  EXPECT_EQ(result, "2");

  ASSERT_EQ(database.left_pop(default_key), default_value);
}

TEST_F(DatabaseTest, ListKeyExistsButHoldsADifferentDataType) {
  database.set_value(default_key, default_value);
  std::string result = database.left_push(default_key, default_value);
  EXPECT_EQ(result, Response::ToString(ResponseMessage::WRONGTYPE));
}

TEST_F(DatabaseTest, LeftPopOnEmptyList) {
  std::string result = database.left_pop(default_key);
  ASSERT_EQ(result, Response::ToString(ResponseMessage::NIL));
}

TEST_F(DatabaseTest, RightPopOnEmptyList) {
  std::string result = database.right_pop(default_key);
  ASSERT_EQ(result, Response::ToString(ResponseMessage::NIL));
}

TEST_F(DatabaseTest, ListLengthTestLeftPopAndPush) {
  int test_size = 5;

  for (int expected_size = 1; expected_size <= test_size; ++expected_size) {
    database.left_push(default_key, default_value);
    EXPECT_EQ(database.list_length(default_key), std::to_string(expected_size));
  }

  for(int expected_size = test_size - 1; expected_size >=0; --expected_size) {
    database.left_pop(default_key);
    EXPECT_EQ(database.list_length(default_key), std::to_string(expected_size));
  }
}

TEST_F(DatabaseTest, ListLengthTestRightPopAndPush) {
  int test_size = 5;

  for (int expected_size = 1; expected_size <= test_size; ++expected_size) {
    database.right_push(default_key, default_value);
    EXPECT_EQ(database.list_length(default_key), std::to_string(expected_size));
  }

  for(int expected_size = test_size - 1; expected_size >=0; --expected_size) {
    database.right_pop(default_key);
    EXPECT_EQ(database.list_length(default_key), std::to_string(expected_size));
  }
}

TEST_F(DatabaseTest, GetLengthOfEmptyList) {
  ASSERT_EQ(database.list_length(default_key), Response::ToString(ResponseMessage::NIL));
}

TEST_F(DatabaseTest, GetlengthOfListWrongKeyType) {
  database.set_value(default_key, default_value);
  ASSERT_EQ(database.list_length(default_key), Response::ToString(ResponseMessage::WRONGTYPE));
}

TEST_F(DatabaseTest, ShouldStoreElementCorrectlyWhenAddingToSortedSet) {
  std::string result = database.z_add(default_key, default_key, default_score);
  EXPECT_EQ(result, "1");
}

TEST_F(DatabaseTest, shouldRemoveElementFromSortedSet) {
  std::string result = database.z_add(default_key, default_key, default_score);
  EXPECT_EQ(result, "1");

  result =  database.z_remove(default_key, default_key);
  EXPECT_EQ(result, "1");
}

TEST_F(DatabaseTest, shouldRetrieveCorrectRangeByScore) {
  database.z_add(default_key, "element1", 10.5);
  database.z_add(default_key, "element2", 15.3);
  database.z_add(default_key, "element3", 20.7);

  std::string result = database.z_range_by_score(default_key, 10.0, 20.0);

  EXPECT_EQ(result, "\"element1\"\"10.5\"\n\"element2\"\"15.3\""); // "element1""10.5"\n"element2""15.3"


}