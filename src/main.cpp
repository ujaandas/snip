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
    std::cout << "Count: " << m.count << std::endl << std::flush;
  } else if (auto quit = as<QuitMessage>(&msg)) {
    std::cout << "Quitting... Reason: " << quit->reason << "\n";
  } else if (auto key = as<KeypressMessage>(&msg)) {
    std::cout << "Pressed: " << key->key << std::endl << std::flush;
    if (key->key == 'q') {
      this->running = false;
      msgQ.push(std::make_unique<QuitMessage>());
    } else if (key->key == '+') {
      this->msgQ.push(std::make_unique<IncrementMessage>(1));
    }
  }
}

void Program::render() {
  // const CounterModel &m = static_cast<const CounterModel &>(this->model);
  // std::cout << "Count: " << m.count << std::endl;
}

int main() {
  CounterModel model;
  Program program(model);

  program.run();

  return 0;
}