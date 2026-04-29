#include "runtime.hpp"

#include <functional>
#include <iostream>
#include <utility>
#include <variant>

using namespace snip::runtime;

Runtime::Runtime(editor::State& s) : state(s) {}

void Runtime::post(Msg&& msg) {
  msgQ.ccpush(msg);
  wakePipe.write('!');
}

void Runtime::run(core::EventLoop& eventLoop) {
  loop = &eventLoop;

  // Register our mailbox source
  loop->addSource(messageSource());

  // Initial render
  std::cout << render(state) << std::flush;

  // Run init commands
  dispatch(init());

  loop->run();
}

void Runtime::quit() { running = false; }

void Runtime::dispatch(const std::vector<Cmd>& cmds) {
  for (const auto& cmd : cmds) {
    pool.enqueue([this, cmd]() {
      if (auto maybeMsg = cmd()) {
        post(std::move(*maybeMsg));
      }
    });
  }
}

snip::core::EventSource Runtime::messageSource() {
  core::EventSource src = core::EventSource::fromFd(wakePipe.getReadFd());

  src.onReadReady = [this]() {
    wakePipe.clear();

    Msg msg;
    while (msgQ.ccawait(msg)) {
      if (std::holds_alternative<QuitMsg>(msg)) {
        running = false;
      } else {
        auto [newState, cmds] = update(state, std::move(msg));
        state = newState;
        std::cout << render(state) << std::flush;
        dispatch(cmds);
      }

      if (!running && loop) {
        loop->stop();
      }
    }
  };

  return src;
}