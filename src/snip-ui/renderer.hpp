#pragma once

#include "snip-editor/view_model.hpp"
#include <string>

namespace snip::ui {

class Renderer {
public:
  virtual ~Renderer() = default;
  virtual std::string render(const editor::ViewModel &vm) const = 0;
};

class AnsiRenderer final : public Renderer {
public:
  std::string render(const editor::ViewModel &vm) const override;
};

} // namespace snip::ui