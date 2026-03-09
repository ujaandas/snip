#include "snip.hpp"
#include "cmd.hpp"
#include "msg.hpp"
#include <sstream>

using namespace snip;

std::vector<Cmd> Snip::init() {
  std::vector<Cmd> cmds;
  cmds.push_back(OpenFile("./flake.nix"));
  return cmds;
}

UpdateResult<State> Snip::update(State &currentState, Msg msg) {
  State newState = currentState;
  std::vector<Cmd> cmds;

  // Check for keypresses
  if (auto *m = std::any_cast<KeyMsg>(&msg)) {
    newState.debugText = m->key;

    switch (m->key) {
    case 'q':
      this->quit();
      break;
    case 'j':
      if (newState.cursor.line + 1 < newState.buffer.size()) {
        newState.cursor.line++;
        newState.curLine.changeLine(newState.buffer[newState.cursor.line]);
        if (newState.cursor.line >=
            newState.scrollOffset + newState.window.height - 1) {
          newState.scrollOffset++;
        }
      }
      break;
    case 'k':
      if (newState.cursor.line > 0) {
        newState.cursor.line--;
        newState.curLine.changeLine(newState.buffer[newState.cursor.line]);
        if (newState.cursor.line < newState.scrollOffset) {
          newState.scrollOffset--;
        }
      }
      break;
    case 'h':
      if (newState.curLine.cursorPos > 0) {
        newState.curLine.shiftLeft();
      }
      break;
    case 'l':
      if (newState.curLine.cursorPos < newState.curLine.length()) {
        newState.curLine.shiftRight();
      }
      break;
    }
  }

  // Check for window resize
  else if (auto *m = std::any_cast<WindowSizeMsg>(&msg)) {
    newState.window.width = m->width;
    newState.window.height = m->height;
  }

  // Check for file loads
  else if (auto *m = std::any_cast<FileLoadedMsg>(&msg)) {
    newState.buffer = std::move(m->lines);
    newState.filename = m->filepath;
    newState.cursor.line = 0;

    if (!newState.buffer.empty()) {
      newState.curLine.changeLine(newState.buffer[0]);
    }
    newState.curLine.cursorPos = 0;
    newState.debugText = "Loaded: " + m->filepath;
  }

  // Handle background file errors
  else if (auto *m = std::any_cast<FileErrorMsg>(&msg)) {
    newState.debugText = "Error: " + m->errorMessage;
  }

  return {newState, std::move(cmds)};
}

std::string Snip::render(State &state) {
  std::stringstream out;

  // Clear screen + move cursor to top-left
  out << "\033[2J\033[H";

  int usableHeight = state.window.height - 1;

  for (int i = 0; i < usableHeight; i++) {
    int lineIndex = state.scrollOffset + i;

    if (lineIndex < state.buffer.size()) {
      std::string line = state.buffer[lineIndex];

      if (lineIndex == state.cursor.line) {
        for (int col = 0; col < line.size(); col++) {
          if (state.curLine.cursorPos == col) {
            out << "\033[7m" << line[col] << "\033[0m";
          } else {
            out << line[col];
          }
        }
        if (state.curLine.cursorPos == line.size()) {
          out << "\033[7m \033[0m";
        }
      } else {
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