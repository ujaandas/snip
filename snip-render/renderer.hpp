#pragma once

#include "snip-editor/view_model.hpp"
#include "snip-render/ir.hpp"

namespace snip::render {

/*
  Renderer converts a ViewModel into a backend-agnostic Frame IR.
  ANSI encoding and terminal-specific output are intentionally handled downstream.
*/
class Renderer {
public:
  Frame render(const editor::ViewModel& vm) const;
};

} // namespace snip::render