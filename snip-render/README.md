# snip-render

`snip-render` is the rendering layer that converts editor view models into a backend-agnostic frame IR, then translates that frame for terminal output.

## Responsibilities

- ViewModel -> Frame IR conversion
- Styling and formatting helpers for frame composition
- Frame -> ANSI translation
