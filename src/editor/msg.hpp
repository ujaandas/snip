#pragma once

#include <string>
#include <vector>

namespace snip {

// Message emitted when a file is successfully loaded by a background worker
struct FileLoadedMsg {
  std::string filepath;
  std::vector<std::string> lines;
};

// Message emitted if a file fails to load
struct ErrorMsg {
  std::string errorMessage;
};

} // namespace snip