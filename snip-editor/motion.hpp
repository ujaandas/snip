#pragma once
#include <functional>
#include <optional>
#include <unordered_map>

namespace snip::editor {

struct State;

enum class SelMode { Character, Line };

struct Selection {
  std::size_t line_start, col_start, line_end, col_end;
  SelMode mode = SelMode::Character;
};

struct Action {
  std::vector<Selection> sel;
  std::optional<char> noun;
  int count = 0;
};

// Produces/updates selections
using Noun = std::function<void(State&)>;

// Consumes selections and edits buffer
using Verb = std::function<void(State&)>;

struct CommandTable {
  std::unordered_map<char, Noun> nouns;
  std::unordered_map<char, Verb> verbs;
};

const CommandTable mk_default_cmds();

} // namespace snip::editor