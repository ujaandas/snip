#include "cmd.hpp"
#include "message.hpp"
#include "state.hpp"
#include <queue>

#ifndef PROGRAM_H
#define PROGRAM_H

struct UpdateResult {
  State newState;
  std::vector<Cmd> commands;
};

class Program {
  State &model;
  std::queue<Msg> msgQ;
  bool running = false;

public:
  Program(State &m) : model(m) {}
  UpdateResult update(const State &state, Msg &msg);
  void render();
  void run();
  void execute(const Cmd &cmd);
  template <typename T, typename Base> T *as(Base *msg) {
    return dynamic_cast<T *>(msg);
  }

private:
  void handleInput();
  void handleOutput();
};

#endif // PROGRAM_H