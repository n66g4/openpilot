#!/usr/bin/env bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

source "$DIR/launch_env.sh"

# Boot timing: /proc/uptime timeline shared with manager.py
export BOOT_TIMING="${BOOT_TIMING:-1}"
if [ -f /AGNOS ]; then
  BOOT_TIMING_PATH="${BOOT_TIMING_PATH:-/data/boot_timing.log}"
  BOOT_TIMING_VERBOSE="${BOOT_TIMING_VERBOSE:-0}"
else
  BOOT_TIMING_PATH="${BOOT_TIMING_PATH:-/tmp/boot_timing.log}"
  BOOT_TIMING_VERBOSE="${BOOT_TIMING_VERBOSE:-1}"
fi
export BOOT_TIMING_PATH BOOT_TIMING_VERBOSE

boot_mark() {
  case "${BOOT_TIMING}" in 0|false|no|off|OFF) return 0 ;; esac
  local event="$1"
  local extra="${2:-}"
  local up
  up=$(awk '{printf "%.3f", $1}' /proc/uptime 2>/dev/null || echo "0.000")
  if [ -n "$extra" ]; then
    echo -e "${up}\t${event}\t${extra}" >> "$BOOT_TIMING_PATH" 2>/dev/null || true
  else
    echo -e "${up}\t${event}" >> "$BOOT_TIMING_PATH" 2>/dev/null || true
  fi
  if [ "${BOOT_TIMING_VERBOSE}" = "1" ]; then
    if [ -n "$extra" ]; then
      echo "boot_timing: ${up}	${event}	${extra}"
    else
      echo "boot_timing: ${up}	${event}"
    fi
  fi
}

# Write a single-line value to /persist (best-effort, non-fatal on failure).
persist_write() {
  local file="$1"
  local value="$2"
  if sudo mount -o remount,rw /persist 2>/dev/null; then
    echo "$value" | sudo tee "$file" >/dev/null 2>&1
    sudo mount -o remount,ro /persist 2>/dev/null
  fi
}

function agnos_init {
  # TODO: move this to agnos
  sudo rm -f /data/etc/NetworkManager/system-connections/*.nmmeta
  rm -f /data/scons_cache/config.lock

  # set success flag for current boot slot
  sudo abctl --set_success

  # TODO: do this without udev in AGNOS
  # udev does this, but sometimes we startup faster
  sudo chgrp gpu /dev/adsprpc-smd /dev/ion /dev/kgsl-3d0
  sudo chmod 660 /dev/adsprpc-smd /dev/ion /dev/kgsl-3d0

  # Check if AGNOS update is required
  if [ $(< /VERSION) != "$AGNOS_VERSION" ]; then
    boot_mark "launch.agnos_update.start"
    AGNOS_PY="$DIR/system/hardware/tici/agnos.py"
    MANIFEST="$DIR/system/hardware/tici/agnos.json"
    if $AGNOS_PY --verify $MANIFEST; then
      sudo reboot
    fi
    $DIR/system/hardware/tici/updater $AGNOS_PY $MANIFEST
    boot_mark "launch.agnos_update.done"
  else
    boot_mark "launch.agnos_skip_update"
  fi
}

# Determine the panda MCU type (F4=DOS, H7=TRES) and set the TICI_* env vars.
# The MCU type is a permanent hardware fact, so it is detected once and cached in
# /persist (survives fork switch / reset / reflash); every later boot reads the
# cache and skips the panda query entirely.
set_tici_hw() {
  grep -q "tici" /sys/firmware/devicetree/base/model 2>/dev/null || return 0
  export TICI_HW=1

  local cache="/persist/dp_dev_panda_mcu_type"
  local attempts=15 confirm=3         # give up after N reads; trust after M in a row
  local mcu="" count=0 last="" cur cached

  # --- fast path: trust a valid (F4/H7) cached value, no panda query or sleep ---
  cached=$(cat "$cache" 2>/dev/null)
  case "$cached" in
    F4|H7) mcu="$cached"; echo "panda MCU $mcu [cached]"; boot_mark "launch.mcu.cached" "$mcu" ;;
  esac

  # --- slow path: detect, requiring M consecutive identical reads to reject a
  #     transient misread while the panda enumerates, then persist for next boot ---
  if [ -z "$mcu" ]; then
    boot_mark "launch.mcu_detect.start"
    echo "Querying panda MCU type..."
    for attempt in $(seq 1 "$attempts"); do
      # wait long while the panda is still coming up, short between confirmations
      if [ -n "$last" ]; then sleep 1; else sleep 3; fi

      # Only the internal panda exists here: the aux USB-C port isn't switched to
      # host mode until after this runs (see set_aux_panda), so a plain connect is
      # unambiguous - there is exactly one panda to read.
      case "$(python -c "from panda_tici import Panda; p = Panda(cli=False); print(p.get_mcu_type()); p.close()" 2>/dev/null)" in
        *McuType.F4*) cur="F4" ;;
        *McuType.H7*) cur="H7" ;;
        *)            cur="" ;;
      esac

      if [ -n "$cur" ] && [ "$cur" = "$last" ]; then
        count=$((count + 1))
      else
        count=1
        last="$cur"
      fi

      if [ -n "$cur" ] && [ "$count" -ge "$confirm" ]; then
        mcu="$cur"
        break
      fi
      echo "panda MCU read='${cur:-UNKNOWN}' (confirmed $count/$confirm, attempt $attempt/$attempts)"
    done

    if [ -z "$mcu" ]; then
      echo "TICI (UNKNOWN) detected after $attempts attempts, stop processing."
      exit 1
    fi

    # Persist it so future boots skip detection. /persist is comma's protected,
    # read-only partition, so flip it rw just for this one write (happens once per
    # device) and back to ro. The fast-path cat above reads fine on a ro mount, so
    # only the write needs this. Any failure here is non-fatal: re-detect next boot.
    if sudo mount -o remount,rw /persist 2>/dev/null; then
      echo "$mcu" | sudo tee "$cache" >/dev/null 2>&1
      sudo mount -o remount,ro /persist 2>/dev/null
    fi
    boot_mark "launch.mcu_detect.done" "$mcu"
  fi

  # --- apply: DOS (F4) also mounts the NVMe; TRES (H7) does not ---
  if [ "$mcu" = "F4" ]; then
    echo "TICI (DOS) detected"
    boot_mark "launch.mount_nvme.start"
    mount_nvme
    boot_mark "launch.mount_nvme.done"
    export TICI_DOS=1
    boot_mark "launch.set_aux_panda.start"
    set_aux_panda
    boot_mark "launch.set_aux_panda.done"
  else
    echo "TICI (TRES) detected"
    export TICI_TRES=1
    boot_mark "launch.tres.skip_nvme_aux"
  fi
}

# The aux USB-C port (a600000.ssusb) boots in OTG idle ("none"); a 2nd panda
# plugged there only enumerates once the port is switched to USB host mode. Only
# DOS (pandad_tici) supports a 2nd USB panda, so this runs for F4 only, and only
# after set_tici_hw has fingerprinted the internal panda alone. Keep host mode
# only if a 2nd panda actually shows up; otherwise revert to "none" so the port
# stays usable as a USB device (PC connect) on units with no aux panda.
# Cache last result in /persist. When cached "none", skip the USB mode switch
# entirely (saves time and avoids disturbing the internal panda). When we do
# switch host/none, allow USB to settle before launch continues.
set_aux_panda() {
  local mode="/sys/devices/platform/soc/a600000.ssusb/mode"
  local cache="/persist/dp_dev_aux_panda"
  local cached="" loops=6 sleep_s=0.5

  [ -e "$mode" ] || return 0

  cached=$(cat "$cache" 2>/dev/null)
  case "$cached" in
    none)
      boot_mark "launch.aux_panda.cached_none"
      return 0
      ;;
    present)
      boot_mark "launch.aux_panda.cached_present"
      loops=4
      sleep_s=0.3
      ;;
  esac

  echo "Checking for aux panda (switching USB-C port to host mode)..."
  echo host | sudo tee "$mode" >/dev/null 2>&1
  for _ in $(seq 1 "$loops"); do
    sleep "$sleep_s"
    if [ "$(lsusb 2>/dev/null | grep -c 'comma.ai panda')" -ge 2 ]; then
      echo "aux panda detected (USB host mode kept)"
      boot_mark "launch.aux_panda.found"
      persist_write "$cache" "present"
      return 0
    fi
  done

  echo "no aux panda found; reverting USB-C port to device mode"
  echo none | sudo tee "$mode" >/dev/null 2>&1
  boot_mark "launch.aux_panda.none"
  persist_write "$cache" "none"
  # Let internal panda/USB stack stabilize after the mode switch.
  sleep 2
  boot_mark "launch.aux_panda.settle"
}

nvme_has_controller() {
  [ -e /dev/nvme0 ] || [ -d /sys/class/nvme/nvme0 ] 2>/dev/null
}

mount_nvme() {
  local cache="/persist/dp_dev_nvme_state"
  local cached="" wait_loops=0 max_wait=5

  cached=$(cat "$cache" 2>/dev/null)
  case "$cached" in
    absent)
      boot_mark "launch.nvme.cached_absent"
      if [ -b /dev/nvme0n1p1 ]; then
        boot_mark "launch.nvme.cache_miss_installed"
        cached=""
      else
        boot_mark "launch.nvme.absent"
        return 0
      fi
      ;;
    present)
      boot_mark "launch.nvme.cached_present"
      max_wait=5
      ;;
  esac

  if [ -z "$cached" ] && ! nvme_has_controller; then
    boot_mark "launch.nvme.no_controller"
    persist_write "$cache" "absent"
    boot_mark "launch.nvme.absent"
    return 0
  fi

  for i in $(seq 1 "$max_wait"); do
    wait_loops=$i
    [ -b /dev/nvme0n1p1 ] && break
    sleep 1
  done
  boot_mark "launch.nvme.wait_loops" "$wait_loops"

  if [ ! -b /dev/nvme0n1p1 ]; then
    boot_mark "launch.nvme.absent"
    persist_write "$cache" "absent"
    return 0
  fi

  persist_write "$cache" "present"

  # We assume /data/media/0/realdata exists per defaults
  if ! mountpoint -q /data/media/0/realdata; then
    mount /dev/nvme0n1p1 /data/media/0/realdata
  fi

  if mountpoint -q /data/media/0/realdata; then
    boot_mark "launch.nvme.mounted"
    OWNER="$(stat -c '%U' /data/media/0/realdata)"
    GROUP="$(stat -c '%G' /data/media/0/realdata)"
    PERM="$(stat -c '%a' /data/media/0/realdata)"

    if [ "$OWNER" != "comma" ] || [ "$GROUP" != "comma" ]; then
      chown comma:comma /data/media/0/realdata
    fi

    if [ "$PERM" != "755" ]; then
      chmod 755 /data/media/0/realdata
    fi
  fi
}

set_lite_hw() {
  if grep -q "tici" /sys/firmware/devicetree/base/model 2>/dev/null; then
    output=$(i2cget -y 0 0x10 0x00 2>/dev/null)

    if [ -z "$output" ]; then
      echo "Lite HW"
      export LITE=1
    fi
  fi
}

# dp - model selector: if a car model has been selected, force it and skip the FW query
# (uses stock openpilot FINGERPRINT/SKIP_FW_QUERY env vars - no opendbc/card patch needed).
# AGNOS-only, so the params path is always /data/params/d.
set_model_fingerprint() {
  local model
  model=$(cat /data/params/d/dp_dev_model_selected 2>/dev/null)
  if [ -n "$model" ] && [ "$model" != "0" ]; then
    export FINGERPRINT="$model"
    export SKIP_FW_QUERY=1
  fi
}

# Internal panda app needs a few seconds after boot; starting manager too early
# (after removing NVMe/aux waits) can yield transient CAN faults (accFaulted).
wait_internal_panda_ready() {
  boot_mark "launch.panda_wait.start"
  python3 - <<'PY'
import time
from panda_tici import Panda

deadline = time.monotonic() + 6.0
while time.monotonic() < deadline:
  try:
    serials = Panda.list()
    if len(serials) == 1:
      with Panda(serials[0]) as p:
        if not p.bootstub:
          break
  except Exception:
    pass
  time.sleep(0.5)
PY
  boot_mark "launch.panda_wait.done"
}

function launch {
  # fresh timeline for this boot/launch
  : > "$BOOT_TIMING_PATH" 2>/dev/null || true
  boot_mark "launch.start"

  # Remove orphaned git lock if it exists on boot
  [ -f "$DIR/.git/index.lock" ] && rm -f $DIR/.git/index.lock

  # Check to see if there's a valid overlay-based update available. Conditions
  # are as follows:
  #
  # 1. The DIR init file has to exist, with a newer modtime than anything in
  #    the DIR Git repo. This checks for local development work or the user
  #    switching branches/forks, which should not be overwritten.
  # 2. The FINALIZED consistent file has to exist, indicating there's an update
  #    that completed successfully and synced to disk.

  if [ -f "${DIR}/.overlay_init" ]; then
    find ${DIR}/.git -newer ${DIR}/.overlay_init | grep -q '.' 2> /dev/null
    if [ $? -eq 0 ]; then
      echo "${DIR} has been modified, skipping overlay update installation"
    else
      if [ -f "${STAGING_ROOT}/finalized/.overlay_consistent" ]; then
        if [ ! -d /data/safe_staging/old_openpilot ]; then
          echo "Valid overlay update found, installing"
          LAUNCHER_LOCATION="${BASH_SOURCE[0]}"

          mv $DIR /data/safe_staging/old_openpilot
          mv "${STAGING_ROOT}/finalized" $DIR
          cd $DIR

          echo "Restarting launch script ${LAUNCHER_LOCATION}"
          unset AGNOS_VERSION
          exec "${LAUNCHER_LOCATION}"
        else
          echo "openpilot backup found, not updating"
          # TODO: restore backup? This means the updater didn't start after swapping
        fi
      fi
    fi
  fi
  boot_mark "launch.overlay_check"

  # handle pythonpath
  ln -sfn $(pwd) /data/pythonpath
  export PYTHONPATH="$PWD"

  # hardware specific init
  if [ -f /AGNOS ]; then
    boot_mark "launch.hw_init.start"
    set_tici_hw
    boot_mark "launch.set_tici_hw"
    set_lite_hw
    boot_mark "launch.set_lite_hw"
    agnos_init
    boot_mark "launch.agnos_init"
    set_model_fingerprint
    boot_mark "launch.hw_init.done"
  fi

  # write tmux scrollback to a file
  tmux capture-pane -pq -S-1000 > /tmp/launch_log

  # start manager
  cd system/manager
  if [ ! -f $DIR/prebuilt ]; then
    boot_mark "launch.build.start"
    ./build.py
    boot_mark "launch.build.done"
  else
    boot_mark "launch.prebuilt_skip_build"
  fi
  if [ -f /AGNOS ] && [ -n "$TICI_HW" ]; then
    wait_internal_panda_ready
  fi
  boot_mark "launch.manager_exec"
  ./manager.py

  # if broken, keep on screen error
  while true; do sleep 1; done
}

launch
