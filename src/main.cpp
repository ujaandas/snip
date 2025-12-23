#include "../include/program.hpp"
#include <iostream>
#include <memory>

struct CounterModel : Model {
  int count = 0;
};

struct IncrementMessage : Message {
  int amount;
  IncrementMessage(int amt) : amount(amt) {}
};

struct QuitMessage : Message {
  std::string reason;
  QuitMessage(std::string r = "User requested quit") : reason(std::move(r)) {}
};

void Program::update(Message &msg) {
  CounterModel &m = static_cast<CounterModel &>(this->model);

  if (auto inc = as<IncrementMessage>(&msg)) {
    m.count += inc->amount;
  } else if (auto quit = as<QuitMessage>(&msg)) {
    this->running = false;
  } else if (auto key = as<KeypressMessage>(&msg)) {
    if (key->key == 'q') {
      msgQ.push(std::make_unique<QuitMessage>());
    } else if (key->key == '+') {
      this->msgQ.push(std::make_unique<IncrementMessage>(1));
    }
  }
}

void Program::render() {
  const CounterModel &m = static_cast<const CounterModel &>(this->model);
  std::cout << "Count: " << m.count << "\n";
  std::cout << std::flush;
}

int main() {
  CounterModel model;
  Program program(model);

  program.run();

  return 0;
}