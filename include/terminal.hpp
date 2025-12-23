#include <termios.h>
#include <unistd.h>

struct TerminalHelper {
    termios original;

    TerminalHelper() {
        tcgetattr(STDIN_FILENO, &original);
    }

    void enableRawMode() {
        termios raw = original;
        cfmakeraw(&raw);
        raw.c_cc[VMIN] = 1;
        raw.c_cc[VTIME] = 0;
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    }

    void disableEcho() {
        termios noecho = original;
        noecho.c_lflag &= ~ECHO; 
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &noecho);
    }

    void enableEcho() {
        termios echo = original;
        echo.c_lflag |= ECHO; 
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &echo);
    }

    ~TerminalHelper() {
        tcsetattr(STDIN_FILENO, TCSANOW, &original);
    }
};