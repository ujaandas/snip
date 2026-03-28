#include "snip/snip.hpp"
#include "snip-core/event_loop.hpp"
#include "snip-core/event_source.hpp"
#include "snip-runtime/input.hpp"
#include "snip-runtime/terminal/terminal.hpp"
#include <signal.h>
#include <unistd.h>

int main() {
  const auto session =
      snip::runtime::term::startSession(false);
  if (!session.valid) {
    return 1;
  }

  snip::State model;

  // Set initial window size
  if (auto size = snip::runtime::term::queryWindowSize(STDOUT_FILENO)) {
    model.window.width = size->width;
    model.window.height = size->height;
  }

  // Initialize app and event loop
  snip::Snip app(model);
  snip::core::EventLoop loop;

  // Register STDIN input source
  auto input = snip::core::EventSource::fromFd(STDIN_FILENO);
  input.onReadReady = [&app]() {
    if (auto key = snip::runtime::input::readKeyPress(STDIN_FILENO)) {
      app.post(*key);
    }
  };
  loop.addSource(std::move(input));

  // Register SIGWINCH resize source
  auto resize = snip::core::EventSource::fromSignal(SIGWINCH);
  resize.onReadReady = [&app]() {
    app.post(snip::runtime::SignalMsg{SIGWINCH});
    if (auto size = snip::runtime::term::queryWindowSize(STDOUT_FILENO)) {
      app.post(snip::runtime::WindowSizeMsg{size->width, size->height});
    }
  };
  loop.addSource(std::move(resize));

  app.run(loop);
  snip::runtime::term::endSession(session);
  return 0;
}