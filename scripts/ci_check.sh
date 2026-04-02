#! /usr/bin/env nix-shell
#! nix-shell -i bash -p clang python314Full gnumake

set -e

make lint-check
make test