#!/bin/sh
set -eu
cd "$(dirname "$0")"
make native
python3 tools/compat.py --candidate bin/albat --samples-only
