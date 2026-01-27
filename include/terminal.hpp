#include <cstring>
#include <fcntl.h>
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#ifndef TERM_H
#define TERM_H

#define HIDE_CURSOR "\x1b[?25l"
#define SHOW_CURSOR "\x1b[?25h"
#define CLEAR_ALTBUF "\x1b[2J\x1b[H"
#define ENTER_ALTBUF "\x1b[?1049h"
#define EXIT_ALTBUF "\x1b[?1049l"

/*
Handles input/output directly pertaining to the terminal.
Also includes setting it up for raw input and non-canonical mode,
buffering, alternative buffer, etc... using termios.
*/
class Terminal {
  struct termios old, new1;
  int old_flags;

public:
  Terminal() { tcgetattr(STDIN_FILENO, &old); }

  void init(int echo) {
    new1 = old;

    // Disable canonical mode
    new1.c_lflag &= ~ICANON;

    // Toggle echo
    if (echo)
      new1.c_lflag |= ECHO;
    else
      new1.c_lflag &= ~ECHO;

    // Disable signals (Ctrl-C, Ctrl-Z)
    new1.c_lflag &= ~ISIG;

    // Disable CR-to-NL translation and XON/XOFF
    new1.c_iflag &= ~(ICRNL | IXON);

    // Make read() return after 1 byte
    new1.c_cc[VMIN] = 1;
    new1.c_cc[VTIME] = 0;

    // Unbuffered stdout
    setvbuf(stdout, NULL, _IONBF, 0);

    // Set stdin to noblock
    old_flags = fcntl(STDIN_FILENO, F_GETFL);
    fcntl(STDIN_FILENO, F_SETFL, old_flags | O_NONBLOCK);

    tcsetattr(STDIN_FILENO, TCSANOW, &new1);

    writeEscapeCode(ENTER_ALTBUF);
    writeEscapeCode(CLEAR_ALTBUF);
    writeEscapeCode(HIDE_CURSOR);
  }

  void writeEscapeCode(const char code[]) {
    write(STDOUT_FILENO, code, strlen(code));
  }

  ~Terminal() {
    writeEscapeCode(EXIT_ALTBUF);
    writeEscapeCode(SHOW_CURSOR);
    tcsetattr(STDIN_FILENO, TCSANOW, &old);
    fcntl(STDIN_FILENO, F_SETFL, old_flags);
  }
};

#endif // TERM_H