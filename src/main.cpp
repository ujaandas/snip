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
          newState.debugText = m.key;
          switch (m.key) {
          case 'q':
            cmds.push_back(Send(QuitMsg{}));
            break;

          // Move cursor down
          case 'j': {
            // Are we moving below the file length?
            if (newState.cursor.line + 1 < newState.buffer.size()) {
              // Move cursor down
              newState.cursor.line++;

              // Clamp column to new line length
              if (newState.curLine.cursorPos >=
                  newState.buffer[newState.cursor.line].size()) {
                newState.curLine.cursorPos =
                    newState.buffer[newState.cursor.line].size();
              }

              // Adjust the current line pointer
              newState.curLine.changeLine(
                  newState.buffer[newState.cursor.line]);

              // Scroll down if cursor goes below window
              if (newState.cursor.line >=
                  newState.scrollOffset + newState.window.height - 1) {
                newState.scrollOffset++;
              }
            }

            break;
          }

            // Move cursor up
          case 'k': {
            // Are we NOT at the start of the file?
            if (newState.cursor.line > 0) {
              // Move cursor up
              newState.cursor.line--;

              // Adjust the current line pointer
              newState.curLine.changeLine(
                  newState.buffer[newState.cursor.line]);

              // Clamp column to new line length
              if (newState.curLine.cursorPos > newState.curLine.length()) {
                newState.curLine.cursorPos = newState.curLine.length();
              }

              // Scroll up if cursor goes above window
              if (newState.cursor.line < newState.scrollOffset) {
                newState.scrollOffset--;
              }
            }
            break;
          }

          // Move cursor left
          case 'h': {
            if (newState.curLine.cursorPos > 0) {
              newState.curLine.shiftLeft();
              newState.curLine.cursorPos--;
            }
            break;
          }

          // Move cursor right
          case 'l': {
            if (newState.curLine.cursorPos < newState.curLine.length()) {
              newState.curLine.shiftRight();
              newState.curLine.cursorPos++;
            } else {
              while (newState.curLine.shiftRight()) {
                newState.curLine.cursorPos = newState.curLine.length();
              }
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
          newState.cursor.line = 0;
          newState.curLine.changeLine(newState.buffer[0]);
          newState.curLine.cursorPos = 0;
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
      std::string line = state.buffer[lineIndex];

      if (lineIndex == state.cursor.line) {
        // Iterate over columns
        for (int col = 0; col < line.size(); col++) {
          // Render line with cursor highlight
          if (state.curLine.cursorPos == col) {
            out << "\033[7m" << line[col] << "\033[0m";
          } else {
            out << line[col];
          }
        }

        // If cursor is at end of line, draw a highlighted space
        if (state.curLine.cursorPos == line.size()) {
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
  out << "Line " << state.cursor.line << "  Col " << state.curLine.cursorPos
      << "  Scroll " << state.scrollOffset << "  Size: " << state.window.width
      << "x" << state.window.height;
  out << " Debug: " << state.debugText;
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