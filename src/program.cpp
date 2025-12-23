#include "../include/program.hpp"
#include "../include/message.hpp"
#include "../include/terminal.hpp"
#include <thread>

void Program::handleInput() {
  TerminalHelper term;
  term.init(STDIN_FILENO);

  while (this->running) {
    char c;
    if (read(STDIN_FILENO, &c, 1) > 0) {
      msgQ.push(std::make_unique<KeypressMessage>(c));
    }
  }
}

void Program::handleOutput() {
  while (this->running) {
    if (!msgQ.empty()) {
      auto msg = std::move(msgQ.front());
      msgQ.pop();
      this->update(*msg);
      this->render();
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

void Program::run() {
  this->running = true;
  this->render();
  std::thread input(&Program::handleInput, this);
  std::thread output(&Program::handleOutput, this);

  input.join();
  output.join();
}
