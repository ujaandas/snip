#pragma once

#include "text/splitline.hpp"
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

// Primary state object for the Snip editor
struct State {
  // UI and Viewport
  WindowState window;
  CursorState cursor;
  int scrollOffset = 0;

  // Data Storage
  std::vector<std::string> buffer; // The full file contents
  SplitLineBuffer curLine;         // The active line being edited

  // Metadata
  std::string debugText = "";
  std::string filename = "";
};