#include "../include/program.hpp"
#include <iostream>

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

void Program::update(const Message &msg) {
  CounterModel &m = static_cast<CounterModel &>(this->model);

  if (auto inc = dynamic_cast<const IncrementMessage *>(&msg)) {
    m.count += inc->amount;
  } else if (auto quit = dynamic_cast<const QuitMessage *>(&msg)) {
    std::cout << "Quitting... Reason: " << quit->reason << "\n";
  } else if (auto key = dynamic_cast<const KeypressMessage *>(&msg)) {
    std::cout << "Pressed: " << key->key << std::endl << std::flush;
    std::cout << "Count: " << m.count << std::endl << std::flush;
  } else if (auto key = dynamic_cast<const KeypressMessage *>(&msg)) {
    std::cout << "Pressed: " << key->key << std::endl;
    if (key->key == 'q') {
      this->running = false;
      msgQ.push(std::make_unique<QuitMessage>());
    }
  }
}

void Program::render() {
  const CounterModel &m = static_cast<const CounterModel &>(this->model);
  std::cout << "Count: " << m.count << std::endl;
}

int main() {
  CounterModel model;
  Program program(model, true);

  program.run();

  return 0;
}