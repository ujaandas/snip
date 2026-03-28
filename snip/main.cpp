#include "snip-core/event_loop.hpp"
#include "snip-core/event_source.hpp"
#include "snip-editor/editor.hpp"
#include "snip-runtime/app.hpp"
#include "snip-runtime/input.hpp"
#include "snip-runtime/terminal/terminal.hpp"
#include "snip-ui/renderer.hpp"
#include <signal.h>
#include <unistd.h>

namespace {

class SnipApp final : public snip::runtime::App<snip::editor::State> {
private:
  snip::editor::Editor editor;
  const snip::ui::Renderer &renderer;

public:
  SnipApp(snip::editor::State &state, const snip::ui::Renderer &renderer)
      : snip::runtime::App<snip::editor::State>(state), renderer(renderer) {}

protected:
  std::vector<snip::runtime::Cmd> init() override { return editor.init(); }

  snip::runtime::UpdateResult<snip::editor::State>

  update(snip::editor::State &currentState, snip::runtime::Msg msg) override {
    auto result = editor.update(currentState, std::move(msg));
    return {std::move(result.newState), std::move(result.commands)};
  }

  std::string render(snip::editor::State &currentState) override {
    return renderer.render(editor.viewModel(currentState));
  }
};

} // namespace

int main() {
  const auto session = snip::runtime::term::startSession(false);
  if (!session.valid) {
    return 1;
  }

  snip::editor::State model;

  // Set initial window size
  if (auto size = snip::runtime::term::queryWindowSize(STDOUT_FILENO)) {
    model.window.width = size->width;
    model.window.height = size->height;
  }

  // Initialize app and event loop
  snip::ui::AnsiRenderer renderer;
  SnipApp app(model, renderer);
  snip::core::EventLoop loop;

  // Register STDIN input source
  auto input = snip::core::EventSource::fromFd(STDIN_FILENO);
  input.onReadReady = [&app]() {
    if (auto key = snip::runtime::input::readKeyPress(STDIN_FILENO)) {
      app.post(*key);
    }
  };
  loop.addSource(std::move(input));

  // Register SIGWINCH resize source
  auto resize = snip::core::EventSource::fromSignal(SIGWINCH);
  resize.onReadReady = [&app]() {
    if (auto size = snip::runtime::term::queryWindowSize(STDOUT_FILENO)) {
      app.post(snip::runtime::WindowSizeMsg{size->width, size->height});
    }
  };
  loop.addSource(std::move(resize));

  app.run(loop);
  snip::runtime::term::endSession(session);
  return 0;
}