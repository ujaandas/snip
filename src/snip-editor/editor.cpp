#include "editor.hpp"

#include <string>
#include <utility>

namespace snip::editor {
namespace {

void moveDown(State &state) {
  const int bufferLines = static_cast<int>(state.buffer.size());
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
         std::to_string(state.scrollOffset) +
         "  Size: " + std::to_string(state.window.width) + "x" +
         std::to_string(state.window.height) + "  " + state.statusText;
}

} // namespace

std::vector<runtime::Cmd> Editor::init() const {
  return {runtime::ReadFile("./flake.nix")};
}

UpdateResult Editor::update(const State &currentState, runtime::Msg msg) const {
  State newState = currentState;
  std::vector<runtime::Cmd> commands;

  if (auto *m = std::get_if<runtime::KeyPressMsg>(&msg)) {
    const char c = m->rune;
    newState.statusText = std::string("Key: ") + c;

    switch (c) {
    case 'q':
      commands.push_back(runtime::Quit());
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
  } else if (auto *m = std::get_if<runtime::WindowSizeMsg>(&msg)) {
    newState.window.width = m->width;
    newState.window.height = m->height;
  } else if (auto *m = std::get_if<runtime::FileLoadedMsg>(&msg)) {
    newState.buffer = std::move(m->lines);
    newState.filename = m->filepath;
    newState.cursor.line = 0;

    if (!newState.buffer.empty()) {
      newState.curLine.cursorPos = 0;
      newState.curLine.changeLine(newState.buffer[0]);
    }

    newState.statusText = "Loaded: " + m->filepath;
  } else if (auto *m = std::get_if<runtime::IOErrorMsg>(&msg)) {
    newState.statusText = "IOError(" + m->operation + "): " + m->errorMessage;
  } else if (auto *m = std::get_if<runtime::FileSavedMsg>(&msg)) {
    newState.statusText = "Saved " + m->filepath + " (" +
                          std::to_string(m->bytesWritten) + " bytes)";
  }

  return {std::move(newState), std::move(commands)};
}

ViewModel Editor::viewModel(const State &state) const {
  ViewModel vm;
  vm.lines = state.buffer;
  vm.width = state.window.width;
  vm.height = state.window.height;
  vm.scrollOffset = state.scrollOffset;
  vm.hideCursor = false;
  vm.clear = true;

  if (state.cursor.line >= 0 &&
      state.cursor.line < static_cast<int>(vm.lines.size())) {
    vm.lines[state.cursor.line] = state.curLine.string();
  }

  vm.cursor.row = (state.cursor.line - state.scrollOffset) + 1;
  vm.cursor.col = static_cast<int>(state.curLine.cursorPos) + 1;
  vm.statusText = makeStatus(state);

  return vm;
}

} // namespace snip::editor