#include "file.hpp"
#include <algorithm>
#include <fstream>

File::File(const std::string fp) {
  // Create input stream
  std::ifstream in(fp);

  // Check openable
  if (!in.is_open()) {
    throw std::runtime_error("Could not open file: " + fp);
  }

  // Read to buf
  std::string line;
  while (std::getline(in, line)) {
    content.push_back(line);
  }
};

// Read between range
std::vector<std::string> File::readRange(int start, int end) {
  // Bound correctly
  start = std::min(start, 0);
  end = std::min(end, (int)content.size());

  // Check start not greater than end
  if (start > end) {
    return {};
  }

  return std::vector<std::string>(content.begin() + start,
                                  content.begin() + end);
}