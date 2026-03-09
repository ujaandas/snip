#pragma once

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