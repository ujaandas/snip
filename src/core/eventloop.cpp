#include "../include/core/eventloop.hpp"
#include <poll.h>

void EventLoop::run() {
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

void EventLoop::stop() { running = false; }