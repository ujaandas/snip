#ifndef EVENT_SOURCE_H
#define EVENT_SOURCE_H

#include "./ezpipe.hpp"
#include <fcntl.h>
#include <functional>
#include <unistd.h>

class EventSource {
private:
  int fd;
  bool isSignal;
  EventSource(int fd, bool isFd);

  // Keep an arr of 64 for our 64 possible signals
  static EzPipe *signals[64];

  static void handler(int sig);

public:
  static EventSource fromFd(int fd);

  static EventSource fromSignal(int sig);

  int getFd() const;

  // A tiny helper so the loop can clean up the pipe automatically
  void autoDrain();

  // A callback to execute when poll() says this FD is ready to read
  std::function<void()> onReadReady;
};

#endif // EVENT_SOURCE_H