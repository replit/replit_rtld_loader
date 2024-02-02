#! /usr/bin/env nix-shell
#! nix-shell -i bash -p clang python310Full gnumake

set -e

make test
make lint-check