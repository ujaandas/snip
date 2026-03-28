
#include "terminal.hpp"

#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>

namespace snip::runtime::term {

namespace {
constexpr std::string_view SHOW_CURSOR = "\x1b[?25h";
constexpr std::string_view ENTER_ALTBUF = "\x1b[?1049h";
constexpr std::string_view EXIT_ALTBUF = "\x1b[?1049l";
constexpr std::string_view CLEAR_SCREEN = "\x1b[2J";
} // namespace

void writeStdout(std::string_view bytes) {
  (void)::write(STDOUT_FILENO, bytes.data(), bytes.size());
}

Session startSession(bool echo) {
  Session session;

  if (tcgetattr(STDIN_FILENO, &session.oldTermios) == -1) {
    return session;
  }

  session.oldFlags = fcntl(STDIN_FILENO, F_GETFL);
  if (session.oldFlags == -1) {
    return session;
  }

  termios raw = session.oldTermios;

  raw.c_lflag &= ~(ICANON | ISIG);
  if (echo) {
    raw.c_lflag |= ECHO;
  } else {
    raw.c_lflag &= ~ECHO;
  }

  raw.c_iflag &= ~(ICRNL | IXON);
  raw.c_cc[VMIN] = 1;
  raw.c_cc[VTIME] = 0;

  if (tcsetattr(STDIN_FILENO, TCSANOW, &raw) == -1) {
    return session;
  }

  if (fcntl(STDIN_FILENO, F_SETFL, session.oldFlags | O_NONBLOCK) == -1) {
    tcsetattr(STDIN_FILENO, TCSANOW, &session.oldTermios);
    return session;
  }

  writeStdout(ENTER_ALTBUF);
  writeStdout(CLEAR_SCREEN);

  session.valid = true;
  return session;
}

void endSession(const Session& session) {
  if (!session.valid) {
    return;
  }

  writeStdout(SHOW_CURSOR);
  writeStdout(EXIT_ALTBUF);

  tcsetattr(STDIN_FILENO, TCSANOW, &session.oldTermios);
  fcntl(STDIN_FILENO, F_SETFL, session.oldFlags);
}

std::optional<WindowSize> queryWindowSize(int fd) {
  struct winsize w;
  if (ioctl(fd, TIOCGWINSZ, &w) == -1) {
    return std::nullopt;
  }

  return WindowSize{w.ws_col, w.ws_row};
}

} // namespace snip::runtime::term