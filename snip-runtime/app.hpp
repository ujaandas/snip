#pragma once

#include "runtime.hpp"
#include "snip-editor/editor.hpp"
#include "snip-render/renderer.hpp"
#include "snip-render/translator.hpp"

namespace snip::runtime {

class App final : public Runtime {
 private:
  snip::editor::Editor editor;
  snip::render::Renderer renderer;
  snip::render::AnsiTranslator translator;

 public:
  explicit App(editor::State& state);

 protected:
  std::vector<Cmd> init() override;
  UpdateResult update(const editor::State& currentState, Msg msg) override;
  std::string render(const editor::State& currentState) override;
};

}  // namespace snip::runtime
