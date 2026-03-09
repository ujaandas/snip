# ✂️ snip

`snip` is a Nix‑first, batteries‑included CLI editor and TUI framework. It provides a predictable, terminal‑based editing experience designed to be configured and deployed entirely through Nix.

I built `snip` because I wanted a full-fledged editor experience without the maintenance burden of a complex NeoVim or LazyVim configuration. It aims to be the middle ground: the power of a terminal-based modal editor with the predictability of a purely functional architecture.

## Features

- **Small & Self-Contained:** A lightweight C++ codebase with zero heavy external dependencies.
- **Event-Driven:** Every event - from a keypress to a file being loaded - is a discrete event.
- **Pure State Transitions:** The core logic is a pure function, making the editor remarkably easy to reason about and test.
- **Non-Blocking I/O:** All side-effects are deferred to a background worker pool so the UI never stutters.
- **Nix-Native:** Designed to be reproducible, configurable, and easily packaged via Nix flakes.

## Getting Started

I'm still working on the build process. Currently, I just use a Makefile and rawdog it, but I intend to upgrade to CMake and Nix later down the road when I find the need to statically link my libraries.

## Architecture

`snip` uses its own mini-event-loop and is a framework in and of itself. Thus, the repository can be split into 3 main chunks.

### 1. The Event Loop

The foundation of `snip` is a custom-built, reactor-pattern engine. It is responsible for the "mechanical" side of the app, ie; message-passing and handling:

- **Event-Driven:** Currently supports monitoring STDIN and OS Signals (`SIGWINCH`) using `poll()`, consuming no resources while waiting for input.
- **Thread Worker Pool:** A dedicated background thread pool handles all I/O, ensuring the main thread stays focused on the UI.
- **Thread Safety:** Uses the self-pipe trick, a thread-safe signaling mechanism that allows background workers to "poke" the main loop once heavy tasks are complete.

### 2. The Framework

Sitting atop the event loop is a TEA‑inspired (The Elm Architecture) framework, heavily influenced by `go/bubbletea`. This layer manages the application lifecycle:

- **Messages:** A universal, type-safe messaging system using `std::any`. Whether it's a hardware keypress or a file-load completion, everything enters the logic as a discrete Message.
- **Commands:** Side-effects are never executed directly by the editor logic. Instead, the editor returns a "Command" which the framework executes asynchronously using the aforementioned thread pool.

### 3. The App

The editor itself is a collection of pure functions. Because the event-loop and framework handle the complexity of threading and I/O, the editor implementation is entirely deterministic:

- **The Model:** An immutable snapshot of the editor world—buffers, cursors, and metadata.
- **The Update Loop:** A pure function `(State, Msg) -> (State, List<Cmd>)`. It is the deterministic "brain" of the editor.
- **The View:** A passive renderer that transforms the State into optimized ANSI escape codes and whatnot for your terminal.

## Roadmap

Roadmap

- [x] Reactor-based event loop and thread pool
- [x] Elm-style framework/runtime
- [x] Basic I/O and modal navigation
- [ ] Nix-first syntax highlighting (Tree-sitter integration)
- [ ] Nix-configurable keybindings
- [ ] And more...
