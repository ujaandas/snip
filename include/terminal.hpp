#include <stdio.h>
#include <termios.h>
#include <unistd.h>

struct TerminalHelper {
  struct termios old, new1;

  TerminalHelper() { tcgetattr(STDIN_FILENO, &old); }

  void init(int echo) {
    new1 = old;
    new1.c_lflag &= ~ICANON;
    new1.c_lflag &= echo ? ECHO : ~ECHO;
    tcsetattr(0, TCSANOW, &new1);
  }

  ~TerminalHelper() { tcsetattr(STDIN_FILENO, TCSANOW, &old); }
};