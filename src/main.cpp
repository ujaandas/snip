#include "../include/program.hpp"
#include <iostream>

UpdateResult Program::update(const State &state, Msg &msg) {
  State newState = state;
  std::vector<Cmd> cmds;

  switch (msg.type) {
  case Msg::MsgType::None:
    break;
  case Msg::MsgType::Quit:
    cmds.push_back(Cmd::Quit());
    break;
  case Msg::MsgType::Increment:
    newState.count += msg.payload.inc;
    break;
  case Msg::MsgType::Keypress:
    if (msg.payload.key == 'q') {
      cmds.push_back(Cmd::Quit());
    } else if (msg.payload.key == '+') {
      cmds.push_back(Cmd::Send(Msg{
          Msg::MsgType::Increment,
      }));
    }
    break;
  }

  return UpdateResult{newState, std::move(cmds)};
}

void Program::render() {
  const State &m = static_cast<const State &>(this->model);
  std::cout << "Count: " << m.count << "\n";
  std::cout << std::flush;
}

int main() {
  State model;
  Program program(model);

  program.run();

  return 0;
}