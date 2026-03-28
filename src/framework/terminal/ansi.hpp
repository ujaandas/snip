#pragma once

#include <string>
#include <string_view>

namespace snip::ansi {

// Core Control
constexpr std::string_view HIDE_CURSOR = "\x1b[?25l";
constexpr std::string_view SHOW_CURSOR = "\x1b[?25h";
constexpr std::string_view ENTER_ALTBUF = "\x1b[?1049h";
constexpr std::string_view EXIT_ALTBUF = "\x1b[?1049l";

// Drawing & Clearing
constexpr std::string_view CLEAR_SCREEN = "\x1b[2J";
constexpr std::string_view CLEAR_LINE = "\x1b[2K";
constexpr std::string_view CURSOR_HOME = "\x1b[H";

// Styling
constexpr std::string_view REVERSE = "\x1b[7m";
constexpr std::string_view RESET = "\x1b[0m";

// Helpers for the future (Parameterized codes)
inline std::string moveCursor(int row, int col) {
  return "\x1b[" + std::to_string(row) + ";" + std::to_string(col) + "H";
}

inline std::string eraseLineAt(int row) {
  return moveCursor(row, 1) + std::string(CLEAR_LINE);
}

} // namespace snip::ansi