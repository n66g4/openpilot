# AGENTS.md

## Cursor Cloud specific instructions

### What this branch is (important)
This checkout is **dragonpilot `v0.9.8`**, a *prebuilt release snapshot* of openpilot
targeting the **comma 3X device (ARM `aarch64`)**. This is not a PC-buildable source tree.

Key consequences for a cloud/x86_64 dev host:

- **All committed binaries and `.so` Python extensions are `aarch64`** (e.g.
  `common/params_pyx.so`, `cereal/messaging/bridge`, `third_party/acados/{aarch64,larch64}`).
  There are **no `x86_64` prebuilts anywhere** in the repo, so none of the compiled
  extensions can be imported on an x86 host.
- **`scons` cannot build on this branch.** The `cereal/messaging` C++ sources
  (`socketmaster.cc`, `bridge.cc`, `msgq_to_zmq.cc`) are not vendored here (there is no
  `.gitmodules`; a root `prebuilt` marker file is present), so the build stops immediately at
  `cereal/messaging/socketmaster.cc not found`. Do not expect `scons -u -j$(nproc)` to work.
- **`pytest` cannot collect on x86.** The root `conftest.py` imports
  `openpilot.common.params`, which loads the `aarch64` `params_pyx.so` → `ImportError`.
- **The full stack / simulator do not run on x86.** `system/manager/manager.py` and
  `tools/sim` need the compiled device binaries (and, for the car, comma hardware). The
  `tools` extra's `metadrive-simulator` wheel also has an upstream hash mismatch (see below).

To actually build/run/test the driving stack you need either a comma 3X device or a
full-source branch (all submodules present) built on the matching architecture.

### What works on an x86_64 dev host
- **Python dependency management** via `uv` (see update script). The venv lives at `.venv`.
  `PYTHONPATH` must be the repo root — `tools/install_python_dependencies.sh` writes this to
  `.env` (`PYTHONPATH=<repo root>`).
- **Environment check:** `tools/op.sh check` (validates git/os/venv/python).
- **Lint:** `ruff check .` or `scripts/lint/lint.sh` (see `tools/op.sh lint`). `ruff` runs
  fine; note the fork's own code currently has pre-existing `ruff` violations, so a full
  `ruff check .` exits non-zero on code issues (not an environment problem).
- **Core data model:** the `cereal` capnp message layer works via `pycapnp` (pure Python,
  cross-arch): `from cereal import log, car` and `log.Event.*` round-trip fine. The IPC
  transport (`cereal.messaging`, `msgq`) does **not** work on x86 (needs the `aarch64` `.so`).

### Dependency install notes
- Do **not** use `uv sync --all-extras` / `tools/install_python_dependencies.sh` as-is: the
  `tools` extra pulls `metadrive-simulator` from a GitHub release URL whose current hash no
  longer matches `uv.lock`, so a `--frozen` sync fails. Install the other extras instead
  (the update script uses `--extra testing --extra dev --extra docs`).
- System (apt) build deps come from `tools/install_ubuntu_dependencies.sh` and are captured
  in the VM snapshot; they are not part of the startup update script.

### Repo layout gotcha
There is a nested `openpilot/` directory that is a second full copy of the tree, so
`import openpilot.*` resolves to `/workspace/openpilot/...`, not the top-level modules.
