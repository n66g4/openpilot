#!/usr/bin/env bash
# Install default openpilot-server env on a comma device (run over SSH as comma user).
set -e

SRC="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)/openpilot_server_env.sh.example"
DEST="/data/openpilot_server_env.sh"

if [ ! -f "$SRC" ]; then
  echo "missing template: $SRC" >&2
  exit 1
fi

cp "$SRC" "$DEST"
chmod +x "$DEST"
echo "installed $DEST"
echo "edit ATHENA_HOST / API_HOST in DP panel or this file, then reboot"
