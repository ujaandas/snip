#pragma once

#include <string>
#include <vector>

namespace snip::editor {

struct Cursor {
  bool hidden = false;
  int row = 1;
  int col = 1;
};

struct SelectionVm {
  int line;
  int col_start;
  int col_end;
};

struct ViewModel {
  std::vector<std::string> lines;
  std::string statusText;
  int width = 0;
  int height = 0;
  int scrollOffset = 0;
  bool clear = true;
  Cursor cursor;
  std::vector<SelectionVm> selections;
};

} // namespace snip::editor