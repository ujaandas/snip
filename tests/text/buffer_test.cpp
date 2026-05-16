#include "Buffer.h"

#include <gtest/gtest.h>

TEST(Buffer, stores_text) {
  Buffer buffer("hello world");

  EXPECT_EQ(buffer.text(), "hello world");
}

TEST(Buffer, starts_with_zero_scroll_offset) {
  Buffer buffer("hello");

  EXPECT_EQ(buffer.scrollOffset(), 0);
}

TEST(Buffer, updates_scroll_offset) {
  Buffer buffer("hello");

  buffer.setScrollOffset(42);

  EXPECT_EQ(buffer.scrollOffset(), 42);
}

TEST(Buffer, stores_empty_text) {
  Buffer buffer("");

  EXPECT_TRUE(buffer.text().isEmpty());
}

TEST(Buffer, preserves_unicode_text) {
  Buffer buffer("こんにちは世界");

  EXPECT_EQ(buffer.text(), "こんにちは世界");
}

TEST(Buffer, replacing_scroll_offset_overwrites_previous_value) {
  Buffer buffer("hello");

  buffer.setScrollOffset(10);
  buffer.setScrollOffset(25);

  EXPECT_EQ(buffer.scrollOffset(), 25);
}

TEST(Buffer, large_scroll_offsets_are_supported) {
  Buffer buffer("hello");

  constexpr qsizetype largeOffset = 100000;

  buffer.setScrollOffset(largeOffset);

  EXPECT_EQ(buffer.scrollOffset(), largeOffset);
}

TEST(Buffer, text_accessor_does_not_modify_contents) {
  Buffer buffer("immutable");

  auto text = buffer.text();

  EXPECT_EQ(text, "immutable");
  EXPECT_EQ(buffer.text(), "immutable");
}