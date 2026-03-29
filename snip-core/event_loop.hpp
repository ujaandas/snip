#pragma once

#include "event_source.hpp"
#include <atomic>
#include <vector>

namespace snip::core {

/*
  The EventLoop manages a collection of EventSources and dispatches events to them.
  It runs in a single thread and processes events in a loop until stopped.
*/
class EventLoop {
private:
  std::vector<EventSource> sources = {};
  std::atomic<bool> running = false;

public:
  EventLoop();
  explicit EventLoop(const std::vector<EventSource>& initSources);
  void addSource(EventSource es);
  void run();
  void stop();
};

} // namespace snip::core