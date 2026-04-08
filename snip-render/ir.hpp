#pragma once

#include <string>
#include <vector>

namespace snip::render {

enum class Style : unsigned int {
  None = 0,
  Reverse = 1 << 0,
};

struct Range {
  int start = 0;
  int end = 0;
};

struct Segment {
  std::string text;
  Style style = Style::None;
};

struct Line {
  std::vector<Segment> segments;
};

struct Frame {
  std::vector<Line> lines;
  bool clear = false;
  bool cursorHidden = false;
  int cursorRow = 0;
  int cursorCol = 0;
};

} // namespace snip::render