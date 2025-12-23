#include "message.hpp"
#include "model.hpp"
#include <memory>
#include <queue>

#ifndef PROGRAM_H
#define PROGRAM_H

class Program {
  Model &model;
  // pushing KeypressMessage to Message queue will slice it down to base class
  // thus, we cannot cast it later. instead, we store _pointers_ to Message.
  // therefore, the actual derived objects (ie; Keypress) stay intact.
  // we use a smart ptr instead of raw - owns a heap-obj, auto del on OOS,
  // cannot copy, only mv
  std::queue<std::unique_ptr<Message>> msgQ;
  bool running = false;

public:
  Program(Model &m) : model(m) {}
  void update(Message &msg);
  void render();
  void run();
  template <typename T, typename Base> T *as(Base *msg) {
    return dynamic_cast<T *>(msg);
  }

private:
  void handleInput();
  void handleOutput();
};

#endif // PROGRAM_H