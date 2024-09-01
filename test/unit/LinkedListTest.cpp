#include <gtest/gtest.h>
#include "LinkedList.cpp" 

template <typename T>
class LinkedListTest : public ::testing::Test {
public:
    LinkedList<T> list;
};

typedef ::testing::Types<std::string> LinkedListTypes;
TYPED_TEST_SUITE(LinkedListTest, LinkedListTypes);

TYPED_TEST(LinkedListTest, is_emptyInitially) {
    EXPECT_TRUE(this->list.is_empty());
}

TYPED_TEST(LinkedListTest, PopFromEmptyThrows) {
    EXPECT_THROW(this->list.left_pop(), std::out_of_range);
}

TYPED_TEST(LinkedListTest, LeftPushAndPop) {
    TypeParam value = TypeParam();  
    this->list.left_push(value);
    EXPECT_FALSE(this->list.is_empty());
    EXPECT_EQ(this->list.left_pop(), value);
    EXPECT_TRUE(this->list.is_empty());
}

TYPED_TEST(LinkedListTest, LeftPopFromEmptyThrows) {
    EXPECT_THROW(this->list.left_pop(), std::out_of_range);
}

TYPED_TEST(LinkedListTest, PushAndPop) {
    TypeParam value = TypeParam();  
    this->list.left_push(value);
    EXPECT_FALSE(this->list.is_empty());
    EXPECT_EQ(this->list.left_pop(), value);
    EXPECT_TRUE(this->list.is_empty());
}

// Right-side tests
TYPED_TEST(LinkedListTest, RightPushAndPop) {
    TypeParam value = TypeParam();  
    this->list.right_push(value);
    EXPECT_FALSE(this->list.is_empty());
    EXPECT_EQ(this->list.right_pop(), value);
    EXPECT_TRUE(this->list.is_empty());
}

TYPED_TEST(LinkedListTest, RightPopFromEmptyThrows) {
    EXPECT_THROW(this->list.right_pop(), std::out_of_range);
}

TYPED_TEST(LinkedListTest, PushLeftAndRight) {
    TypeParam left_value = TypeParam();  
    TypeParam right_value = TypeParam();
    this->list.left_push(left_value);
    this->list.right_push(right_value);
    EXPECT_FALSE(this->list.is_empty());
    EXPECT_EQ(this->list.left_pop(), left_value);
    EXPECT_EQ(this->list.right_pop(), right_value);
    EXPECT_TRUE(this->list.is_empty());
}
