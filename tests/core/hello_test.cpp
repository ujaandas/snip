#include "Hello.h"

#include <gtest/gtest.h>

TEST(Hello, Greets) {
  Hello hello;
  EXPECT_EQ(hello.greet(), "Hello from snip_core");
}
