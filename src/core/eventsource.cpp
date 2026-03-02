#include "../include/core/eventsource.hpp"

EventSource::EventSource(int fd) { this->fd = fd; };

void EventSource::handler(int sig) { ezp.write(sig); }

EventSource EventSource::fromFd(int fd) { return EventSource(fd); }

EventSource EventSource::fromSignal(int sig) {
  // Bind the signal to our handler
  signal(sig, handler);

  // Watch the read end of the pipe so poll() can watch it
  return EventSource(ezp.read());
}

int EventSource::getFd() const { return fd; }

// Global bc our pipe is also global
bool EventSource::consumeSignal(char &c) { return ezp.read(c); }