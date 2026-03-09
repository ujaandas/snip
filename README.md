# ✂️ snip

`snip` is a Nix‑first, batteries‑included CLI editor and TUI framework. It provides a predictable, terminal‑based editing experience designed to be configured and deployed entirely through Nix.

I built `snip` because I wanted a full-fledged editor experience without the maintenance burden of a complex NeoVim or LazyVim configuration. It aims to be the middle ground: the power of a terminal-based modal editor with the predictability of a purely functional architecture.

## Features

- **Small & Self-Contained:** A lightweight C++ codebase with zero heavy external dependencies.
- **Event-Driven:** Every event - from a keypress to a file being loaded - is a discrete event.
- **Pure State Transitions:** The core logic is a pure function, making the editor remarkably easy to reason about and test.
- **Non-Blocking I/O:** All side-effects are deferred to a background worker pool so the UI never stutters.
- **Nix-Native:** Designed to be reproducible, configurable, and easily packaged via Nix flakes.

## Architecture

`snip` follows a TEA‑inspired (The Elm Architecture) design, very heavily influenced by `go/bubbletea`. It also separates the "what" (logic) from the "how" (runtime) by using and implementing its own framework (which I intend to separate into a different project once more evolved).

### 1. The Model (State)

The `State` is an immutable snapshot of your editor at a specific point in time. It holds information like the buffer content, cursor position, and window dimensions.

### 2. Messages (Msg)

Messages are the "heartbeat" of the app and are used to relay data across components.

- **Input:** Keyboard presses and mouse events.
- **Signals:** Terminal window resizes (`SIGWINCH`).
- **Internal:** Background tasks telling the app that a file has finished loading.

### 3. The Update Loop

This is a pure function: `(State, Msg) -> (State, List<Cmd>)`.
It calculates the next version of the world and decides if any work needs to be done in the background. Because this function is pure, the editor's "logic" is entirely decoupled from the hardware.

### 4. Commands

Commands represent deferred side-effects. When the Update loop returns a `Cmd`, the runtime tosses it into a background thread pool. This is where file reading, writing, and network calls happen.

### 5. The Runtime

The runtime is the engine that ties it all together. It uses a custom event loop to:

- Monitor raw terminal input.
- Manage a thread-safe "mailbox" using a self-pipe trick for background tasks to communicate back to the UI.
- Handle the rendering of the state into ANSI escape codes for the terminal.
