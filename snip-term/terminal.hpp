#pragma once

#include <termios.h>

#include <optional>
#include <string_view>

namespace snip::term {

struct Session {
  termios oldTermios{};
  int oldFlags = 0;
  bool valid = false;
};

class Terminal {
 public:
  explicit Terminal(bool echo = false);
  ~Terminal();

  // Disallow copying to avoid double endSession()s
  Terminal(const Terminal&) = delete;
  Terminal& operator=(const Terminal&) = delete;

  Terminal(Terminal&& other);
  Terminal& operator=(Terminal&& other);

  bool valid() const { return session.valid; }

 private:
  Session session{};
};

struct WindowSize {
  int width = 0;
  int height = 0;
};

Session startSession(bool echo = false);
void endSession(const Session& session);

void writeStdout(std::string_view bytes);
std::optional<WindowSize> queryWindowSize(int fd);

}  // namespace snip::term
