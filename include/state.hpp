#ifndef STATE_H
#define STATE_H

/* The app's internal state, passed around through program's runtime.
Create a new copy each time to ease testing.
*/
struct State {
  int count = 0;
};

#endif // STATE_H