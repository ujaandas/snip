#pragma once

#include "snip-editor/motion.hpp"
#include "snip-editor/text/splitline.hpp"
#include <string>
#include <vector>

namespace snip::editor {

struct WindowState {
  int width = 80;
  int height = 24;
};

struct State {
  WindowState window;
  int cursorLine = 0;
  Action action;
  int scrollOffset = 0;

  std::vector<std::string> buffer;
  SplitLineBuffer curLine;

  std::string statusText;
  std::string filename;
};

} // namespace snip::editor