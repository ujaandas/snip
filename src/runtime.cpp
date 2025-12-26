#include "../include/program.hpp"
#include "../include/terminal.hpp"
#include <iostream>
#include <thread>

void Program::handleInput() {
  while (running) {
    char c;
    if (read(STDIN_FILENO, &c, 1) > 0) {
      std::lock_guard<std::mutex> lock(qMutex);
      msgQ.push(Msg::Keypress(c));
    }
  }
}

void Program::execute(const Cmd &cmd) {
  switch (cmd.type) {
  case Cmd::CmdType::SendMessage:
    msgQ.push(cmd.msg);
    break;

  case Cmd::CmdType::Quit:
    running = false;
    break;
  }
}

void Program::run() {
  Terminal term;
  term.init(0);

  running = true;
  std::cout << render(state);
  std::thread input(&Program::handleInput, this);

  while (running) {
    bool hadMessage = false;
    Msg msg;
    {
      std::lock_guard<std::mutex> lock(qMutex);
      if (!msgQ.empty()) {
        hadMessage = true;
        msg = msgQ.front();
        msgQ.pop();
      }
    }

    if (hadMessage) {
      auto result = update(state, msg);
      state = result.newState;
      std::cout << render(state);
      for (auto &cmd : result.commands)
        execute(cmd);
    } else {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
  }

  input.join();
}
