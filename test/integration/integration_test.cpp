#include <gtest/gtest.h>
#include "server.h"
#include "client.h"
#include "ResponseMessage.h"

class RediXIntegrationTest : public ::testing::Test {
protected:
    Server server;
    Client client;

    RediXIntegrationTest() : server(1234), client(1234) {}

    void SetUp() override {
        // Start the server
        server.start();
        // Connect the client to the server
        client.connect_to_server();
    }

    void TearDown() override {
        // Disconnect the client
        // client.disconnect();
        // Stop the server
        server.stop();
    }
};

TEST_F(RediXIntegrationTest, SetGetTest) {
    client.set("key", "value");

    EXPECT_EQ(client.get("key"), "value");
}

TEST_F(RediXIntegrationTest, DelTest) {
    client.set("key", "value");
    client.del("key");
    std::string expected_response = Response::ToString(ResponseMessage::NIL);

    EXPECT_EQ(client.get("key"), expected_response);
}

TEST_F(RediXIntegrationTest, TTLTest) {
    client.set("key", "value");

    int Expected_TTL = 10;
    client.expire("key", Expected_TTL);

    int TTL = client.ttl("key");

    // NOTE: We expect the TTL to be between 0-Expected_TTL
    // however this test could fail if for some reason this network
    //  request takes over ten seconds.
    EXPECT_TRUE((TTL >= 0) && (TTL <= Expected_TTL));
}

TEST_F(RediXIntegrationTest, ExpireTest) {
    client.set("key", "value");

    client.expire("key", 5);
    std::this_thread::sleep_for(std::chrono::seconds(6));

    std::string expected_response = Response::ToString(ResponseMessage::NIL);
    EXPECT_EQ(client.get("key"), expected_response);
}

TEST_F(RediXIntegrationTest, ListPushPopTest) {
    // Left Push Test
    client.lpush("list_key", "value1");
    client.lpush("list_key", "value2");
    client.lpush("list_key", "value3");

    // Verify the list length
    EXPECT_EQ(client.llen("list_key"), "3");

    // Right Push Test
    client.rpush("list_key", "value4");
    client.rpush("list_key", "value5");

    // Verify the list length
    EXPECT_EQ(client.llen("list_key"), "5");

    // Left Pop Test
    std::string left_pop_result = client.lpop("list_key");
    EXPECT_EQ(left_pop_result, "value3");

    // Verify the list length
    EXPECT_EQ(client.llen("list_key"), "4");

    // Right Pop Test
    std::string right_pop_result = client.rpop("list_key");
    EXPECT_EQ(right_pop_result, "value5");

    // Verify the list length
    EXPECT_EQ(client.llen("list_key"), "3");
}

TEST_F(RediXIntegrationTest, SortedSetTest) {
    EXPECT_EQ(client.ZAdd("sset", "key1", 0.0), "1");
    EXPECT_EQ(client.ZAdd("sset", "key2", 50.0), "1");
    EXPECT_EQ(client.ZAdd("sset", "key3", 100.0), "1");

    std::vector<std::pair<std::string, double>> expectedRange = {
      std::pair<std::string, double>("key1", 0.0),
      std::pair<std::string, double>("key2", 50.0) 
    };

    std::vector<std::pair<std::string, double>> range = client.ZRangeByScore("sset", 0.0, 50.0);

    ASSERT_EQ(expectedRange.size(), range.size());
    for(size_t i = 0; i < expectedRange.size(); ++i) {
        EXPECT_EQ(expectedRange[i].first, range[i].first);
        EXPECT_EQ(expectedRange[i].second, range[i].second);
    }
}