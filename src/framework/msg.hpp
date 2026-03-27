#pragma once

#include <any>
#include <string>
#include <vector>

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

// Message emitted when a file is successfully loaded by a background worker
struct FileLoadedMsg {
  std::string filepath;
  std::vector<std::string> lines;
};

// Message emitted if a file fails to load
struct ErrorMsg {
  std::string errorMessage;
};

} // namespace snip