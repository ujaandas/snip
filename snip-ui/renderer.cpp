#include "renderer.hpp"

#include "ansi.hpp"
#include <algorithm>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace snip::ui {
namespace {

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

std::vector<std::pair<int, int>> normalizeRanges(const editor::ViewModel& vm, int lineIndex) {
  std::vector<std::pair<int, int>> ranges;
  ranges.reserve(vm.selections.size());

  for (const auto& sel : vm.selections) {
    if (sel.line != lineIndex) {
      continue;
    }

    int start = std::max(0, sel.col_start);
    int end = std::min(vm.width, sel.col_end);
    if (start < end) {
      ranges.push_back({start, end});
    }
  }

  if (ranges.empty()) {
    return ranges;
  }

  std::sort(ranges.begin(), ranges.end());

  std::vector<std::pair<int, int>> merged;
  merged.push_back(ranges.front());

  for (std::size_t i = 1; i < ranges.size(); ++i) {
    auto& last = merged.back();
    const auto [start, end] = ranges[i];

    if (start <= last.second) {
      last.second = std::max(last.second, end);
    } else {
      merged.push_back({start, end});
    }
  }

  return merged;
}

void appendStyledRow(std::string& out, std::string_view src, int width,
                     const std::vector<std::pair<int, int>>& ranges) {
  if (width <= 0) {
    return;
  }

  bool reverseOn = false;
  std::size_t rangeIdx = 0;

  for (int col = 0; col < width; ++col) {
    while (rangeIdx < ranges.size() && col >= ranges[rangeIdx].second) {
      ++rangeIdx;
    }

    bool selected =
        rangeIdx < ranges.size() && col >= ranges[rangeIdx].first && col < ranges[rangeIdx].second;

    if (selected && !reverseOn) {
      out += ansi::REVERSE;
      reverseOn = true;
    } else if (!selected && reverseOn) {
      out += ansi::RESET;
      reverseOn = false;
    }

    if (col < static_cast<int>(src.size())) {
      out.push_back(src[static_cast<std::size_t>(col)]);
    } else {
      out.push_back(' ');
    }
  }

  if (reverseOn) {
    out += ansi::RESET;
  }
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
    std::string_view src;

    if (lineIndex >= 0 && lineIndex < vm.lines.size()) {
      src = vm.lines[static_cast<std::size_t>(lineIndex)];
    }

    const auto ranges = normalizeRanges(vm, lineIndex);
    appendStyledRow(out, src, vm.width, ranges);
    out += "\n";
  }

  out += statusBar(vm.statusText, vm.width);

  const int row = std::max(1, vm.cursor.row);
  const int col = std::max(1, vm.cursor.col);
  out += ansi::moveCursor(row, col);

  return out;
}

} // namespace snip::ui