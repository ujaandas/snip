#include "ezpipe.hpp"
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>

EzPipe::EzPipe() {
  if (pipe(fds) == -1) {
    throw "Somehow, Palpatine returned (and cooked your EzPipe ctor)!";
  }
  fcntl(fds[0], F_SETFL, O_NONBLOCK);
  fcntl(fds[1], F_SETFL, O_NONBLOCK);
}

// Put a byte in the write-end
void EzPipe::write(char c) {
  if (fds[1] != -1) {
    if (::write(fds[1], &c, 1) != 1) {
      std::fprintf(stderr, "pipe write failed\n");
    }
  }
}
// Pull a byte from the read-end
bool EzPipe::read(char &c) {
  if (fds[0] == -1) {
    return false;
  }
  return ::read(fds[0], &c, 1) > 0;
}

void EzPipe::clear() {
  if (fds[0] == -1)
    return;

  char buf[64];

  while (true) {
    ssize_t n = ::read(fds[0], buf, sizeof(buf));
    if (n <= 0) {
      break; // empty or error
    }
  }
}

int EzPipe::getWriteFd() { return fds[1]; }
int EzPipe::getReadFd() { return fds[0]; }

EzPipe::~EzPipe() {
  if (fds[0] != -1) {
    ::close(fds[0]);
    fds[0] = -1;
  }

  if (fds[1] != -1) {
    ::close(fds[1]);
    fds[1] = -1;
  }
}