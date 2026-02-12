#include "../include/gapbufline.hpp"

void GapBufferedLine::changeLine(const std::string &newLine) {
  buf.assign(newLine.begin(), newLine.end());

  // Clamp cursor to line length
  if (cursorPos > buf.size()) {
    cursorPos = buf.size();
  }

  gapStart = cursorPos;
  gapEnd = cursorPos;

  // Create an initial gap at the cursor
  expandGap(16);
}

bool GapBufferedLine::shiftRight() {
  // Check if right side available
  if (gapEnd >= buf.size()) {
    return false;
  }

  // Move the first right-side character into the last gap position
  buf[gapStart] = buf[gapEnd];

  // Move the gap and cursor by 1
  gapStart++;
  gapEnd++;
  cursorPos++;

  return true;
}

bool GapBufferedLine::shiftLeft() {
  // Check if left side available
  if (gapStart == 0) {
    return false;
  }

  gapStart--;
  gapEnd--;

  // Move the last left-side character into the first gap position
  buf[gapEnd] = buf[gapStart];
  cursorPos--;

  return true;
}

void GapBufferedLine::expandGap(int amount) {
  buf.insert(buf.begin() + gapStart, amount, '\0');
  gapEnd += amount;
}

void GapBufferedLine::insert(char c) {
  if (gapStart == gapEnd) {
    expandGap(16);
  }

  buf[gapStart] = c;
  gapStart++;
  cursorPos++;
}

void GapBufferedLine::insert(const std::string &text) {
  for (char c : text) {
    insert(c);
  }
}

void GapBufferedLine::deleteBefore() {
  // Nothing to delete
  if (gapStart == 0) {
    return;
  }

  gapStart--;
  cursorPos--;
}