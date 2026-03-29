#pragma once

#include "snip-editor/view_model.hpp"
#include <string>

namespace snip::ui {

/*
  Renderer is responsible for converting a ViewModel into a string that can be written to the
  terminal. It's designed to be extensible to maybe later support other frontends, like a GUI or
  Notcurses or somesuch.
*/
class Renderer {
public:
  virtual ~Renderer() = default;
  virtual std::string render(const editor::ViewModel& vm) const = 0;
};

class AnsiRenderer final : public Renderer {
public:
  std::string render(const editor::ViewModel& vm) const override;
};

} // namespace snip::ui