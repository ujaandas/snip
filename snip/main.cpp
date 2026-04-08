#include "snip-core/event_loop.hpp"
#include "snip-core/event_source.hpp"
#include "snip-editor/editor.hpp"
#include "snip-render/renderer.hpp"
#include "snip-render/translator.hpp"
#include "snip-runtime/app.hpp"
#include "snip-runtime/input.hpp"
#include "snip-term/terminal.hpp"
#include <signal.h>
#include <unistd.h>

namespace {

class SnipApp final : public snip::runtime::App<snip::editor::State> {
private:
  snip::editor::Editor editor;
  snip::render::Renderer renderer;
  snip::render::AnsiTranslator translator;

public:
  explicit SnipApp(snip::editor::State& state) : snip::runtime::App<snip::editor::State>(state) {}

protected:
  std::vector<snip::runtime::Cmd> init() override {
    return editor.init();
  }

  snip::runtime::UpdateResult<snip::editor::State>

  update(snip::editor::State& currentState, snip::runtime::Msg msg) override {
    auto result = editor.update(currentState, std::move(msg));
    return {std::move(result.newState), std::move(result.commands)};
  }

  std::string render(snip::editor::State& currentState) override {
    const auto frame = renderer.render(editor.viewModel(currentState));
    return translator.translate(frame);
  }
};

} // namespace

int main() {
  const auto session = snip::term::startSession(false);
  if (!session.valid) {
    return 1;
  }

  snip::editor::State model;

  // Set initial window size
  if (auto size = snip::term::queryWindowSize(STDOUT_FILENO)) {
    model.window.width = size->width;
    model.window.height = size->height;
  }

  // Initialize app and event loop
  SnipApp app(model);
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
    if (auto size = snip::term::queryWindowSize(STDOUT_FILENO)) {
      app.post(snip::runtime::WindowSizeMsg{size->width, size->height});
    }
  };
  loop.addSource(std::move(resize));

  app.run(loop);
  snip::term::endSession(session);
  return 0;
}