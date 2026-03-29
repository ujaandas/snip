#pragma once

#include "msg.hpp"
#include <functional>
#include <optional>
#include <string>
#include <vector>

namespace snip::runtime {

/*
  A Command is a deferred side-effect that optionally returns a Msg
 */
using Cmd = std::function<std::optional<Msg>()>;

Cmd Quit();

Cmd ReadFile(std::string path);
Cmd WriteFile(std::string path, const std::vector<std::string>& buffer);

} // namespace snip::runtime