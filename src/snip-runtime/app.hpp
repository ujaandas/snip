#pragma once

#include "event_loop/cc_queue.hpp"
#include "event_loop/event_loop.hpp"
#include "event_loop/event_source.hpp"
#include "event_loop/ezpipe.hpp"
#include "event_loop/thread_pool.hpp"
#include "cmd.hpp"
#include "msg.hpp"
#include <functional>
#include <iostream>

namespace snip {
template <typename State> struct UpdateResult {
  State newState;
  std::vector<Cmd> commands;
};

template <typename State> class App {
private:
  CCQueue<Msg> msgQ{true};
  EzPipe wakePipe;

  ThreadPool pool{4};
  EventLoop *loop = nullptr;

protected:
  State &state;
  bool running = true;

public:
  App(State &s) : state(s) {}
  virtual ~App() = default;

  // T is arbitrary and can include richer framework payloads.
  template <typename T> void post(T &&msg) {
    // Convert to std::any as we push it into the queue.
    msgQ.ccpush(std::forward<T>(msg));
    wakePipe.write('!');
  }

  // Run the app inside an EventLoop
  void run(EventLoop &eventLoop) {
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
  void dispatch(const std::vector<Cmd> &cmds) {
    for (const auto &cmd : cmds) {
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
  EventSource messageSource() {
    EventSource src = EventSource::fromFd(wakePipe.getReadFd());

    src.onReadReady = [this]() {
      wakePipe.clear();

      Msg msg;
      // Message handler
      while (msgQ.ccawait(msg)) {
        if (std::any_cast<QuitMsg>(&msg)) {
          quit();

          if (!running && loop) {
            loop->stop();
            break;
          }

          continue;
        }

        UpdateResult result = update(state, msg);
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
  virtual UpdateResult<State> update(State &, Msg) = 0;
  virtual std::string render(State &) = 0;

  void quit() { running = false; }
};
} // namespace snip