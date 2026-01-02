# ✂️ snip

snip is a tiny, batteries‑included CLI editor and TUI framework. It’s fast, minimal, and built to stay out of your way.

Under the hood, snip follows a TEA‑inspired, Elm‑style architecture: pure state transitions, deferred effects, and a tiny runtime that does the heavy lifting. The result is a predictable, testable foundation for building terminal interfaces.

The underlying TUI framework very, very, VERY heavily inspired by `go/bubbletea`, and the editor is inspired by Vim.

## Features

- Small, self‑contained C++ codebase
- Message‑driven update loop
- Pure, testable state transitions (But do I test them? No.)
- Deferred commands for all side‑effects
- Minimal runtime with clean I/O handling

## Architecture

### Messages

Events-keypresses, user actions, or internal signals. Each message carries just enough data for the update loop to decide what happens next.

### Commands

Deferred effects. The update loop never performs I/O directly; instead, it returns commands for the runtime to execute later. This keeps the core pure and easy to test.

### Event Loop

Takes the current state and a message, and returns:

- a new state
- a list of commands describing any side‑effects

No direct state mutation happens here, so makes testing and overall reasoning-about easier.

### Runtime

Handles raw input/output, executes commands, and feeds messages back into the event loop. It’s intentionally tiny - almost all logic lives in the above "pure" functions.

