#include "app.hpp"

#include <utility>

namespace snip::runtime {

App::App(editor::State& state) : Runtime(state) {}

std::vector<Cmd> App::init() { return editor.init(); }

UpdateResult App::update(const editor::State& currentState, Msg msg) {
  auto result = editor.update(currentState, std::move(msg));
  return {std::move(result.newState), std::move(result.commands)};
}

std::string App::render(const editor::State& currentState) {
  const auto frame = renderer.render(editor.viewModel(currentState));
  return translator.translate(frame);
}

}  // namespace snip::runtime
