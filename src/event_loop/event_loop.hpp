#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H

#include "event_source.hpp"
#include <atomic>
#include <vector>

class EventLoop {
private:
  std::vector<EventSource> sources = {};
  std::atomic<bool> running = false;

public:
  EventLoop();
  EventLoop(std::vector<EventSource> initSources);
  void addSource(EventSource es);
  void run();
  void stop();
};

#endif // EVENT_LOOP_H