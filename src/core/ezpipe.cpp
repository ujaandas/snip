#include "../include/core/ezpipe.hpp"
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
    ::write(fds[1], &c, 1);
  }
}

// Pull a byte from the read-end
bool EzPipe::read(char &c) {
  if (fds[0] == -1) {
    return false;
  }
  return ::read(fds[0], &c, 1) > 0;
}

int EzPipe::write() { return fds[1]; }
int EzPipe::read() { return fds[0]; }