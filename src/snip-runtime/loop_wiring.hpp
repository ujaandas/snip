#pragma once

#include "input.hpp"
#include "msg.hpp"
#include "snip-core/event_source.hpp"
#include "terminal/terminal.hpp"

#include <functional>
#include <optional>

namespace snip::runtime {

using KeyHandler = std::function<void(const KeyPressMsg &)>;
using SignalHandler = std::function<void(const SignalMsg &)>;
using ResizeHandler = std::function<void(const WindowSizeMsg &)>;

EventSource makeInputSource(int inputFd, KeyHandler onKey);
EventSource makeResizeSource(int signalNumber, SignalHandler onSignal);

std::optional<WindowSizeMsg> readWindowSizeMsg(int queryFd);

} // namespace snip::runtime
