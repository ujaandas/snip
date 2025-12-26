#include "cmd.hpp"
#include "message.hpp"
#include "state.hpp"
#include <queue>

/*
Program is the heart of the app's functionality, and can be split into 2 clear
sections. The event handler (update) and renderer control how given inputs are
handled, and what they are, as well as how to actually display them. These 2 are
intended to be completely pure functions. The runtime, which should be touched
very minimally, defines how to execute our deferred commands, input handling,
and actually calling the event handler and renderer.
*/

#ifndef PROGRAM_H
#define PROGRAM_H

// Returned by event loop - a list of commands to execute, and new state.
struct UpdateResult {
  State newState;
  std::vector<Cmd> commands;
};

// Defines input/output handling and core runtime.
class Program {
  State &state;
  std::mutex qMutex;
  std::queue<Msg> msgQ;
  bool running = false;

public:
  Program(State &m) : state(m) {}
  UpdateResult update(const State &state, Msg &msg);
  std::string render(const State &state);
  void run();
  void execute(const Cmd &cmd);

private:
  void handleInput();
  void handleOutput();
};

#endif // PROGRAM_H