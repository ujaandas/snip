#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

class Terminal {
  struct termios old, new1;
  struct winsize w;

public:
  Terminal() {
    tcgetattr(STDIN_FILENO, &old);
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  }

  void init(int echo) {
    new1 = old;
    new1.c_lflag &= ~ICANON;
    new1.c_lflag &= echo ? ECHO : ~ECHO;
    tcsetattr(0, TCSANOW, &new1);
  }

  ~Terminal() { tcsetattr(STDIN_FILENO, TCSANOW, &old); }
};