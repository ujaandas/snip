#include "renderer.hpp"

#include "ansi.hpp"
#include <algorithm>
#include <string>
#include <string_view>

namespace snip::ui {
namespace {

std::string clipLine(const std::string& line, int width) {
  if (width <= 0) {
    return "";
  }
  if (line.size() <= width) {
    return line;
  }
  return line.substr(0, width);
}

std::string padRight(std::string s, int width) {
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

std::string statusBar(std::string_view text, int width) {
  return std::string(ansi::REVERSE) + padRight(std::string(text), width) + std::string(ansi::RESET);
}

} // namespace

std::string AnsiRenderer::render(const editor::ViewModel& vm) const {
  std::string out;

  out += vm.cursor.hidden ? std::string(ansi::HIDE_CURSOR) : std::string(ansi::SHOW_CURSOR);

  if (vm.clear) {
    out += std::string(ansi::CLEAR_SCREEN);
    out += std::string(ansi::CURSOR_HOME);
  }

  const int usableHeight = std::max(0, vm.height - 1);

  for (int i = 0; i < usableHeight; ++i) {
    const int lineIndex = vm.scrollOffset + i;
    std::string row;

    if (lineIndex >= 0 && lineIndex < vm.lines.size()) {
      const std::string& src = vm.lines[lineIndex];

      // Collect selection ranges that touch this line
      std::vector<std::pair<int, int>> ranges;
      for (auto const& sel : vm.selections) {
        if (sel.line == lineIndex)
          ranges.push_back({sel.col_start, sel.col_end});
      }

      // Build the row with highlighting
      row.reserve(src.size());
      for (int col = 0; col < src.size(); ++col) {
        bool in_sel = false;
        for (auto [s, e] : ranges) {
          if (col >= s && col < e) {
            in_sel = true;
            break;
          }
        }
        if (in_sel) {
          row += ansi::REVERSE;
          row.push_back(src[col]);
          row += ansi::RESET;
        } else {
          row.push_back(src[col]);
        }
      }
    }

    row = clipLine(std::move(row), vm.width);
    out += padRight(std::move(row), vm.width);
    out += "\n";
  }

  out += statusBar(vm.statusText, vm.width);

  const int row = std::max(1, vm.cursor.row);
  const int col = std::max(1, vm.cursor.col);
  out += ansi::moveCursor(row, col);

  return out;
}

} // namespace snip::ui