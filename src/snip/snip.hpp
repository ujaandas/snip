#pragma once

#include "snip-runtime/app.hpp"
#include "state.hpp"

// The concrete application
class Snip : public snip::App<State> {
public:
  Snip(State &state) : snip::App<State>(state) {}

protected:
  std::vector<snip::Cmd> init() override;
  snip::UpdateResult<State> update(State &currentState, snip::Msg msg) override;
  std::string render(State &currentState) override;
};