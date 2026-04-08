#pragma once
#include <string>
#include <string_view>

namespace snip::render::ansi {

inline constexpr std::string_view HIDE_CURSOR = "\x1b[?25l";
inline constexpr std::string_view SHOW_CURSOR = "\x1b[?25h";
inline constexpr std::string_view CLEAR_SCREEN = "\x1b[2J";
inline constexpr std::string_view CURSOR_HOME = "\x1b[H";
inline constexpr std::string_view REVERSE = "\x1b[7m";
inline constexpr std::string_view RESET = "\x1b[0m";

inline std::string moveCursor(int row, int col) {
  return "\x1b[" + std::to_string(row) + ";" + std::to_string(col) + "H";
}

} // namespace snip::render::ansi