#include "snip-editor/motion.hpp"
#include "snip-editor/state.hpp"

#include <algorithm>
#include <cctype>

namespace snip::editor {
const CommandTable mk_default_cmds() {
  CommandTable t;

  t.nouns['w'] = [](State& s) {
    if (s.buffer.empty()) {
      return;
    }

    if (s.cursorLine < 0 || s.cursorLine >= static_cast<int>(s.buffer.size())) {
      return;
    }

    const auto& line = s.buffer[static_cast<std::size_t>(s.cursorLine)];

    auto is_word = [](char c) { return std::isalnum(static_cast<unsigned char>(c)) || c == '_'; };

    std::size_t i = std::min<std::size_t>(s.curLine.cursorPos, line.size());

    if (s.action.noun == 'w' && !s.action.sel.empty()) {
      const auto& last = s.action.sel.back();
      if (last.line_start == static_cast<std::size_t>(s.cursorLine)) {
        i = std::min(last.col_end, line.size());
      }
    }

    while (i < line.size() && !is_word(line[i])) {
      i++;
    }

    if (i >= line.size()) {
      s.action.noun = 'w';
      return;
    }

    std::size_t left = i;
    std::size_t right = i;

    while (right < line.size() && is_word(line[right])) {
      ++right;
    }

    s.action.sel.push_back({static_cast<std::size_t>(s.cursorLine), left,
                            static_cast<std::size_t>(s.cursorLine), right, SelMode::Character});
    s.action.noun = 'w';
  };

  return t;
};
} // namespace snip::editor