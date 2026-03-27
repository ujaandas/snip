#include "event_loop.hpp"
#include <cerrno>
#include <poll.h>

EventLoop::EventLoop() {};

EventLoop::EventLoop(std::vector<EventSource> initSources)
    : sources(initSources) {};

void EventLoop::run() {
  running = true;

  while (running) {
    // Build a struct of fds instead of going thru each manually
    // We can pass this to poll() directly
    std::vector<struct pollfd> pfds;
    for (auto &src : sources) {
      pfds.push_back({src.getFd(), POLLIN, 0});
    }

    // Wait and poll
    if (poll(pfds.data(), pfds.size(), -1) < 0) {
      if (errno == EINTR) {
        continue;
      }
      break;
    }

    // Run callback of ready fds
    for (size_t i = 0; i < pfds.size(); ++i) {
      if (pfds[i].revents & POLLIN) {
        // Drain signal pipe if needed
        sources[i].autoDrain();

        if (sources[i].onReadReady) {
          sources[i].onReadReady();
        }
      }
    }
  }
}

void EventLoop::addSource(EventSource es) { sources.push_back(es); };

void EventLoop::stop() { running = false; }