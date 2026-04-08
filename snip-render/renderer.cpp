#include "renderer.hpp"
#include "formatting.hpp"
#include "styling.hpp"

#include <algorithm>
#include <string_view>
#include <vector>

namespace {

std::string_view lineTextAt(const snip::editor::ViewModel& vm, int lineIndex) {
  if (lineIndex < 0 || lineIndex >= static_cast<int>(vm.lines.size())) {
    return {};
  }

  return vm.lines[static_cast<std::size_t>(lineIndex)];
}

std::vector<snip::render::Range> selectionRanges(const snip::editor::ViewModel& vm, int lineIndex,
                                                 int width) {
  std::vector<snip::render::Range> ranges;
  if (width <= 0 || vm.selections.empty()) {
    return ranges;
  }

  ranges.reserve(vm.selections.size());

  for (const auto& sel : vm.selections) {
    if (sel.line != lineIndex) {
      continue;
    }

    const int start = std::max(0, sel.col_start);
    const int end = std::min(width, sel.col_end);
    if (start < end) {
      ranges.push_back({start, end});
    }
  }

  if (ranges.size() < 2) {
    return ranges;
  }

  std::sort(ranges.begin(), ranges.end(),
            [](const snip::render::Range& a, const snip::render::Range& b) {
              if (a.start != b.start) {
                return a.start < b.start;
              }
              return a.end < b.end;
            });

  std::vector<snip::render::Range> merged;
  merged.reserve(ranges.size());
  for (const auto& range : ranges) {
    if (merged.empty() || range.start > merged.back().end) {
      merged.push_back(range);
    } else {
      merged.back().end = std::max(merged.back().end, range.end);
    }
  }

  return merged;
}

snip::render::Line buildContentLine(const snip::editor::ViewModel& vm, int lineIndex, int width) {
  snip::render::Line line;
  if (width <= 0) {
    return line;
  }

  const std::string_view src = lineTextAt(vm, lineIndex);
  const auto ranges = selectionRanges(vm, lineIndex, width);
  line.segments.reserve(ranges.size() * 2 + 1);

  int col = 0;
  for (const auto& range : ranges) {
    snip::render::formatting::appendSlice(line, snip::render::Style::None, src, col, range.start,
                                          width);
    snip::render::formatting::appendSlice(line, snip::render::Style::Reverse, src, range.start,
                                          range.end, width);
    col = range.end;
  }

  snip::render::formatting::appendSlice(line, snip::render::Style::None, src, col, width, width);

  return line;
}

snip::render::Line buildStatusLine(std::string_view status, int width) {
  if (width <= 0) {
    return {};
  }

  return snip::render::styling::reverseLine(
      snip::render::formatting::slice(status, 0, width, width));
}

} // namespace

namespace snip::render {

Frame Renderer::render(const editor::ViewModel& vm) const {
  Frame frame;

  frame.clear = vm.clear;
  frame.cursorHidden = vm.cursor.hidden;

  const int width = std::max(0, vm.width);
  const int usableHeight = std::max(0, vm.height - 1);

  frame.lines.reserve(static_cast<std::size_t>(usableHeight + 1));

  for (int i = 0; i < usableHeight; ++i) {
    const int lineIndex = vm.scrollOffset + i;
    frame.lines.push_back(buildContentLine(vm, lineIndex, width));
  }

  frame.lines.push_back(buildStatusLine(vm.statusText, width));

  frame.cursorRow = std::clamp(vm.cursor.row, 1, std::max(1, usableHeight));
  frame.cursorCol = std::clamp(vm.cursor.col, 1, std::max(1, width));

  return frame;
}

} // namespace snip::render