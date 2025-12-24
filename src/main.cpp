#include "../include/program.hpp"
#include <iostream>
#include <sstream>
#include <string>

UpdateResult Program::update(const State &state, Msg &msg) {
  State newState = state;
  std::vector<Cmd> cmds;

  switch (msg.type) {
  case Msg::MsgType::Quit:
    cmds.push_back(Cmd::Quit());
    break;

  case Msg::MsgType::Increment:
    newState.count += msg.inc;
    break;

  case Msg::MsgType::Keypress:
    switch (msg.key) {
    case 'q':
      cmds.push_back(Cmd::Quit());
      break;
    case '+':
      cmds.push_back(Cmd::Send(Msg::Increment(1)));
      break;
    }
    break;
  }

  return UpdateResult{newState, std::move(cmds)};
}

std::string Program::render(const State &state) {
  std::stringstream buf;
  buf << "Count: " << state.count << "\n";
  buf << std::flush;
  return buf.str();
}

int main() {
  State model;
  Program program(model);

  program.run();

  return 0;
}