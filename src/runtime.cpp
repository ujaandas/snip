#include "../include/program.hpp"
#include "../include/terminal.hpp"
#include "message.hpp"
#include "state.hpp"
#include <iostream>
#include <sys/ioctl.h>
#include <thread>

void Program::requestQuit() {
  running.store(false);
  msgQ.close();
}

void Program::handleInput() {
  while (running.load()) {
    // Read keyboard input
    char c;
    if (read(STDIN_FILENO, &c, 1) > 0) {
      msgQ.ccpush(KeypressMsg{c});
    }

    // Check window size
    // TODO: Replace with SIGINT
    struct winsize w;
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
  while (running.load()) {
    Cmd cmd;
    if (!cmdQ.ccawait(cmd)) {
      break; // queue closed and empty
    }
    executeCmd(cmd);
  }
}

void Program::run() {
  Terminal term;
  term.init(0);

  std::thread input(&Program::handleInput, this);
  std::thread cmd(&Program::handleCmd, this);

  for (auto cmd : init()) {
    cmdQ.ccpush(cmd);
  }

  while (running.load()) {
    Msg msg;
    if (!msgQ.ccawait(msg)) {
      break; // queue closed and empty
    }

    auto result = update(state, msg);
    state = result.newState;
    std::cout << render(state) << std::flush;

    for (auto &cmd : result.commands) {
      cmdQ.ccpush(cmd);
    }
  }

  input.join();
}
