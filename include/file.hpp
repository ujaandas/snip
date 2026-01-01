#include <algorithm>
#include <fstream>
#include <string>
#include <vector>

/*
Currently open file buffer - handles all IO operations thereof.
*/

#ifndef FILE_H
#define FILE_H

class File {
private:
  std::vector<std::string> buf;

public:
  // Load entire file into memory
  // TODO: Should we use mmap? Can we read file size before?
  File(const std::string fp) {
    // Create input stream
    std::ifstream in(fp);

    // Check openable
    if (!in.is_open()) {
      throw std::runtime_error("Could not open file: " + fp);
    }

    // Read to buf
    std::string line;
    while (std::getline(in, line)) {
      buf.push_back(line);
    }
  };

  // Read between range
  std::vector<std::string> readRange(int start, int end) const {
    // Bound correctly
    start = std::min(start, 0);
    end = std::min(end, (int)buf.size());

    // Check start not greater than end
    if (start > end) {
      return {};
    }

    return std::vector<std::string>(buf.begin() + start, buf.begin() + end);
  }
};

#endif // FILE_H