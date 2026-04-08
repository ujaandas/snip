#pragma once

#include "snip-render/ir.hpp"

#include <string>

namespace snip::render {

class Translator {
public:
  virtual ~Translator() {};
  virtual std::string translate(const Frame& frame) const = 0;
};

class AnsiTranslator : Translator {
public:
  std::string translate(const Frame& frame) const override;
};

} // namespace snip::render
