#include "snip.hpp"
#include "cmd.hpp"
#include "msg.hpp"
#include "terminal/ansi.hpp"
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
      quit();
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

  // Handle background errors
  else if (auto *m = std::any_cast<ErrorMsg>(&msg)) {
    newState.debugText = "Error: " + m->errorMessage;
  }

  return {newState, std::move(cmds)};
}

std::string Snip::render(State &state) {
  std::stringstream out;

  // Clear screen + move cursor to top-left
  out << ansi::CLEAR_SCREEN << ansi::CURSOR_HOME;

  int usableHeight = state.window.height - 1;

  for (int i = 0; i < usableHeight; i++) {
    int lineIndex = state.scrollOffset + i;

    if (lineIndex < state.buffer.size()) {
      std::string line = state.buffer[lineIndex];

      if (lineIndex == state.cursor.line) {
        for (int col = 0; col < line.size(); col++) {
          if (state.curLine.cursorPos == col) {
            // Highlight the cursor block
            out << ansi::REVERSE << line[col] << ansi::RESET;
          } else {
            out << line[col];
          }
        }
        // Highlight trailing space if cursor is at the end of the line
        if (state.curLine.cursorPos == line.size()) {
          out << ansi::REVERSE << " " << ansi::RESET;
        }
      } else {
        out << line;
      }
    }
    out << "\n";
  }

  // Draw the debug/status bar at the bottom
  out << ansi::REVERSE << "Line " << state.cursor.line << "  Col "
      << state.curLine.cursorPos << "  Scroll " << state.scrollOffset
      << "  Size: " << state.window.width << "x" << state.window.height
      << " Debug: " << state.debugText << ansi::RESET;

  out << std::flush;
  return out.str();
}