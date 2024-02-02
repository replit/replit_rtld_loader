#! /usr/bin/env nix-shell
#! nix-shell -i bash -p clang python310Full gnumake

set -e

make lint-check
make test