#ifndef ASYNC_QUEUE_H
#define ASYNC_QUEUE_H

#include "cc_queue.hpp"
#include "event_source.hpp"
#include "ezpipe.hpp"
#include <functional>

template <typename T> class AsyncQueue {
private:
  CCQueue<T> queue;
  EzPipe pipe;
  EventSource source;

public:
  // The event loop must never block!
  AsyncQueue() : queue(true), source(EventSource::fromFd(pipe.read())) {

    // Wire up event loop callback
    source.onReadReady = [this]() {
      // Drain the doorbell pipe completely
      char dummy;
      while (this->pipe.read(dummy)) {
        // Pull bytes out to clear POLLIN flag
      }

      // Process all items that background threads dropped off
      T item;
      while (this->queue.ccawait(item)) {
        if (this->onItem) {
          this->onItem(item);
        }
      }
    };
  }

  // Background threads call this
  void push(const T &item) {
    queue.ccpush(item);
    pipe.write('!'); // Ring the doorbell!
  }

  EventSource getEventSource() { return source; }

  // What happens when an item arrives?
  std::function<void(T)> onItem;
};

#endif // ASYNC_QUEUE_H