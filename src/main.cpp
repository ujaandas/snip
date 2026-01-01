#include "../include/file.hpp"
#include "../include/program.hpp"
#include <cstddef>
#include <iostream>
#include <ostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <vector>

std::vector<Cmd> Program::init() {
  // Batch commands
  std::vector<Cmd> cmds;

  // Get window dimensions
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  cmds.push_back(Cmd::Send(Msg::WindowDimensions(w.ws_col, w.ws_row)));

  // Read input file
  File f = File("./Makefile");
  for (auto line : f.readRange(0, 20)) {
    std::cout << line << "\n";
  }

  return cmds;
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

  case Msg::MsgType::WindowDimensions:
    newState.window.width = msg.x;
    newState.window.height = msg.y;
    break;
  }

  return UpdateResult{newState, std::move(cmds)};
}

std::string Program::render(const State &state) {
  std::stringstream buf;
  buf << "Count: " << state.count << "\n";
  buf << state.window.height << " x " << state.window.width << "\n";
  buf << std::flush;
  return buf.str();
}

int main() {
  State model;
  Program program(model);

  program.run();

  return 0;
}