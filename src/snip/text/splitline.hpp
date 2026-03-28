#pragma once

#include <cstddef>
#include <string>

class SplitLineBuffer {
private:
  std::string left;
  std::string rightReversed;

public:
  std::size_t cursorPos = 0;

  void changeLine(const std::string &newLine);

  bool shiftRight();

  bool shiftLeft();

  void expandGap(int amount = 10);

  void insert(char c);

  void insert(const std::string &text);

  void deleteBefore();

  std::size_t length() const { return left.size() + rightReversed.size(); }

  std::string string() const;
};