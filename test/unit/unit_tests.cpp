#include <gtest/gtest.h>
#include "DatabaseTest.cpp"
#include "LinkedListTest.cpp"
#include "SortedSetTest.cpp"


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
