#pragma once

#include <string>
#include <vector>

namespace snip::editor {

struct Cursor {
  int row = 1;
  int col = 1;
};

struct ViewModel {
  std::vector<std::string> lines;
  std::string statusText;
  int width = 0;
  int height = 0;
  int scrollOffset = 0;
  bool clear = true;
  bool hideCursor = false;
  Cursor cursor;
};

} // namespace snip::editor