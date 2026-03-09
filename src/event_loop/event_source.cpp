#include "event_source.hpp"

EzPipe *EventSource::signals[64] = {nullptr};

EventSource::EventSource(int fd, bool isSignal) : fd(fd), isSignal(isSignal) {};

EventSource EventSource::fromFd(int fd) { return EventSource(fd, false); }

void EventSource::handler(int sig) {
  if (signals[sig]) {
    signals[sig]->write(sig);
  }
}

EventSource EventSource::fromSignal(int sig) {
  // If this signal doesn't have a pipe yet, make one
  if (!signals[sig]) {
    signals[sig] = new EzPipe();
  }

  signal(sig, handler);

  // Return the read end of THIS specific signal's pipe
  return EventSource(signals[sig]->getReadFd(), true);
}

int EventSource::getFd() const { return fd; }

void EventSource::autoDrain() {
  if (isSignal) {
    char dummy;
    read(fd, &dummy, 1);
  }
}