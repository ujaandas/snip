#pragma once

#include "snip-render/ir.hpp"
#include "snip-render/styling.hpp"

#include <algorithm>
#include <string>
#include <string_view>

namespace snip::render::formatting {

inline std::string slice(std::string_view src, int start, int end, int width) {
  start = std::max(0, start);
  end = std::min(end, width);

  std::string out;
  out.reserve(static_cast<std::size_t>(std::max(0, end - start)));

  for (int i = start; i < end; ++i) {
    out.push_back(i < static_cast<int>(src.size()) ? src[static_cast<std::size_t>(i)] : ' ');
  }

  return out;
}

inline void appendSlice(Line& line, Style style, std::string_view src, int start, int end,
                        int width) {
  if (start >= end) {
    return;
  }

  styling::append(line, style, slice(src, start, end, width));
}

} // namespace snip::render::formatting
