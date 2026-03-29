#pragma once

#include <cstddef>
#include <string>
#include <vector>

// Stateless file I/O helpers
namespace snip::file {

/*
  File provides static methods for reading and writing files.
  TODO: Make this not completely crap. Also consider adding caching and stateful file handles for
  better performance on large files and to support editing files larger than available memory.
*/
class File {
public:
  File() = delete;

  // Read all lines from a file
  static std::vector<std::string> readAll(const std::string& fp);

  // Read a line range [start, end) from a file
  static std::vector<std::string> readRange(const std::string& fp, int start, int end);

  // Write all lines to a file and return bytes written
  static std::size_t writeAll(const std::string& fp, const std::vector<std::string>& lines);
};

} // namespace snip::file
