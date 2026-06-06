# parity

`parity` is a Nix-first terminal editor for people who want everything essential in one place: fast editing, clean architecture, and zero feature bloat.

## Philosophy

`parity` includes the pieces you actually need to ship and use a serious terminal editor:

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
./build/parity
```

## Architecture

The project is split into focused modules with strict responsibilities.

### `parity-core`

Concurrency and scheduling primitives:

- event loop
- event sources
- signaling pipe
- concurrent queue
- thread pool

### `parity-runtime`

Runtime integration layer:

- input decoding
- command execution and orchestration

### `parity-file`

Filesystem integration layer:

- stateless file read/write helpers
- line-range extraction utilities

### `parity-term`

Terminal integration layer:

- terminal session lifecycle
- terminal sizing/output utilities

### `parity-editor`

Editor domain layer:

- editor state/model
- operations and update logic
- message handling
- UI-agnostic view model generation

### `parity-render`

Rendering backend layer:

- ViewModel to Frame IR rendering
- styling and formatting helpers
- Frame to ANSI translation

This keeps editor behavior independent from terminal backend details and makes backend evolution straightforward.

## Contributing

Take care to adhere to the following rules when adding new functionality to `parity`:

- Each new library should have a short `README.md` detailing the responsibilities thereof.
- Try to include a short comment before each function.
- Each class should have a brief and simple explanation of what it does at the top-level.

## Roadmap

- Expand modal editing semantics toward complete workflows
- Add richer navigation, search, and picker workflows
- Add language-aware capabilities (highlighting, structure, diagnostics)
- Improve rendering performance and terminal capability detection
- Keep plugin surfaces extensible while protecting core simplicity
