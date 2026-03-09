#ifndef APP_H
#define APP_H

#include "../event_loop/cc_queue.hpp"
#include "../event_loop/event_loop.hpp"
#include "../event_loop/event_source.hpp"
#include "../event_loop/ezpipe.hpp"
#include "../event_loop/thread_pool.hpp"
#include "cmd.hpp"
#include "msg.hpp"
#include "state.hpp"
#include <functional>
#include <iostream>

struct UpdateResult {
  State newState;
  std::vector<Cmd> commands;
};

class App {
private:
  CCQueue<Msg> msgQ;
  EzPipe wakePipe;

  ThreadPool pool;
  EventLoop *loop = nullptr;

protected:
  State &state;
  bool running = true;

public:
  App(State &s) : msgQ(true), pool(4), state(s) {}

  // Send a message into the app - anyone can call this to add message
  void post(const Msg &msg) {
    msgQ.ccpush(msg);
    wakePipe.write('!'); // Wake event loop
  }

private:
  void dispatch(const std::vector<Cmd> &cmds) {
    for (const auto &cmd : cmds) {
      pool.enqueue([this, cmd]() {
        if (auto maybeMsg = cmd()) {
          this->post(*maybeMsg);
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
  virtual UpdateResult update(State &, Msg) = 0;
  virtual std::string render(State &) = 0;

  void quit() { running = false; }

public:
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
};

#endif