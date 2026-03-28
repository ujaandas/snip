#pragma once

#include <fcntl.h>
#include <unistd.h>

namespace snip::core {

class EzPipe {
private:
  int fds[2] = {-1, -1};

public:
  EzPipe();

  // Put a byte in the write-end
  void write(char c);

  // Pull a byte from the read-end
  bool read(char &c);

  // Empty pipe
  void clear();

  // Accessor methods for fds
  int getWriteFd();
  int getReadFd();

  ~EzPipe();
};

} // namespace snip::core