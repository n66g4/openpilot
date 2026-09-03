"""Boot timeline markers for cold-start profiling.

Timeline uses seconds since kernel boot (/proc/uptime) so launch.sh and
manager.py share one anchor at device power-on.

Env:
  BOOT_TIMING=1          enable marks (default 1)
  BOOT_TIMING_VERBOSE=1  also print to stdout (default 0 on AGNOS)
  BOOT_TIMING_PATH=...   log file path
"""
from __future__ import annotations

import os
import time

_PATH = os.environ.get(
  "BOOT_TIMING_PATH",
  "/data/boot_timing.log" if os.path.exists("/AGNOS") else "/tmp/boot_timing.log",
)
_seen: set[str] = set()
_uptime_cache: tuple[float, float] | None = None  # (uptime, monotonic_at_read)


def log_path() -> str:
  return _PATH


def enabled() -> bool:
  v = os.environ.get("BOOT_TIMING")
  if v is not None:
    return v.lower() not in ("0", "false", "no", "off")
  return True


def verbose() -> bool:
  v = os.environ.get("BOOT_TIMING_VERBOSE")
  if v is not None:
    return v.lower() not in ("0", "false", "no", "off")
  return not os.path.exists("/AGNOS")


def uptime_s() -> float:
  global _uptime_cache
  now = time.monotonic()
  if _uptime_cache is not None and (now - _uptime_cache[1]) < 0.05:
    return _uptime_cache[0] + (now - _uptime_cache[1])

  try:
    with open("/proc/uptime") as f:
      up = float(f.read().split()[0])
  except (OSError, ValueError, IndexError):
    up = now
  _uptime_cache = (up, now)
  return up


def mark(event: str, *, once: bool = False, extra: str | None = None) -> None:
  if not enabled():
    return
  if once:
    if event in _seen:
      return
    _seen.add(event)

  text = event if extra is None else f"{event}\t{extra}"
  line = f"{uptime_s():.3f}\t{text}\n"
  try:
    with open(_PATH, "a") as f:
      f.write(line)
  except OSError:
    pass
  if verbose():
    print(f"boot_timing: {line}", end="")


def reset() -> None:
  global _uptime_cache
  _seen.clear()
  _uptime_cache = None
  if not enabled():
    return
  try:
    with open(_PATH, "w") as f:
      f.write(f"{uptime_s():.3f}\tboot_timing.reset\n")
  except OSError:
    pass
