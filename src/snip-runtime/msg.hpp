#pragma once

#include <any>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace snip::runtime {

// The universal message type
using Msg = std::any;

struct KeyMsg {
  char key;
};

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

struct FocusMsg {
  bool focused;
};

struct SignalMsg {
  int signal;
};

enum class MouseButton {
  Left,
  Middle,
  Right,
  WheelUp,
  WheelDown,
  None,
};

enum class MouseAction {
  Press,
  Release,
  Drag,
  Move,
  Scroll,
};

struct MouseMsg {
  MouseButton button = MouseButton::None;
  MouseAction action = MouseAction::Move;
  int row = 0;
  int col = 0;
  bool shift = false;
  bool alt = false;
  bool ctrl = false;
};

// Framework-level shutdown signal
struct QuitMsg {};

// Generic delayed/timer tick message
struct TickMsg {
  std::string id;
  std::uint64_t seq;
};

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

// Message emitted if a file fails to load
struct ErrorMsg {
  std::string errorMessage;
};

} // namespace snip::runtime