#ifndef EVENT_SOURCE_H
#define EVENT_SOURCE_H

#include "./ezpipe.hpp"
#include <fcntl.h>
#include <functional>
#include <unistd.h>

class EventSource {
private:
  int fd;
  EventSource(int fd);
  static EzPipe ezp;

  static void handler(int sig);

public:
  static EventSource fromFd(int fd);

  static EventSource fromSignal(int sig);

  int getFd() const;

  // Global bc our pipe is also global
  static bool consumeSignal(char &c);

  // A callback to execute when poll() says this FD is ready to read
  std::function<void()> onReadReady;
};

#endif // EVENT_SOURCE_H