#include "../include/program.hpp"
#include "../include/terminal.hpp"
#include <iostream>
#include <thread>

void Program::handleInput() {
  while (running) {
    char c;
    if (read(STDIN_FILENO, &c, 1) > 0) {
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
  term.init(1);

  running = true;
  std::cout << render(state);

  std::thread input(&Program::handleInput, this);

  while (running) {
    if (!msgQ.empty()) {
      auto msg = std::move(msgQ.front());
      msgQ.pop();
      auto result = update(state, msg);
      state = result.newState;
      std::cout << render(state);
      for (auto &cmd : result.commands) {
        execute(cmd);
      }
    }
  }

  input.join();
}
