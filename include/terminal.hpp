#include <cstring>
#include <fcntl.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#define ENTER_ALTBUF "\x1b[?1049h"
#define EXIT_ALTBUF "\x1b[?1049l"

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