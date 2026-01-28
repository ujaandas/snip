#include "../include/program.hpp"
#include "../include/terminal.hpp"
#include "message.hpp"
#include "state.hpp"
#include <iostream>
#include <mutex>
#include <sys/ioctl.h>
#include <thread>

void Program::requestQuit() {
  running.store(false);
  msgCv.notify_all();
}

void Program::handleInput() {
  while (running.load()) {
    // Read keyboard input
    char c;
    if (read(STDIN_FILENO, &c, 1) > 0) {
      std::unique_lock<std::mutex> lock(msgQMutex);
      // Should I return a command here instead?
      msgQ.push(KeypressMsg{c});
      msgCv.notify_one();
    }

    // Check window size
    // TODO: Replace with SIGINT
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    if (w.ws_col != state.window.width || w.ws_row != state.window.height) {
      std::unique_lock<std::mutex> lock(msgQMutex);
      msgQ.push(WindowDimensionsMsg{w.ws_col, w.ws_row});
      msgCv.notify_one();
    }

    // Prevent busy-waiting
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

void Program::executeCmd(const Cmd &cmd) {
  if (auto maybeMsg = cmd()) {
    std::unique_lock<std::mutex> lock(msgQMutex);
    msgQ.push(*maybeMsg);
    msgCv.notify_one();
  }
}

void Program::handleCmd() {
  while (running.load()) {
    Cmd cmd;
    {
      std::unique_lock<std::mutex> lock(cmdQMutex);
      while (cmdQ.empty() && running.load()) {
        cmdCv.wait(lock);
      }

      if (!running.load() && cmdQ.empty()) {
        break;
      }

      cmd = cmdQ.front();
      cmdQ.pop();
    }
    executeCmd(cmd);

    // Prevent busy-waiting
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

void Program::run() {
  Terminal term;
  term.init(0);

  std::thread input(&Program::handleInput, this);
  std::thread cmd(&Program::handleCmd, this);

  for (auto cmd : init()) {
    std::unique_lock<std::mutex> lock(cmdQMutex);
    cmdQ.push(cmd);
    cmdCv.notify_one();
  }

  while (running.load()) {
    Msg msg;
    {
      std::unique_lock<std::mutex> lock(msgQMutex);
      while (msgQ.empty() && running.load()) {
        msgCv.wait(lock);
      }

      if (!running.load() && msgQ.empty()) {
        break;
      }

      msg = msgQ.front();
      msgQ.pop();
    }

    auto result = update(state, msg);
    state = result.newState;
    std::cout << render(state) << std::flush;

    for (auto &cmd : result.commands) {
      std::unique_lock<std::mutex> lock(cmdQMutex);
      cmdQ.push(cmd);
      cmdCv.notify_one();
    }
  }

  input.join();
}
