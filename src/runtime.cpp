#include "../include/program.hpp"
#include "../include/terminal.hpp"
#include <iostream>
#include <mutex>
#include <thread>

void Program::handleInput() {
  while (running.load()) {
    char c;
    if (read(STDIN_FILENO, &c, 1) > 0) {
      std::unique_lock<std::mutex> lock(qMutex);
      msgQ.push(Msg::Keypress(c));
      cv.notify_one();
    }
  }
}

void Program::execute(const Cmd &cmd) {
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
  execute(init(state));

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
      execute(cmd);
    }
  }

  input.join();
}
