/* The app's internal state, passed around through program's runtime.
Create a new copy each time to ease testing.
*/

#include <string>
#include <vector>

#ifndef STATE_H
#define STATE_H

struct Window {
  int height = 0;
  int width = 0;
};
struct State {
  std::vector<std::string> buffer;
  int cursorLine = 0;
  int scrollOffset = 0;

  Window window;
};

#endif // STATE_H