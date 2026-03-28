#pragma once

#include "msg.hpp"
#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <vector>

namespace snip {

// A Command is a deferred side-effect that optionally returns a Msg
using Cmd = std::function<std::optional<Msg>()>;

Cmd Noop();
std::vector<Cmd> Batch(std::vector<Cmd> cmds);
Cmd Sequence(std::vector<Cmd> cmds);

Cmd Emit(Msg msg);
Cmd DelayMs(int milliseconds, Msg msg);
Cmd Tick(std::string id, std::uint64_t seq, int milliseconds);
Cmd Quit();
Cmd ReadWindowSize(int fd);

Cmd ReadFile(std::string path);
Cmd WriteFile(std::string path, const std::vector<std::string> &buffer);

} // namespace snip