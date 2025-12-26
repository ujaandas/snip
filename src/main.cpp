#include "../include/program.hpp"
#include <cstddef>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>

Cmd Program::init(const State &state) {
  std::cout << "allo" << std::endl;
  // return Cmd::Send(Msg::Write("hi there!"));
  return Cmd::None();
};

UpdateResult Program::update(const State &state, Msg &msg) {
  State newState = state;
  std::vector<Cmd> cmds;

  switch (msg.type) {
  case Msg::MsgType::Quit:
    cmds.push_back(Cmd::Quit());
    break;

  case Msg::MsgType::Integer:
    newState.count += msg.i;
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
  case Msg::MsgType::Text:
    newState.text = msg.text;
    break;
  }

  return UpdateResult{newState, std::move(cmds)};
}

std::string Program::render(const State &state) {
  std::stringstream buf;
  buf << "Count: " << state.count << "\n";
  buf << state.text << "\n";
  buf << std::flush;
  return buf.str();
}

int main() {
  State model;
  Program program(model);

  program.run();

  return 0;
}