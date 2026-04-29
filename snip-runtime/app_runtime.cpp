#include "app_runtime.hpp"

#include <utility>

namespace snip::runtime {

SnipRuntime::SnipRuntime(editor::State& s) : Runtime(s) {}

std::vector<Cmd> SnipRuntime::init() { return editor.init(); }

UpdateResult SnipRuntime::update(const editor::State& currentState, Msg msg) {
  auto result = editor.update(currentState, std::move(msg));
  return {std::move(result.newState), std::move(result.commands)};
}

std::string SnipRuntime::render(const editor::State& currentState) {
  const auto frame = renderer.render(editor.viewModel(currentState));
  return translator.translate(frame);
}

}  // namespace snip::runtime
