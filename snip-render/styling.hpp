#pragma once

#include "snip-render/ir.hpp"

#include <string>
#include <utility>

namespace snip::render::styling {

inline Segment make(Style style, std::string text) {
  return Segment{std::move(text), style};
}

inline Segment plain(std::string text) {
  return make(Style::None, std::move(text));
}

inline Segment reverse(std::string text) {
  return make(Style::Reverse, std::move(text));
}

inline Line line(Style style, std::string text) {
  Line out;
  out.segments.push_back(make(style, std::move(text)));
  return out;
}

inline Line plainLine(std::string text) {
  return line(Style::None, std::move(text));
}

inline Line reverseLine(std::string text) {
  return line(Style::Reverse, std::move(text));
}

inline void append(Line& line, Style style, std::string text) {
  if (line.segments.empty()) {
    line.segments.push_back(make(style, std::move(text)));
    return;
  }

  Segment& last = line.segments.back();
  if (last.style == style) {
    last.text += text;
    return;
  }

  line.segments.push_back(make(style, std::move(text)));
}

} // namespace snip::render::styling
