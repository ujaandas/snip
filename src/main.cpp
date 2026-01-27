#include "../include/file.hpp"
#include "../include/program.hpp"
#include "message.hpp"
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
  File f = File("./flake.nix");
  state.buffer = f.readRange(0, 14);

  return cmds;
};

UpdateResult Program::update(const State &state, Msg &msg) {
  State newState = state;
  std::vector<Cmd> cmds;

  std::visit(
      [&cmds, &newState, this](auto &&m) {
        using T = std::decay_t<decltype(m)>;

        // Quit program
        if constexpr (std::is_same_v<T, QuitMsg>) {
          cmds.push_back(Quit(*this));
        }

        // Check keypresses
        else if constexpr (std::is_same_v<T, KeypressMsg>) {
          switch (m.key) {
          case 'q':
            cmds.push_back(Send(QuitMsg{}));
            break;

          // Go up
          case 'j':
            if (newState.cursorLine + 1 < newState.buffer.size()) {
              newState.cursorLine++;
            }

            // Scroll if cursor goes off screen
            if (newState.cursorLine >=
                newState.scrollOffset + newState.window.height - 1) {
              newState.scrollOffset++;
            }
            break;

          // Go down
          case 'k':
            if (newState.cursorLine > 0) {
              newState.cursorLine--;
            }

            // Scroll up
            if (newState.cursorLine < newState.scrollOffset) {
              newState.scrollOffset--;
            }
            break;

          // Go right
          case 'h':
            if (newState.cursorCol > 0) {
              newState.cursorCol--;
            }
            break;

          // Go left
          case 'l': {
            int lineLen = 0;
            if (newState.cursorLine < newState.buffer.size())
              lineLen = newState.buffer[newState.cursorLine].size();

            if (newState.cursorCol < lineLen) {
              newState.cursorCol++;
            } else {
              newState.cursorCol = lineLen;
            }
            break;
          }
          }
        }

        // Window size changed
        else if constexpr (std::is_same_v<T, WindowDimensionsMsg>) {
          newState.window.width = m.width;
          newState.window.height = m.height;
        }

        // File opened
        else if constexpr (std::is_same_v<T, FilepathMsg>) {
          cmds.push_back(OpenFile(m.path));
        }
      },
      msg);

  return UpdateResult{newState, std::move(cmds)};
}

std::string Program::render(const State &state) {
  std::stringstream out;

  // Clear screen + move cursor to top-left
  out << "\033[2J\033[H";

  int usableHeight = state.window.height - 1; // Leave room for debug bar

  for (int i = 0; i < usableHeight; i++) {
    int lineIndex = state.scrollOffset + i;

    if (lineIndex < state.buffer.size()) {
      const std::string &line = state.buffer[lineIndex];

      if (lineIndex == state.cursorLine) {
        // Render line with cursor highlight
        for (int col = 0; col < line.size(); col++) {
          if (col == state.cursorCol) {
            out << "\033[7m" << line[col] << "\033[0m";
          } else {
            out << line[col];
          }
        }

        // If cursor is at end of line, draw a highlighted space
        if (state.cursorCol == line.size()) {
          out << "\033[7m \033[0m";
        }
      } else {
        // Normal line
        out << line;
      }
    }

    out << "\n";
  }

  out << "\033[7m"; // Reverse video
  out << "Line " << state.cursorLine << "  Scroll " << state.scrollOffset
      << "  Size " << state.window.width << "x" << state.window.height;
  out << "\033[0m"; // Reset formatting

  out << std::flush;
  return out.str();
}

int main() {
  State model;
  Program program(model);

  program.run();

  return 0;
}