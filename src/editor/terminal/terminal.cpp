

#include "terminal.hpp"
#include "ansi.hpp"
#include <fcntl.h>
#include <unistd.h>

Terminal::Terminal() {
  tcgetattr(STDIN_FILENO, &old_termios);
  old_flags = fcntl(STDIN_FILENO, F_GETFL);
}

void Terminal::init(bool echo) {
  new_termios = old_termios;

  // Disable canonical mode & signals (Ctrl-C, Ctrl-Z)
  new_termios.c_lflag &= ~(ICANON | ISIG);

  // Toggle echo
  if (echo) {
    new_termios.c_lflag |= ECHO;
  } else {
    new_termios.c_lflag &= ~ECHO;
  }

  // Disable CR-to-NL translation and flow control (XON/XOFF)
  new_termios.c_iflag &= ~(ICRNL | IXON);

  // Make read() return after 1 byte, no timeout
  new_termios.c_cc[VMIN] = 1;
  new_termios.c_cc[VTIME] = 0;

  // Apply terminal settings
  tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);

  // Set stdin to non-blocking
  fcntl(STDIN_FILENO, F_SETFL, old_flags | O_NONBLOCK);

  // Enter full-screen TUI mode
  writeEscapeCode(ansi::ENTER_ALTBUF);
  writeEscapeCode(ansi::CLEAR_SCREEN);
  writeEscapeCode(ansi::HIDE_CURSOR);
}

Terminal::~Terminal() {
  // Graceful teardown
  writeEscapeCode(ansi::SHOW_CURSOR);
  writeEscapeCode(ansi::EXIT_ALTBUF);

  tcsetattr(STDIN_FILENO, TCSANOW, &old_termios);
  fcntl(STDIN_FILENO, F_SETFL, old_flags);
}

void Terminal::writeEscapeCode(std::string_view code) {
  if (::write(STDOUT_FILENO, code.data(), code.size()) == -1) {
    std::fprintf(stderr, "write failed\n");
  }
}