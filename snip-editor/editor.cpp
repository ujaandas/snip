#include "editor.hpp"
#include "snip-editor/view_model.hpp"

#include <cctype>
#include <string>
#include <utility>

namespace snip::editor {
namespace {

void moveDown(State& state) {
  const int bufferLines = state.buffer.size();
  if (state.cursorLine + 1 >= bufferLines) {
    return;
  }

  state.cursorLine++;
  state.curLine.changeLine(state.buffer[state.cursorLine]);
  if (state.cursorLine >= state.scrollOffset + state.window.height - 1) {
    state.scrollOffset++;
  }
}

void moveUp(State& state) {
  if (state.cursorLine <= 0) {
    return;
  }

  state.cursorLine--;
  state.curLine.changeLine(state.buffer[state.cursorLine]);
  if (state.cursorLine < state.scrollOffset) {
    state.scrollOffset--;
  }
}

void moveLeft(State& state) {
  if (state.curLine.cursorPos > 0) {
    state.curLine.shiftLeft();
  }
}

void moveRight(State& state) {
  if (state.curLine.cursorPos < state.curLine.length()) {
    state.curLine.shiftRight();
  }
}

std::string makeStatus(const State& state) {
  return "Line " + std::to_string(state.cursorLine) + "  Col " +
         std::to_string(state.curLine.cursorPos) + "  Scroll " +
         std::to_string(state.scrollOffset) + "  Size: " + std::to_string(state.window.width) +
         "x" + std::to_string(state.window.height) + "  " + state.statusText;
}

} // namespace

std::vector<runtime::Cmd> Editor::init() const {
  return {runtime::ReadFile("./flake.nix")};
}

UpdateResult Editor::update(const State& currentState, runtime::Msg msg) const {
  State newState = currentState;
  std::vector<runtime::Cmd> cmds;

  if (auto* m = std::get_if<runtime::KeyPressMsg>(&msg)) {
    const char c = m->rune;

    newState.statusText = "Key: " + std::string(1, c) + " (" + std::to_string(c) + ")";

    // If digit, multiply verb
    if (std::isdigit(static_cast<unsigned char>(c))) {
      newState.action.count = newState.action.count * 10 + (c - '0');
      return {newState, std::move(cmds)};
    }

    if (auto it = ct.nouns.find(c); it != ct.nouns.end()) {
      int n = std::max(1, newState.action.count);
      newState.action.count = 0;

      for (int i = 0; i < n; ++i) {
        it->second(newState);
      }

      return {newState, std::move(cmds)};
    }

    if (auto it = ct.verbs.find(c); it != ct.verbs.end()) {
      int n = std::max(1, newState.action.count);
      newState.action.count = 0;

      for (int i = 0; i < n; ++i) {
        it->second(newState);
      }

      return {newState, std::move(cmds)};
    }

    switch (c) {
    case 'q':
      cmds.push_back(runtime::Quit());
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
  } else if (auto* m = std::get_if<runtime::WindowSizeMsg>(&msg)) {
    if (m->width > 0) {
      newState.window.width = m->width;
    }
    if (m->height > 0) {
      newState.window.height = m->height;
    }
  } else if (auto* m = std::get_if<runtime::FileLoadedMsg>(&msg)) {
    newState.buffer = std::move(m->lines);
    newState.filename = m->filepath;
    newState.cursorLine = 0;

    if (!newState.buffer.empty()) {
      newState.curLine.cursorPos = 0;
      newState.curLine.changeLine(newState.buffer[0]);
    }

    newState.statusText = "Loaded: " + m->filepath;
  } else if (auto* m = std::get_if<runtime::IOErrorMsg>(&msg)) {
    newState.statusText = "IOError(" + m->operation + "): " + m->errorMessage;
  } else if (auto* m = std::get_if<runtime::FileSavedMsg>(&msg)) {
    newState.statusText =
        "Saved " + m->filepath + " (" + std::to_string(m->bytesWritten) + " bytes)";
  }

  return {std::move(newState), std::move(cmds)};
}

ViewModel Editor::viewModel(const State& state) const {
  ViewModel vm;

  vm.lines = state.buffer;
  vm.width = state.window.width;
  vm.height = state.window.height;
  vm.scrollOffset = state.scrollOffset;
  vm.cursor.hidden = false;
  vm.clear = true;

  if (state.cursorLine >= 0 && state.cursorLine < vm.lines.size()) {
    vm.lines[state.cursorLine] = state.curLine.string();
  }

  // Build selections
  vm.selections.clear();
  for (const auto& sel : state.action.sel) {
    vm.selections.push_back(SelectionVm{static_cast<int>(sel.line_start),
                                        static_cast<int>(sel.col_start),
                                        static_cast<int>(sel.col_end)});
  }

  vm.cursor.row = (state.cursorLine - state.scrollOffset) + 1;
  vm.cursor.col = state.curLine.cursorPos + 1;
  vm.statusText = makeStatus(state);

  return vm;
}

} // namespace snip::editor