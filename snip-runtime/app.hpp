#pragma once

#include "cmd.hpp"
#include "msg.hpp"
#include "snip-core/cc_queue.hpp"
#include "snip-core/event_loop.hpp"
#include "snip-core/event_source.hpp"
#include "snip-core/ezpipe.hpp"
#include "snip-core/thread_pool.hpp"
#include <functional>
#include <iostream>
#include <utility>
#include <variant>

namespace snip::runtime {
template <typename State> struct UpdateResult {
  State newState;
  std::vector<Cmd> commands;
};

/*
  App represent's Snip's runtime and provides an API for managing the application state and handling
  events. It provides a main event loop, a message passing system, and a simple command abstraction
  for running background tasks.

  We subclass App and implement the init, update, and render methods. Then call
  run() with an EventLoop instance to start the app.
*/
template <typename State> class App {
private:
  core::CCQueue<Msg> msgQ{true};
  core::EzPipe wakePipe;

  core::ThreadPool pool{4};
  core::EventLoop* loop = nullptr;

protected:
  State& state;
  bool running = true;

public:
  App(State& s) : state(s) {}
  virtual ~App() = default;

  // T must be a supported runtime::Msg alternative.
  template <typename T> void post(T&& msg) {
    msgQ.ccpush(Msg{std::forward<T>(msg)});
    wakePipe.write('!');
  }

  // Run the app inside an EventLoop
  void run(core::EventLoop& eventLoop) {
    loop = &eventLoop;

    // Register our mailbox source
    loop->addSource(messageSource());

    // Initial render
    std::cout << render(state) << std::flush;

    // Run init commands
    dispatch(init());

    loop->run();
  }

private:
  void dispatch(const std::vector<Cmd>& cmds) {
    for (const auto& cmd : cmds) {
      pool.enqueue([this, cmd]() {
        if (auto maybeMsg = cmd()) {
          // Template argument deduction lets us omit the angle bracket syntax
          // here
          post(*maybeMsg);
        }
      });
    }
  }

  // Event source used by the event loop
  core::EventSource messageSource() {
    core::EventSource src = core::EventSource::fromFd(wakePipe.getReadFd());

    src.onReadReady = [this]() {
      wakePipe.clear();

      Msg msg;
      // Message handler
      while (msgQ.ccawait(msg)) {
        if (std::holds_alternative<QuitMsg>(msg)) {
          quit();

          if (!running && loop) {
            loop->stop();
            break;
          }

          continue;
        }

        UpdateResult result = update(state, std::move(msg));
        state = result.newState;

        std::cout << render(state) << std::flush;

        dispatch(result.commands);

        if (!running && loop) {
          loop->stop();
        }
      }
    };

    return src;
  }

protected:
  virtual std::vector<Cmd> init() = 0;
  virtual UpdateResult<State> update(State&, Msg) = 0;
  virtual std::string render(State&) = 0;

  void quit() {
    running = false;
  }
};
} // namespace snip::runtime