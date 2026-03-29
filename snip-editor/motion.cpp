#include "snip-editor/motion.hpp"
#include "snip-editor/state.hpp"

namespace snip::editor {
CommandTable mk_default_cmds() {
  CommandTable t;

  t.nouns['w'] = [](State& s) {
    if (s.buffer.empty()) {
      return;
    }

    const auto& line = s.buffer[s.cursor.line];
    std::size_t i = s.curLine.cursorPos;

    auto is_word = [](char c) { return std::isalnum(static_cast<unsigned char>(c)) || c == ' '; };

    std::size_t left = i, right = i;

    while (left > 0 && is_word(line[left - 1])) {
      --left;
    }

    while (right < line.size() && is_word(line[right])) {
      ++right;
    }

    s.action.sel = {{static_cast<std::size_t>(s.cursor.line), left,
                     static_cast<std::size_t>(s.cursor.line), right, SelMode::Character}};
    s.action.noun = 'w';
  };

  return t;
};
} // namespace snip::editor