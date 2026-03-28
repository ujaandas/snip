#include "renderer.hpp"

#include "ansi.hpp"
#include <algorithm>
#include <string>
#include <string_view>

namespace snip::ui {
namespace {

std::string clipLine(const std::string &line, int width) {
  if (width <= 0) {
    return "";
  }
  if (line.size() <= static_cast<std::size_t>(width)) {
    return line;
  }
  return line.substr(0, static_cast<std::size_t>(width));
}

std::string padRight(std::string s, int width) {
  if (width <= 0) {
    return "";
  }
  if (s.size() > static_cast<std::size_t>(width)) {
    s.resize(static_cast<std::size_t>(width));
    return s;
  }
  if (s.size() < static_cast<std::size_t>(width)) {
    s.append(static_cast<std::size_t>(width) - s.size(), ' ');
  }
  return s;
}

std::string statusBar(std::string_view text, int width) {
  return std::string(ansi::REVERSE) + padRight(std::string(text), width) +
         std::string(ansi::RESET);
}

} // namespace

std::string AnsiRenderer::render(const editor::ViewModel &vm) const {
  std::string out;

  out += vm.hideCursor ? std::string(ansi::HIDE_CURSOR)
                       : std::string(ansi::SHOW_CURSOR);

  if (vm.clear) {
    out += std::string(ansi::CLEAR_SCREEN);
    out += std::string(ansi::CURSOR_HOME);
  }

  const int usableHeight = std::max(0, vm.height - 1);

  for (int i = 0; i < usableHeight; ++i) {
    const int lineIndex = vm.scrollOffset + i;
    std::string row;

    if (lineIndex >= 0 && lineIndex < static_cast<int>(vm.lines.size())) {
      row = clipLine(vm.lines[static_cast<std::size_t>(lineIndex)], vm.width);
    }

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