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
#include <variant>
#include <vector>

std::vector<Cmd> Program::init() {
  // Batch commands
  std::vector<Cmd> cmds;

  // Check initial window size
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

  // Read input file
  File f = File("./Makefile");
  for (auto line : f.readRange(0, w.ws_row)) {
    std::cout << line << "\n";
  }

  return cmds;
};

UpdateResult Program::update(const State &state, Msg &msg) {
  State newState = state;
  std::vector<Cmd> cmds;

  std::visit(
      [&cmds, &newState](auto &&m) {
        using T = std::decay_t<decltype(m)>;
        if constexpr (std::is_same_v<T, QuitMsg>) {
          cmds.push_back(QuitCmd{});
        }

        else if constexpr (std::is_same_v<T, KeypressMsg>) {
          switch (m.key) {
          case 'q':
            cmds.push_back(QuitCmd{});
            break;
          case '+':
            cmds.push_back(SendMessageCmd{(KeypressMsg{'+'})});
            break;
          }
        }

        else if constexpr (std::is_same_v<T, WindowDimensionsMsg>) {
          newState.window.width = m.width;
          newState.window.height = m.height;
        }

        else if constexpr (std::is_same_v<T, FilepathMsg>) {
          cmds.push_back(OpenFileCmd{m.path});
        }
      },
      msg);

  return UpdateResult{newState, std::move(cmds)};
}

std::string Program::render(const State &state) {
  std::stringstream output;
  output << "Count: " << state.count << "\n";
  output << state.window.height << " x " << state.window.width << "\n";
  output << std::flush;
  return output.str();
}

int main() {
  State model;
  Program program(model);

  program.run();

  return 0;
}