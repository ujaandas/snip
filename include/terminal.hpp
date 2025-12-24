#include <cstring>
#include <fcntl.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#ifndef TERM_H
#define TERM_H

#define ENTER_ALTBUF "\x1b[?1049h"
#define EXIT_ALTBUF "\x1b[?1049l"

/*
Handles input/output directly pertaining to the terminal.
Also includes setting it up for raw input and non-canonical mode,
buffering, alternative buffer, etc... using termios.
*/
class Terminal {
  struct termios old, new1;

public:
  Terminal() { tcgetattr(STDIN_FILENO, &old); }
  void init(int echo) {
    new1 = old;
    new1.c_lflag &= ~ICANON;
    new1.c_lflag &= echo ? ECHO : ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &new1);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
    writeEscapeCode(ENTER_ALTBUF);
  }

  void writeEscapeCode(const char code[]) {
    write(STDOUT_FILENO, code, strlen(code));
  }

  ~Terminal() {
    writeEscapeCode(EXIT_ALTBUF);
    tcsetattr(STDIN_FILENO, TCSANOW, &old);
  }
};

#endif // TERM_H