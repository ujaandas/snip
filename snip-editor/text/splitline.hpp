#pragma once

#include <cstddef>
#include <string>

namespace snip::editor {

class SplitLineBuffer {
private:
  std::string left;
  std::string rightReversed;

public:
  std::size_t cursorPos = 0;

  void changeLine(const std::string &newLine);

  bool shiftRight();

  bool shiftLeft();

  void insert(char c);

  std::size_t length() const { return left.size() + rightReversed.size(); }

  std::string string() const;
};

} // namespace snip::editor