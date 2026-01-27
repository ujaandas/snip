/* The app's internal state, passed around through program's runtime.
Create a new copy each time to ease testing.
*/

#ifndef STATE_H
#define STATE_H

struct Window {
  int height = 0;
  int width = 0;
};
struct State {
  int count = 0;
  Window window;
};

#endif // STATE_H