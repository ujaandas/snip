#pragma once

#include <string>
#include <vector>

class GapBufferedLine {
private:
  std::vector<char> buf;
  std::size_t gapStart = 0;
  std::size_t gapEnd = 0;

public:
  std::size_t cursorPos = 0;

  void changeLine(const std::string &newLine);

  bool shiftRight();

  bool shiftLeft();

  void expandGap(int amount = 10);

  void insert(char c);

  void insert(const std::string &text);

  void deleteBefore();

  std::size_t length() { return buf.size(); }

  std::string string() const { return std::string{buf.begin(), buf.end()}; };
};