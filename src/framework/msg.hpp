#pragma once

#include <any>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <string>
#include <vector>

namespace snip {

// The universal message type
using Msg = std::any;

struct KeyMsg {
  char key;
};

struct WindowSizeMsg {
  int width;
  int height;
};

// Framework-level shutdown signal.
struct QuitMsg {};

// Generic delayed/timer tick message.
struct TickMsg {
  std::string id;
  std::uint64_t seq;
};

// Message emitted when a file is successfully loaded by a background worker
struct FileLoadedMsg {
  std::string filepath;
  std::vector<std::string> lines;
};

// Message emitted when a file is successfully written by a background worker.
struct FileSavedMsg {
  std::string filepath;
  std::size_t bytesWritten;
  std::size_t lineCount;
};

// Structured I/O failure message for framework commands.
struct IOErrorMsg {
  std::string operation;
  std::string path;
  std::string errorMessage;
};

// Message emitted if a file fails to load
struct ErrorMsg {
  std::string errorMessage;
};

} // namespace snip