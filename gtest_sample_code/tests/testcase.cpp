#include <gtest/gtest.h>

TEST(testsuit, test_add) {
  double a = 0;
  double b = 1.0f;
  double c = a + b;
  ASSERT_DOUBLE_EQ(a + b, c);
}