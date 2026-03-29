#pragma once

#include "snip-editor/text/splitline.hpp"
#include <string>
#include <vector>

namespace snip::editor {

struct WindowState {
  int width = 80;
  int height = 24;
};

struct CursorState {
  int line = 0;
};

struct State {
  WindowState window;
  CursorState cursor;
  int scrollOffset = 0;

  std::vector<std::string> buffer;
  SplitLineBuffer curLine;

  std::string statusText;
  std::string filename;
};

} // namespace snip::editor