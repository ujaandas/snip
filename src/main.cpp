#include "editor/snip.hpp"
#include "editor/state.hpp"
#include "editor/terminal/terminal.hpp"
#include "event_loop/event_loop.hpp"

#include <sys/ioctl.h>
#include <signal.h>
#include <unistd.h>

int main() {
  // Initialize terminal
  Terminal term;
  term.init(0);

  // Setup initial state
  State model;

  // Fetch initial window size
  struct winsize w;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) != -1) {
    model.window.width = w.ws_col;
    model.window.height = w.ws_row;
  }

  // Create our app
  EventLoop loop;
  Snip app(model);

  // Setup event sources
  EventSource inputSrc = EventSource::fromFd(STDIN_FILENO);
  inputSrc.onReadReady = [&app]() {
    char c;
    if (::read(STDIN_FILENO, &c, 1) > 0) {
      // Send the universal keymsg into the app
      app.post(snip::KeyMsg{c});
    }
  };

  EventSource resizeSrc = EventSource::fromSignal(SIGWINCH);
  resizeSrc.onReadReady = [&app]() {
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) != -1) {
      // Send the universal windowsizemsg
      app.post(snip::WindowSizeMsg{w.ws_col, w.ws_row});
    }
  };

  // Register hardware sources with the loop
  loop.addSource(inputSrc);
  loop.addSource(resizeSrc);

  // Hand control over to the App
  app.run(loop);

  return 0;
}