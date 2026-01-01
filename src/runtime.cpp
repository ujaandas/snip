#include "../include/program.hpp"
#include "../include/terminal.hpp"
#include "message.hpp"
#include <iostream>
#include <mutex>
#include <sys/ioctl.h>
#include <thread>

void Program::handleInput() {
  while (running.load()) {
    // Read keyboard input
    char c;
    if (read(STDIN_FILENO, &c, 1) > 0) {
      std::unique_lock<std::mutex> lock(qMutex);
      // Should I return a command here instead?
      msgQ.push(Msg::Keypress(c));
      cv.notify_one();
    }

    // Check window size
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    if (w.ws_col != state.window.width || w.ws_row != state.window.height) {
      std::unique_lock<std::mutex> lock(qMutex);
      msgQ.push(Msg::WindowDimensions(w.ws_col, w.ws_row));
      cv.notify_one();
    }

    // Prevent busy-waiting
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

void Program::addJob(const Cmd &cmd) {
  switch (cmd.type) {
  case Cmd::CmdType::SendMessage: {
    std::unique_lock<std::mutex> lock(qMutex);
    msgQ.push(cmd.msg);
    cv.notify_one();
    break;
  }
  case Cmd::CmdType::Quit:
    running.store(false);
    cv.notify_all();
    break;
  case Cmd::CmdType::None:
    break;
  }
}

void Program::run() {
  Terminal term;
  term.init(0);

  std::thread input(&Program::handleInput, this);

  for (auto cmd : init()) {
    addJob(cmd);
  }

  while (running) {
    Msg msg;
    {
      std::unique_lock<std::mutex> lock(qMutex);
      while (msgQ.empty() && running.load()) {
        cv.wait(lock);
      }

      if (!running.load() && msgQ.empty()) {
        break;
      }

      msg = msgQ.front();
      msgQ.pop();
    }

    auto result = update(state, msg);
    state = result.newState;
    std::cout << render(state);

    for (auto &cmd : result.commands) {
      addJob(cmd);
    }
  }

  input.join();
}
