#include "snip.hpp"
#include "framework/terminal/render.hpp"
#include <string>

using namespace snip;

namespace {

void moveDown(State &state) {
  const int bufferLines = state.buffer.size();
  if (state.cursor.line + 1 >= bufferLines) {
    return;
  }

  state.cursor.line++;
  state.curLine.changeLine(state.buffer[state.cursor.line]);
  if (state.cursor.line >= state.scrollOffset + state.window.height - 1) {
    state.scrollOffset++;
  }
}

void moveUp(State &state) {
  if (state.cursor.line <= 0) {
    return;
  }

  state.cursor.line--;
  state.curLine.changeLine(state.buffer[state.cursor.line]);
  if (state.cursor.line < state.scrollOffset) {
    state.scrollOffset--;
  }
}

void moveLeft(State &state) {
  if (state.curLine.cursorPos > 0) {
    state.curLine.shiftLeft();
  }
}

void moveRight(State &state) {
  if (state.curLine.cursorPos < state.curLine.length()) {
    state.curLine.shiftRight();
  }
}

std::string makeStatus(const State &state) {
  return "Line " + std::to_string(state.cursor.line) + "  Col " +
         std::to_string(state.curLine.cursorPos) + "  Scroll " +
         std::to_string(state.scrollOffset) + "  Size: " +
         std::to_string(state.window.width) + "x" +
         std::to_string(state.window.height) + " Debug: " + state.debugText;
}

} // namespace

std::vector<Cmd> Snip::init() {
  std::vector<Cmd> cmds;
  cmds.push_back(ReadFile("./flake.nix"));
  return cmds;
}

UpdateResult<State> Snip::update(State &currentState, Msg msg) {
  State newState = currentState;
  std::vector<Cmd> cmds;

  if (auto *m = std::any_cast<KeyPressMsg>(&msg)) {
    const char c = m->rune;
    newState.debugText = std::string(1, c);
    switch (c) {
    case 'q':
      cmds.push_back(Quit());
      break;
    case 'j':
      moveDown(newState);
      break;
    case 'k':
      moveUp(newState);
      break;
    case 'h':
      moveLeft(newState);
      break;
    case 'l':
      moveRight(newState);
      break;
    default:
      break;
    }
  }

  else if (auto *m = std::any_cast<WindowSizeMsg>(&msg)) {
    newState.window.width = m->width;
    newState.window.height = m->height;
  }

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

  else if (auto *m = std::any_cast<ErrorMsg>(&msg)) {
    newState.debugText = "Error: " + m->errorMessage;
  }

  else if (auto *m = std::any_cast<IOErrorMsg>(&msg)) {
    newState.debugText = "IOError(" + m->operation + "): " + m->errorMessage;
  }

  else if (auto *m = std::any_cast<FileSavedMsg>(&msg)) {
    newState.debugText = "Saved " + m->filepath + " (" +
                         std::to_string(m->bytesWritten) + " bytes)";
  }

  return {newState, std::move(cmds)};
}

std::string Snip::render(State &state) {
  std::vector<std::string> frameBuffer = state.buffer;
  if (state.cursor.line >= 0 &&
      state.cursor.line < frameBuffer.size()) {
    frameBuffer[state.cursor.line] = state.curLine.string();
  }

  term::FrameSpec spec;
  spec.width = state.window.width;
  spec.height = state.window.height;
  spec.scrollOffset = state.scrollOffset;
  spec.clear = true;
  spec.hideCursor = false;

  term::Cursor cursor;
  cursor.row = (state.cursor.line - state.scrollOffset) + 1;
  cursor.col = state.curLine.cursorPos + 1;

  return term::renderFrame(frameBuffer, makeStatus(state), spec, cursor);
}