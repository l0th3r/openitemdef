# openitemdef
[![Build](https://github.com/l0th3r/openitemdef/actions/workflows/push_build_deploy.yml/badge.svg)](https://github.com/l0th3r/openitemdef/actions/workflows/push_build_deploy.yml)
[![Coverage](https://img.shields.io/badge/Coverate_Report-Available-green)](https://l0th3r.github.io/openitemdef/coverage/)
[![Documentation](https://img.shields.io/badge/Documentation-Available-green)](https://l0th3r.github.io/openitemdef/)
![status](https://img.shields.io/badge/status-in--development-orange)
[![License](https://img.shields.io/github/license/l0th3r/openitemdef)](LICENSE)

openitemdef is a **C library for [Steam Inventory Schemas](https://partner.steamgames.com/doc/features/inventory/schema) validation**, focused on **accurate diagnostics**, **clear architecture**, and **long-term extensibility**.

This project is designed as a **core validation engine library**, not a one-off CLI tool.  
It can be embedded in CLIs, CI pipelines, editors, or future GUI tools without rewriting the core logic.

## Features
- A **semantic validator** for Steam Inventory schemas
- A **parser-agnostic core** (JSON backend can be swapped)
- A foundation for:
  - CLI validation
  - CI checks
  - VS Code / editor integrations
  - GUI tooling

### What this project is **NOT**
- A schema editor
- A formatter or rewriter
- A Steamworks replacement
- A monolithic tool mixing parsing, validation, and output

## Roadmap
### [v0.1.0 - Core foundation](https://github.com/l0th3r/openitemdef/milestone/1)
- Diagnostics model
- Source spans and line mapping
- JSON backend adapter
- LocationMap v1
- Minimal validation pipeline

_No Steam rules yet._

### [v1.0.0 - Semantic rules](https://github.com/l0th3r/openitemdef/milestone/2)
- Duplicate `itemdefid` detection
- Type validation
- Values format validation
- Required fields
- Bundle / exchange validation
- Cross-reference checks

_Library is usable and v1.0 is shipped._
