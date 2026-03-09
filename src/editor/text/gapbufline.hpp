#include <string>
#include <vector>

#ifndef GAP_BUFFERED_LINE_H
#define GAP_BUFFERED_LINE_H

class GapBufferedLine {
private:
  std::vector<char> buf;
  int gapStart = 0;
  int gapEnd = 0;

public:
  int cursorPos = 0;

  void changeLine(const std::string &newLine);

  bool shiftRight();

  bool shiftLeft();

  void expandGap(int amount = 10);

  void insert(char c);

  void insert(const std::string &text);

  void deleteBefore();

  int length() { return buf.size(); }

  std::string string() const { return std::string{buf.begin(), buf.end()}; };
};

#endif // GAP_BUFFERED_LINE_H