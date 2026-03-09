#ifndef STATE_H
#define STATE_H

#include "../editor/text/gapbufline.hpp"

struct Window {
  int height = 0;
  int width = 0;
};

struct Cursor {
  int line;
  // int col;
};

/*
The app's internal state, passed around through program's runtime.
Create a new copy each time to ease testing.
*/
struct State {
  Cursor cursor;
  std::vector<std::string> buffer;
  GapBufferedLine curLine;
  int scrollOffset = 0;
  int debugText;

  Window window;
};

#endif // STATE_H