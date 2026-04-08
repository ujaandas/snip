#include "translator.hpp"
#include "ansi.hpp"

#include <string>
namespace snip::render {
namespace {

inline std::string moveCursor(int row, int col) {
  return "\x1b[" + std::to_string(row) + ";" + std::to_string(col) + "H";
}

} // namespace

std::string AnsiTranslator::translate(const Frame& frame) const {
  std::string out;

  out += frame.cursorHidden ? std::string(ansi::HIDE_CURSOR) : std::string(ansi::SHOW_CURSOR);

  if (frame.clear) {
    out += std::string(ansi::CLEAR_SCREEN);
    out += std::string(ansi::CURSOR_HOME);
  }

  for (std::size_t lineIndex = 0; lineIndex < frame.lines.size(); ++lineIndex) {
    const auto& line = frame.lines[lineIndex];

    for (const auto& segment : line.segments) {
      if (segment.style == Style::Reverse) {
        out += ansi::REVERSE;
      }

      out += segment.text;

      if (segment.style == Style::Reverse) {
        out += ansi::RESET;
      }
    }

    if (lineIndex + 1 < frame.lines.size()) {
      out.push_back('\n');
    }
  }

  out += moveCursor(frame.cursorRow, frame.cursorCol);
  return out;
}

} // namespace snip::render
