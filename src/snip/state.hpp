#pragma once

#include "text/splitline.hpp"
#include <optional>
#include <string>
#include <vector>

// Dimensions of the terminal window
struct WindowState {
  int width = 0;
  int height = 0;
};

// Position of the cursor in the file
struct CursorState {
  int line = 0;
};

enum class SelMode { Character, Line };

// Current selection
struct Selection {
  std::size_t lineStart, lineEnd, colStart, colEnd;
  SelMode mode = SelMode::Character;
};

struct Operations {
  std::vector<Selection> sels;
  std::optional<char> last_noun;
  int count = 0;
};

// Primary state object for the Snip editor
struct State {
  // UI and Viewport
  WindowState window;
  CursorState cursor;
  int scrollOffset = 0;

  Operations op;

  // Data Storage
  std::vector<std::string> buffer; // The full file contents
  SplitLineBuffer curLine;         // The active line being edited

  // Metadata
  std::string debugText = "";
  std::string filename = "";
};