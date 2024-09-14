#include <gtest/gtest.h>
#include "SortedSet.cpp"

class SortedSetTest : public ::testing::Test {
public:
    SortedSet<std::string, double> set;
};

TEST_F(SortedSetTest, AddAndContains) {
    // Test adding an element and checking if it exists
    std::string key = "test_key";
    double score = 10.0;

    this->set.add(key, score);
    EXPECT_TRUE(this->set.contains(key));

    // Adding the same element again should not change the result
    this->set.add(key, score);
    EXPECT_EQ(this->set.size(), 1);
}

TEST_F(SortedSetTest, RemoveElement) {
    // Test adding and removing an element
    std::string key = "test_key";
    double score = 10.0;

    this->set.add(key, score);
    EXPECT_TRUE(this->set.contains(key));

    // Remove the element
    this->set.remove(key);
    EXPECT_FALSE(this->set.contains(key));
}

TEST_F(SortedSetTest, RangeQuery) {
    // Test adding multiple elements and performing a range query
    std::string key1 = "key1";
    double score1 = 5.0;

    std::string key2 = "key2";
    double score2 = 15.0;

    std::string key3 = "key3";
    double score3 = 25.0;

    this->set.add(key1, score1);
    this->set.add(key2, score2);
    this->set.add(key3, score3);

    // Perform a range query between 10.0 and 20.0
    std::vector<std::pair<std::string, double>> result = this->set.range(10.0, 20.0);
    
    // Expect only key2 to be in the result
    ASSERT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].first, "key2");
    EXPECT_EQ(result[0].second, 15.0);
}

TEST_F(SortedSetTest, AddMultipleElements) {
    // Test adding multiple elements and checking their existence
    std::string key1 = "key1";
    double score1 = 5.0;

    std::string key2 = "key2";
    double score2 = 15.0;

    std::string key3 = "key3";
    double score3 = 25.0;

    this->set.add(key1, score1);
    this->set.add(key2, score2);
    this->set.add(key3, score3);

    EXPECT_TRUE(this->set.contains("key1"));
    EXPECT_TRUE(this->set.contains("key2"));
    EXPECT_TRUE(this->set.contains("key3"));
    EXPECT_EQ(this->set.size(), 3);
}

TEST_F(SortedSetTest, RemoveNonExistentElement) {
    // Test removing a non-existent element (should do nothing)
    std::string key = "non_existent_key";
    this->set.remove(key);
    EXPECT_FALSE(this->set.contains(key));
}
