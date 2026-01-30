#include "../include/file.hpp"
#include "../include/program.hpp"
#include "message.hpp"
#include <climits>
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

  // Read input file
  cmds.push_back(OpenFile("./flake.nix"));

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

            // Move cursor down
          case 'j': {
            if (newState.cursorLine + 1 < newState.buffer.size()) {
              newState.cursorLine++;
            }

            // Clamp cursorCol to new line length
            int lineLen = newState.buffer[newState.cursorLine].string().size();
            newState.cursorCol = std::min(newState.cursorCol, lineLen);

            // Scroll down if cursor goes below window
            if (newState.cursorLine >=
                newState.scrollOffset + newState.window.height - 1) {
              newState.scrollOffset++;
            }

            break;
          }

          // Move cursor up
          case 'k': {
            if (newState.cursorLine > 0) {
              newState.cursorLine--;
            }

            // Clamp cursorCol to new line length
            int lineLen = newState.buffer[newState.cursorLine].string().size();
            newState.cursorCol = std::min(newState.cursorCol, lineLen);

            // Scroll up if cursor goes above window
            if (newState.cursorLine < newState.scrollOffset) {
              newState.scrollOffset--;
            }
            break;
          }

          // Move cursor left
          case 'h': {
            if (newState.cursorCol > 0) {
              newState.cursorCol--;
            }
            break;
          }
          // Move cursor right
          case 'l': {
            int lineLen = newState.buffer[newState.cursorLine].string().size();

            if (newState.cursorCol < lineLen) {
              newState.cursorCol++;
            } else {
              // Clamp to end of line
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
          newState.buffer = File(m.path).readRange(0, INT_MAX);
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

  // Iterate over lines
  for (int i = 0; i < usableHeight; i++) {
    int lineIndex = state.scrollOffset + i;

    if (lineIndex < state.buffer.size()) {
      GapBufferedLine line = state.buffer[lineIndex];

      if (lineIndex == state.cursorLine) {
        // Iterate over columns
        for (int col = 0; col < line.string().size(); col++) {
          // Render line with cursor highlight
          if (col == state.cursorCol) {
            out << "\033[7m" << line.string()[col] << "\033[0m";
          } else {
            out << line.string()[col];
          }
        }

        // If cursor is at end of line, draw a highlighted space
        if (state.cursorCol == line.string().size()) {
          out << "\033[7m \033[0m";
        }
      } else {
        // Normal line
        out << line.string();
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