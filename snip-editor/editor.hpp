#pragma once

#include "snip-editor/state.hpp"
#include "snip-editor/view_model.hpp"
#include "snip-runtime/cmd.hpp"
#include "snip-runtime/msg.hpp"

namespace snip::editor {

struct UpdateResult {
  State newState;
  std::vector<runtime::Cmd> commands;
};

class Editor {
public:
  std::vector<runtime::Cmd> init() const;
  UpdateResult update(const State& currentState, runtime::Msg msg) const;
  ViewModel viewModel(const State& state) const;
};

} // namespace snip::editor