#include "../include/core/asyncqueue.hpp"
#include "../include/core/eventloop.hpp"
#include "../include/program.hpp"
#include "../include/terminal.hpp"
#include "core/threadpool.hpp"
#include "message.hpp"
#include "state.hpp"
#include <iostream>
#include <poll.h>
#include <sys/ioctl.h>
#include <thread>
#include <unistd.h>

void Program::requestQuit() {
  running.store(false);
  msgQ.close();
  cmdQ.close();
}

void Program::handleInput() {
  struct pollfd pfd = {STDIN_FILENO, POLLIN, 0};
  struct winsize w;

  while (running.load()) {
    // Wait 100 ms for keypress
    int ret = poll(&pfd, 1, 100);

    if (ret > 0 && (pfd.revents & POLLIN)) {
      char c;
      if (read(STDIN_FILENO, &c, 1) > 0) {
        // Map char to Msg and push to msgQ
        msgQ.ccpush(KeypressMsg{c});
      }
    }

    // Check window size
    // TODO: Replace with SIGINT
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    if (w.ws_col != state.window.width || w.ws_row != state.window.height) {
      msgQ.ccpush(WindowDimensionsMsg{w.ws_col, w.ws_row});
    }

    // Prevent busy-waiting
    // TODO: Look into self-pipe trick (?)
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

void Program::executeCmd(const Cmd &cmd) {
  if (auto maybeMsg = cmd()) {
    msgQ.ccpush(*maybeMsg);
  }
}

void Program::handleCmd() {
  Cmd cmd;
  while (cmdQ.ccawait(cmd)) {
    executeCmd(cmd);
  }
}

void Program::run() {
  // Setup terminal
  Terminal term;
  term.init(0);

  // Setup app stuff
  EventLoop loop;
  AsyncQueue<Msg> msgQueue;
  ThreadPool pool(4);

  // Take commands from update/init and toss them to workers
  auto dispatchCmds = [&pool, &msgQueue](const std::vector<Cmd> &cmds) {
    for (const auto &cmd : cmds) {
      pool.enqueue([cmd, &msgQueue]() {
        // Run the command on a background thread
        // If it returns a Msg, ring the doorbell!
        if (auto maybeMsg = cmd()) {
          msgQueue.push(*maybeMsg);
        }
      });
    }
  };

  msgQueue.onItem = [this, &loop, &dispatchCmds](Msg msg) {
    // Get the new state and commands
    UpdateResult result = update(state, msg);
    state = result.newState;

    // Render to the screen
    std::cout << render(state) << std::flush;

    // Dispatch any new commands to the background
    dispatchCmds(result.commands);

    // Check for quit command
    if (!running.load()) {
      loop.stop();
    }
  };

  // Stdin
  EventSource inputSrc = EventSource::fromFd(STDIN_FILENO);
  inputSrc.onReadReady = [&msgQueue]() {
    char c;
    if (read(STDIN_FILENO, &c, 1) > 0) {
      msgQueue.push(KeypressMsg{c});
    }
  };

  // Window resize
  EventSource resizeSrc = EventSource::fromSignal(SIGWINCH);
  resizeSrc.onReadReady = [&msgQueue]() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    msgQueue.push(WindowDimensionsMsg{w.ws_col, w.ws_row});
  };

  // Register sources
  loop.addSource(inputSrc);
  loop.addSource(resizeSrc);
  loop.addSource(msgQueue.getEventSource());

  struct winsize w;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) != -1) {
    state.window.width = w.ws_col;
    state.window.height = w.ws_row;
  }

  // Do the first render and fire the initial commands
  std::cout << render(state) << std::flush;
  dispatchCmds(init());

  loop.run();
}