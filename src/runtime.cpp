#include "../include/program.hpp"
#include "../include/terminal.hpp"
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

  if (input.joinable()) {
    input.join();
  }

  if (cmd.joinable()) {
    cmd.join();
  }
}
