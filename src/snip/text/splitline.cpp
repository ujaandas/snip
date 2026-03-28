#include "splitline.hpp"

namespace snip {

void SplitLineBuffer::changeLine(const std::string &newLine) {
  if (cursorPos > newLine.size()) {
    cursorPos = newLine.size();
  }

  left = newLine.substr(0, cursorPos);
  rightReversed.clear();
  rightReversed.reserve(newLine.size() - cursorPos);

  for (std::size_t i = newLine.size(); i > cursorPos; --i) {
    rightReversed.push_back(newLine[i - 1]);
  }
}

bool SplitLineBuffer::shiftRight() {
  if (rightReversed.empty()) {
    return false;
  }

  left.push_back(rightReversed.back());
  rightReversed.pop_back();
  cursorPos++;
  return true;
}

bool SplitLineBuffer::shiftLeft() {
  if (left.empty()) {
    return false;
  }

  rightReversed.push_back(left.back());
  left.pop_back();
  cursorPos--;
  return true;
}

void SplitLineBuffer::expandGap(int amount) {
  if (amount <= 0) {
    return;
  }

  left.reserve(left.size() + amount);
  rightReversed.reserve(rightReversed.size() + amount);
}

void SplitLineBuffer::insert(char c) {
  left.push_back(c);
  cursorPos++;
}

void SplitLineBuffer::insert(const std::string &text) {
  left += text;
  cursorPos += text.size();
}

void SplitLineBuffer::deleteBefore() {
  if (left.empty()) {
    return;
  }

  left.pop_back();
  cursorPos--;
}

std::string SplitLineBuffer::string() const {
  std::string out = left;
  out.reserve(length());

  for (auto it = rightReversed.rbegin(); it != rightReversed.rend(); ++it) {
    out.push_back(*it);
  }

  return out;
}

} // namespace snip