#include "file.hpp"
#include <algorithm>
#include <fstream>
#include <stdexcept>

namespace snip::file {

std::vector<std::string> File::readAll(const std::string& fp) {
  std::ifstream in(fp);
  if (!in.is_open()) {
    throw std::runtime_error("Could not open file: " + fp);
  }

  std::vector<std::string> content;
  std::string line;
  while (std::getline(in, line)) {
    content.push_back(line);
  }

  return content;
}

std::vector<std::string> File::readRange(const std::string& fp, int start, int end) {
  std::vector<std::string> content = readAll(fp);

  start = std::max(start, 0);
  end = std::min(end, static_cast<int>(content.size()));

  if (start >= end) {
    return {};
  }

  return std::vector<std::string>(content.begin() + start, content.begin() + end);
}

std::size_t File::writeAll(const std::string& fp, const std::vector<std::string>& lines) {
  std::ofstream out(fp, std::ios::out | std::ios::trunc);
  if (!out.is_open()) {
    throw std::runtime_error("Could not open file for writing: " + fp);
  }

  std::size_t bytesWritten = 0;
  for (std::size_t i = 0; i < lines.size(); ++i) {
    out << lines[i];
    bytesWritten += lines[i].size();

    if (i + 1 < lines.size()) {
      out << '\n';
      ++bytesWritten;
    }
  }

  out.flush();
  if (!out) {
    throw std::runtime_error("Failed while writing file: " + fp);
  }

  return bytesWritten;
}

} // namespace snip::file
