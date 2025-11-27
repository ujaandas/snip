#include <termios.h>
#include <unistd.h>

struct TerminalHelper {
    termios original;

    void enableRawMode() {
        termios raw = original;
        cfmakeraw(&raw);
        raw.c_cc[VMIN] = 1;
        raw.c_cc[VTIME] = 0;
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    }

    ~TerminalHelper() {
        tcsetattr(STDIN_FILENO, TCSANOW, &original);
    }
};