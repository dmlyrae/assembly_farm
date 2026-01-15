#include <gtest/gtest.h>

int add(int a, int b) {
    return a + b;
}

// Простой тест
TEST(AddTest, PositiveNumbers) {
    EXPECT_EQ(add(2, 3), 5);
    ASSERT_EQ(add(1, 1), 2);
}

TEST(AddTest, NegativeNumbers) {
    EXPECT_EQ(add(-1, -1), -2);
}