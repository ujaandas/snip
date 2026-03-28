#pragma once

#include "snip-runtime/app.hpp"
#include "state.hpp"

namespace snip {

// The concrete application
class Snip : public runtime::App<State> {
public:
  Snip(State &state) : runtime::App<State>(state) {}

protected:
  std::vector<runtime::Cmd> init() override;
  runtime::UpdateResult<State> update(State &currentState,
                                      runtime::Msg msg) override;
  std::string render(State &currentState) override;
};

} // namespace snip