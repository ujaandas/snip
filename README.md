# snip

`snip` is a Nix-first terminal editor for people who want everything essential in one place: fast editing, clean architecture, and zero feature bloat.

## Philosophy

`snip` includes the pieces you actually need to ship and use a serious terminal editor:

- Yoinks LSPs from your Nix shell, eliminating need for plugins
- Highly performant and concurrent event-driven app loop for predictable behavior
- Typed message/update pipeline for explicit state transitions and easy testing
- Non-blocking (deferred) command execution via background workers
- Terminal input + rendering split into clean runtime/UI boundaries for system-agnostic operation

## Quick Start

### Build with Nix

```bash
nix build
```

Run all checks:

```bash
nix flake check
```

### Without Nix

```bash
cmake -S . -B build
cmake --build build -j
./build/snip
```

## Architecture

The project is split into focused modules with strict responsibilities.

### `snip-core`

Concurrency and scheduling primitives:

- event loop
- event sources
- signaling pipe
- concurrent queue
- thread pool

### `snip-runtime`

Runtime integration layer:

- terminal session lifecycle
- input decoding
- filesystem adapters
- command execution and orchestration

### `snip-editor`

Editor domain layer:

- editor state/model
- operations and update logic
- message handling
- UI-agnostic view model generation

### `snip-ui`

Rendering backend layer:

- renderer interface
- ANSI renderer implementation

This keeps editor behavior independent from terminal backend details and makes backend evolution straightforward.

## Contributing

Take care to adhere to the following rules when adding new functionality to `snip`:
- Each new library should have a short `README.md` detailing the responsibilities thereof.
- Try to include a short comment before each function.
- Each class should have a brief and simple explanation of what it does at the top-level.

## Roadmap

- Expand modal editing semantics toward complete workflows
- Add richer navigation, search, and picker workflows
- Add language-aware capabilities (highlighting, structure, diagnostics)
- Improve rendering performance and terminal capability detection
- Keep plugin surfaces extensible while protecting core simplicity

