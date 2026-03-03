#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H

#include "./eventsource.hpp"
#include <atomic>
#include <vector>

class EventLoop {
private:
  std::vector<EventSource> sources = {};
  std::atomic<bool> running = false;

public:
  EventLoop();
  EventLoop(std::vector<EventSource> initSources) : sources(initSources) {};
  void run();
  void stop();
};

#endif // EVENT_LOOP_H