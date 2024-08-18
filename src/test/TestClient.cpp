#include "TestClient.h"

// Simple test cases
TEST(SimpleTest, AssertionTrue) {
    EXPECT_TRUE(true);
}

TEST(SimpleTest, AssertionFalse) {
    EXPECT_FALSE(false);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
