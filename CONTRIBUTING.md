# Contributing to snip

Thanks for contributing! This document defines branch naming, PR requirements, and minimal standards for tests and documentation.

## Overview
- We follow trunk-based development: `main` is the single source of truth. Keep changes small and short-lived. Work on feature/bugfix branches and open PRs against `main` frequently.

## Branching & naming
- Protect `main`: require at least one review and passing CI to merge.
- Branches should be short-lived and named as `<type>/<scope>` or `<type>/<scope>/<short-desc>`.

Types (pick one):
- `feat` - new feature
- `fix` - bug fix
- `refactor` - code cleanup / restructuring
- `test` - tests only
- `docs` - documentation
- `chore` - maintenance (CI, tooling)

Scope: a single component or area, e.g. `snip-core`, `editor`, `cmake`.

Examples:
- `feat/snip-editor/add-multiselect`
- `fix/snip-file/path-handling`
- `refactor/cmake`

## Work flow
1. Branch from the `main`.
2. Make a focused change and open a PR against `main` as soon as it's reviewable.
3. Require passing CI, at least one approving review and a clear PR description.
4. Merge using a squash to keep `main` readable.

## Commits
- Prefer small, focused commits and use Conventional Commits (`feat:`, `fix:`, `chore:`).
- Squash or fixup locally before merging to keep history tidy.

## Tests & CI
- Every PR that changes behavior must include tests.