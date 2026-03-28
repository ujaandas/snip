#pragma once

#include "ansi.hpp"
#include <algorithm>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace snip::term {

struct Cursor {
  int row = 1;
  int col = 1;
};

struct FrameSpec {
  int width = 0;
  int height = 0;
  int scrollOffset = 0;
  bool clear = true;
  bool hideCursor = true;
};

inline std::string clipLine(const std::string &line, int width) {
  if (width <= 0) {
    return "";
  }
  if (line.size() <= width) {
    return line;
  }
  return line.substr(0, width);
}

inline std::string padRight(std::string s, int width) {
  if (width <= 0) {
    return "";
  }
  if (s.size() > width) {
    s.resize(width);
    return s;
  }
  if (s.size() < width) {
    s.append(width - s.size(), ' ');
  }
  return s;
}

inline std::string statusBar(std::string_view text, int width) {
  return std::string(ansi::REVERSE) + padRight(std::string(text), width) +
         std::string(ansi::RESET);
}

inline std::string renderFrame(const std::vector<std::string> &buffer,
                               const std::string &statusText,
                               const FrameSpec &spec,
                               std::optional<Cursor> cursor = std::nullopt) {
  std::string out;

  if (spec.hideCursor) {
    out += std::string(ansi::HIDE_CURSOR);
  } else {
    out += std::string(ansi::SHOW_CURSOR);
  }

  if (spec.clear) {
    out += std::string(ansi::CLEAR_SCREEN);
    out += std::string(ansi::CURSOR_HOME);
  }

  const int usableHeight = std::max(0, spec.height - 1);

  for (int i = 0; i < usableHeight; ++i) {
    const int lineIndex = spec.scrollOffset + i;
    std::string row;

    if (lineIndex >= 0 && lineIndex < buffer.size()) {
      row = clipLine(buffer[(lineIndex)], spec.width);
    }

    out += padRight(std::move(row), spec.width);
    out += "\n";
  }

  out += statusBar(statusText, spec.width);

  if (cursor.has_value()) {
    const int row = std::max(1, cursor->row);
    const int col = std::max(1, cursor->col);
    out += ansi::moveCursor(row, col);
  }

  return out;
}

} // namespace snip::term
