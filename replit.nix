
{pkgs}: {
  deps = [
    pkgs.gdb
    pkgs.python310Full
    pkgs.clang
    pkgs.nix-index-unwrapped
  ];
}