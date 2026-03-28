#pragma once

#include <cstddef>
#include <string>
#include <variant>
#include <vector>

namespace snip::runtime {

enum class KeyCode {
  Rune,
  Enter,
  Escape,
  Backspace,
  Tab,
  Up,
  Down,
  Left,
  Right,
  Home,
  End,
  PageUp,
  PageDown,
  DeleteKey,
  Insert,
  Unknown,
};

struct KeyPressMsg {
  KeyCode code = KeyCode::Unknown;
  char rune = '\0';
  bool ctrl = false;
  bool alt = false;
  bool shift = false;
  std::string raw;
};

struct WindowSizeMsg {
  int width;
  int height;
};

// Framework-level shutdown signal
struct QuitMsg {};

// Message emitted when a file is successfully loaded by a background worker
struct FileLoadedMsg {
  std::string filepath;
  std::vector<std::string> lines;
};

// Message emitted when a file is successfully written by a background worker
struct FileSavedMsg {
  std::string filepath;
  std::size_t bytesWritten;
  std::size_t lineCount;
};

// Structured I/O failure message for framework commands
struct IOErrorMsg {
  std::string operation;
  std::string path;
  std::string errorMessage;
};

using Msg =
    std::variant<KeyPressMsg, WindowSizeMsg, QuitMsg, FileLoadedMsg, FileSavedMsg, IOErrorMsg>;

} // namespace snip::runtime