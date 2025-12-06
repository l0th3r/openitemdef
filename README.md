# openitemdef

![status](https://img.shields.io/badge/status-in--development-orange)
[![License](https://img.shields.io/github/license/l0th3r/openitemdef)](LICENSE)
[![language](https://img.shields.io/github/languages/top/l0th3r/openitemdef)](https://github.com/search?q=repo%3Al0th3r%2Fopenitemdef++language%3AC&type=code)
[![Build](https://github.com/l0th3r/openitemdef/actions/workflows/push_build_deploy.yml/badge.svg)](https://github.com/l0th3r/openitemdef/actions/workflows/push_build_deploy.yml)
[![Coverage](https://img.shields.io/badge/Test_Coverage-uploaded-blue)](https://l0th3r.github.io/openitemdef/coverage/)
[![Documentation](https://img.shields.io/badge/docs-latest-green)](https://l0th3r.github.io/openitemdef/)


*A C library for parsing, validating, and manipulating [Steam Inventory Schema](https://partner.steamgames.com/doc/features/inventory/schema) files.*

**openitemdef** provides a clean C API with predictable memory behavior, strict schema validation, and error reporting designed for integration into game engines, build pipelines, CLI tools, and GUI editors.\
The project includes a complete test suite using Unity and optional code-coverage instrumentation via GCC/Clang.

## 🚧 Status

> **Note**: This project is under active development.  
> The current codebase is not stable and APIs may change. Documentation and public examples will follow in future versions.

## Features

- **Full JSON schema parsing**  
  Powered by [*jansson*](https://github.com/akheron/jansson), with strict type checks and detailed validation.

- **Robust internal model**  
  Supports all Steam Inventory constructs: item definitions, bundles, exchange lists, generators, playtime generators, tags, colors, and more.

- **Embedding-friendly**  
  Made in C to ease cross-language bindings. Designed for use in game engines and their extensions or standalone tools.

## Build Requirements

- **C c99 compiler:** GCC or Clang  
- [CMake](https://cmake.org/) ≥ 3.16
- **Dependencies:**  
  - `jansson` (e.g., `libjansson-dev`)

Optional:
  - [Unity](https://github.com/ThrowTheSwitch/Unity) for testing (bundled as submodule in `third_party/unity`)
  - [`lcov`](https://github.com/linux-test-project/lcov) for coverage generation

## 🛠 Build
