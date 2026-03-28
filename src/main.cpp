#include "editor/snip.hpp"
#include "editor/state.hpp"
#include "framework/input.hpp"
#include "framework/terminal/terminal.hpp"
#include "event_loop/event_loop.hpp"

#include <signal.h>
#include <unistd.h>

int main() {
  const auto session = snip::term::startSession(false);
  if (!session.valid) {
    return 1;
  }

  // Setup initial state
  State model;

  if (auto size = snip::term::queryWindowSize(STDOUT_FILENO)) {
    model.window.width = size->width;
    model.window.height = size->height;
  }

  // Create our app
  EventLoop loop;
  Snip app(model);

  // Setup event sources
  EventSource inputSrc = EventSource::fromFd(STDIN_FILENO);
  inputSrc.onReadReady = [&app]() {
    if (auto key = snip::input::readKeyPress(STDIN_FILENO)) {
      app.post(*key);
    }
  };

  EventSource resizeSrc = EventSource::fromSignal(SIGWINCH);
  resizeSrc.onReadReady = [&app]() {
    app.post(snip::SignalMsg{SIGWINCH});

    if (auto size = snip::term::queryWindowSize(STDOUT_FILENO)) {
      app.post(snip::WindowSizeMsg{size->width, size->height});
    }
  };

  // Register hardware sources with the loop
  loop.addSource(inputSrc);
  loop.addSource(resizeSrc);

  // Hand control over to the App
  app.run(loop);

  snip::term::endSession(session);

  return 0;
}