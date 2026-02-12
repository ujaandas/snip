/* The app's internal state, passed around through program's runtime.
Create a new copy each time to ease testing.
*/

#include "gapbufline.hpp"

#ifndef STATE_H
#define STATE_H

struct Window {
  int height = 0;
  int width = 0;
};

struct Cursor {
  int line;
  // int col;
};

struct State {
  Cursor cursor;
  std::vector<std::string> buffer;
  GapBufferedLine curLine;
  // int cursorLine = 0; // height on buffer (y-axis)
  // GapBufferedLine *curLine = &buffer[cursorLine];
  int scrollOffset = 0;
  // int cursorCol = 0; // left/rightness on buffer's current line (x-axis)
  int debugText;

  Window window;
};

#endif // STATE_H