#pragma once

#include "snip-editor/motion.hpp"
#include "snip-editor/state.hpp"
#include "snip-editor/view_model.hpp"
#include "snip-runtime/cmd.hpp"
#include "snip-runtime/msg.hpp"

namespace snip::editor {

struct UpdateResult {
  State newState;
  std::vector<runtime::Cmd> commands;
};

/*
  Editor is responsible for managing the editor state and implementing the core editing logic. It
  receives messages from the runtime, updates the state accordingly, and produces a new ViewModel
  for rendering. It also produces commands for side effects like file I/O.
*/
class Editor {
private:
  CommandTable ct = mk_default_cmds();

public:
  std::vector<runtime::Cmd> init() const;
  UpdateResult update(const State& currentState, runtime::Msg msg) const;
  ViewModel viewModel(const State& state) const;
};

} // namespace snip::editor