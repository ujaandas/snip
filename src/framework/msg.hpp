#ifndef MSG_H
#define MSG_H

#include <any>

namespace snip {

// The universal message type
using Msg = std::any;

struct KeyMsg {
  char key;
};
struct WindowSizeMsg {
  int width;
  int height;
};

} // namespace snip

#endif // MSG_H