# parity

`parity` is a Nix-backed IDE for fully reproducible development environments.

Every project ships its own IDE. Languages, SDKs, LSPs, formatters, linters, build tools, and AI tooling are all defined by the project itself and built into the environment.

No setup guides, version drift, or "it works on my machine" - all without the sluggishness of Docker.

Open a project and get _exactly_ the tooling that project requires.

## Philosophy

Development environments should be part of the codebase.

`parity` uses Nix to build hermetic, project-specific IDEs. Every developer, CI runner, and AI agent gets the same tools, the same versions, and the same behavior.

Whether you're working in a small repository or a massive polyglot monorepo, everyone operates from the same environment.

The goal is simple:

* Complete parity across teams
* Reproducible development environments
* Zero global dependencies
* Fast local workflows
* Minimal operational overhead

## Why parity?

Most modern IDEs are built on Chromium and JavaScript stacks, but `parity` isn't.

It's written in modern C++ and Qt6/QtQuick, with a focus on speed, efficiency, and keeping resource usage predictable.

The editor is native, cross-platform, and designed around a minimal core rather than an ever-growing plugin ecosystem.

Features are added because they're necessary, not because they're fashionable.

## What You Get

* Hermetic development environments built from Nix
* Project-provided languages, SDKs, and runtimes
* Project-provided LSPs, formatters, and linters
* Consistent tooling across developers, CI, and AI agents
* Native C++ and Qt6 implementation
* Cross-platform support
* Fast startup and low resource usage
* Minimal, focused user experience
* No dependency on globally installed tooling
