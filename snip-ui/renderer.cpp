#include "renderer.hpp"

#include "ansi.hpp"
#include <algorithm>
#include <string>
#include <string_view>
#include <vector>

namespace snip::ui {
namespace {

struct StyleRange {
  int start = 0;
  int end = 0;
};

// Keep width handling in one place so every row is exactly terminal-width columns
int clampWidth(int width) {
  return std::max(0, width);
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

std::string_view lineTextAt(const editor::ViewModel& vm, int lineIndex) {
  if (lineIndex < 0 || lineIndex >= static_cast<int>(vm.lines.size())) {
    return {};
  }

  return vm.lines[static_cast<std::size_t>(lineIndex)];
}

std::vector<StyleRange> styleRangesForLine(const editor::ViewModel& vm, int lineIndex) {
  std::vector<StyleRange> ranges;
  ranges.reserve(vm.selections.size());
  const int width = clampWidth(vm.width);

  for (const auto& sel : vm.selections) {
    if (sel.line != lineIndex) {
      continue;
    }

    int start = std::max(0, sel.col_start);
    int end = std::min(width, sel.col_end);
    if (start < end) {
      ranges.push_back(StyleRange{start, end});
    }
  }

  if (ranges.empty()) {
    return ranges;
  }

  std::sort(ranges.begin(), ranges.end(), [](const StyleRange& a, const StyleRange& b) {
    if (a.start != b.start) {
      return a.start < b.start;
    }
    return a.end < b.end;
  });

  std::vector<StyleRange> merged;
  merged.push_back(ranges.front());

  for (std::size_t i = 1; i < ranges.size(); ++i) {
    auto& last = merged.back();
    const auto next = ranges[i];

    // Merge overlapping (or touching) ranges so render logic has one active range at a time
    if (next.start <= last.end) {
      last.end = std::max(last.end, next.end);
    } else {
      merged.push_back(next);
    }
  }

  return merged;
}

void appendStyledRow(std::string& out, std::string_view src, int width,
                     const std::vector<StyleRange>& ranges) {
  if (width <= 0) {
    return;
  }

  bool reverseOn = false;
  std::size_t rangeIdx = 0;

  for (int col = 0; col < width; ++col) {
    while (rangeIdx < ranges.size() && col >= ranges[rangeIdx].end) {
      ++rangeIdx;
    }

    bool selected =
        rangeIdx < ranges.size() && col >= ranges[rangeIdx].start && col < ranges[rangeIdx].end;

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
  const int width = clampWidth(vm.width);
  const int usableHeight = std::max(0, vm.height - 1);

  out += vm.cursor.hidden ? std::string(ansi::HIDE_CURSOR) : std::string(ansi::SHOW_CURSOR);

  if (vm.clear) {
    out += std::string(ansi::CLEAR_SCREEN);
    out += std::string(ansi::CURSOR_HOME);
  }

  // Main content pass: iterate visible lines and apply that line's styles
  for (int i = 0; i < usableHeight; ++i) {
    const int lineIndex = vm.scrollOffset + i;
    const std::string_view src = lineTextAt(vm, lineIndex);
    const auto ranges = styleRangesForLine(vm, lineIndex);
    appendStyledRow(out, src, width, ranges);
    out += "\n";
  }

  out += statusBar(vm.statusText, width);

  const int row = std::clamp(vm.cursor.row, 1, std::max(1, usableHeight));
  const int col = std::clamp(vm.cursor.col, 1, std::max(1, width));
  out += ansi::moveCursor(row, col);

  return out;
}

} // namespace snip::ui