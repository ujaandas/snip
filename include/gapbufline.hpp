#include <string>
#include <vector>

#ifndef GAP_BUFFERED_LINE_H
#define GAP_BUFFERED_LINE_H

class GapBufferedLine {
private:
  int cursorPos = 0;

  std::vector<char> buf;
  int gapStart = 0;
  int gapEnd = 0;

public:
  GapBufferedLine(std::string initialLine) {
    for (auto c : initialLine) {
      buf.push_back(c);
    }
  };

  void shiftRight() {
    // Check if right side available
    if (gapEnd >= buf.size()) {
      return;
    }

    // Move the first right-side character into the last gap position
    buf[gapStart] = buf[gapEnd];

    // Move the gap and cursor by 1
    gapStart++;
    gapEnd++;
    cursorPos++;
  }

  void shiftLeft() {
    // Check if left side available
    if (gapStart == 0) {
      return;
    }

    gapStart--;
    gapEnd--;

    // Move the last left-side character into the first gap position
    buf[gapEnd] = buf[gapStart];
    cursorPos--;
  }

  void expandGap(int amount = 10) {
    buf.insert(buf.begin() + gapEnd, amount, '\0');
    gapEnd += amount;
  }

  void insert(char c) {
    if (gapStart == gapEnd) {
      expandGap();
    }

    buf[gapStart] = c;
    gapStart++;
    cursorPos++;
  }

  void insert(const std::string &text) {
    for (char c : text) {
      insert(c);
    }
  }

  void deleteBefore() {
    // Nothing to delete
    if (gapStart == 0) {
      return;
    }

    gapStart--;
    cursorPos--;
  }
};

#endif // GAP_BUFFERED_LINE_H