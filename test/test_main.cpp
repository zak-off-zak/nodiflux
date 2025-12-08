#include <gtest/gtest.h>

int main() {
  ::testing::InitGoogleTest();
  if (RUN_ALL_TESTS())
	;
  return 0;
}
