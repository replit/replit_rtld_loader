# Replit RTLD Loader

The Replit RTLD Loader allows dynamically loaded shared libraries (.so)
to work seamlessly in Repls. It uses
the [rtld-audit API](https://man7.org/linux/man-pages/man7/rtld-audit.7.html)
to observe a process' library loading activity to learn which Nix channel
the binary was built from (if any), and then resolves dynamic libraries loading requests
for the process from a list of known Nix packages from the matching Nix channel to make sure
they are compatible.

See [LD_AUDIT-based Shared Library Loader Experiments](https://docs.google.com/document/d/1llRzZdBZIKDFk5n5NQromYMCeDaYUCB9pVLy2vahTH4)
for more background.

## How it Works

1. Activate the loader via the LD_AUDIT variable when running a program, ex: `LD_AUDIT=rtld_loader.so python main.py`
2. The loader will observe which `libc.so` is loaded when the program runs based on the la_objopen hook. If it is
   from a known Nix path, we'll use it to infer the Nix channel the program was built from.
3. If the program is unable to load a required shared object library by itself, it will fall back to the loader, which
   perform a lookup based on a known list of .so files from Nix packages from the same Nix channel as the program.
4. If the above lookup does not return anything, it will also search the directory paths within `REPLIT_LD_LIBRARY_PATH`
   for a matching library.

## Logging

You can control the loader's log level via the `REPLIT_RTLD_LOG_LEVEL` environment variable. Valid values:

* 0 - off
* 1 - warnings
* 2 - info
* 3 - debug

Also see env_parser.h and logging.h