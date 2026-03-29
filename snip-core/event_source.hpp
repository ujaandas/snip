#pragma once

#include "ezpipe.hpp"
#include <fcntl.h>
#include <functional>
#include <unistd.h>

namespace snip::core {

/*
  EventSource represents a source of events that can be monitored by the EventLoop.
  It can be created from a file descriptor (eg; STDIN for user input) or from a signal (eg; SIGWINCH
  for window resize). Each EventSource has an onReadReady callback that is executed when the
  EventLoop detects that the source is ready to read.
*/
class EventSource {
private:
  int fd;
  bool isSignal;
  EventSource(int fd, bool isFd);

  // Keep an arr of 64 for our 64 possible signals
  static EzPipe* signals[64];

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

} // namespace snip::core