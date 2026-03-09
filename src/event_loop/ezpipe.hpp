#ifndef EZ_PIPE_H
#define EZ_PIPE_H

#include <fcntl.h>
#include <unistd.h>

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

#endif // EZ_PIPE_H