#ifndef FILE_H
#define FILE_H

#include <string>
#include <vector>

/*
Currently open file buffer - handles all IO operations thereof.
*/
class File {
private:
  std::vector<std::string> content;

public:
  // Load entire file into memory
  // TODO: Should we use mmap? Can we read file size before?
  File(const std::string fp);

  // Read between range
  std::vector<std::string> readRange(int start, int end);
};

#endif // FILE_H