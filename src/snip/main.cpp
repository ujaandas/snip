#include "snip/snip.hpp"

#include "snip-core/event_loop.hpp"
#include "snip-runtime/loop_wiring.hpp"
#include "snip-runtime/terminal/terminal.hpp"

#include <csignal>
#include <unistd.h>

int main() {
  const snip::term::Session session = snip::term::startSession(false);
  if (!session.valid) {
    return 1;
  }

  State model;
  if (auto size = snip::runtime::readWindowSizeMsg(STDOUT_FILENO)) {
    model.window.width = size->width;
    model.window.height = size->height;
  }

  Snip app(model);
  snip::core::EventLoop loop;

  loop.addSource(snip::runtime::makeInputSource(
      STDIN_FILENO, [&app](const snip::KeyPressMsg &key) { app.post(key); }));

  loop.addSource(snip::runtime::makeResizeSource(
      SIGWINCH, [&app](const snip::SignalMsg &sig) {
        app.post(sig);
        if (auto size = snip::runtime::readWindowSizeMsg(STDOUT_FILENO)) {
          app.post(*size);
        }
      }));

  app.run(loop);
  snip::term::endSession(session);
  return 0;
}