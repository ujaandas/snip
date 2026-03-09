#ifndef CMD_H
#define CMD_H

#include "msg.hpp"
#include <functional>
#include <optional>

namespace snip {

// A Command is a deferred side-effect that optionally returns a Msg
using Cmd = std::function<std::optional<Msg>()>;

} // namespace snip

#endif // CMD_H