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
