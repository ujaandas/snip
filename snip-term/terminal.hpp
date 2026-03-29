#pragma once

#include <optional>
#include <string_view>
#include <termios.h>

namespace snip::term {

struct Session {
  termios oldTermios{};
  int oldFlags = 0;
  bool valid = false;
};

struct WindowSize {
  int width = 0;
  int height = 0;
};

Session startSession(bool echo = false);
void endSession(const Session& session);

void writeStdout(std::string_view bytes);
std::optional<WindowSize> queryWindowSize(int fd);

} // namespace snip::term
