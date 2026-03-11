#pragma once

#include <string_view>
#include <termios.h>

class Terminal {
  struct termios old_termios, new_termios;
  int old_flags;

public:
  Terminal();

  void init(bool echo = false);

  ~Terminal();

private:
  void writeEscapeCode(std::string_view code);
};