#include "loop_wiring.hpp"

#include <csignal>

namespace snip::runtime {

core::EventSource makeInputSource(int inputFd, KeyHandler onKey) {
  core::EventSource input = core::EventSource::fromFd(inputFd);
  input.onReadReady = [inputFd, onKey = std::move(onKey)]() {
    if (!onKey) {
      return;
    }

    if (auto key = snip::input::readKeyPress(inputFd)) {
      onKey(*key);
    }
  };
  return input;
}

core::EventSource makeResizeSource(int signalNumber, SignalHandler onSignal) {
  core::EventSource resize = core::EventSource::fromSignal(signalNumber);
  resize.onReadReady = [signalNumber, onSignal = std::move(onSignal)]() {
    if (onSignal) {
      onSignal(SignalMsg{signalNumber});
    }
  };
  return resize;
}

std::optional<WindowSizeMsg> readWindowSizeMsg(int queryFd) {
  if (auto size = snip::term::queryWindowSize(queryFd)) {
    return WindowSizeMsg{size->width, size->height};
  }
  return std::nullopt;
}

} // namespace snip::runtime
