# Replit RTLD Loader

The Replit RTLD Loader allows dynamically loaded shared libraries (.so)
to work seamlessly in Repls. It uses
the [rtld-audit API](https://man7.org/linux/man-pages/man7/rtld-audit.7.html)
to observe a process' library loading activity to learn which Nix channel
the binary was built from (if any), and then uses REPLIT_LD_LIBRARY_PATH variable to resolve
libraries. It is a better alternative than using `LD_LIBRARY_PATH` because:

1. rather than overriding the default behavior of the system loader, it acts as a fallback
2. it is aware of how Nix-built binaries work

See [LD_AUDIT-based Shared Library Loader Experiments](https://docs.google.com/document/d/1llRzZdBZIKDFk5n5NQromYMCeDaYUCB9pVLy2vahTH4)
for more background.

## How it Works

1. Activate the loader via the LD_AUDIT variable when running a program, ex: `LD_AUDIT=rtld_loader.so python main.py`
2. The loader will observe which `libc.so` is loaded when the program runs based on the la_objopen hook. If it is
   from a known Nix path, we'll use it to infer the Nix channel the program was built from.
3. If the system loader fails locate the library, it will also search the directory paths within `REPLIT_LD_LIBRARY_PATH`
   for a matching library.

## Logging

You can control the loader's log level via the `REPLIT_RTLD_LOG_LEVEL` environment variable. Valid values:

* 0 - off
* 1 - warnings
* 2 - info
* 3 - debug

Also see env_parser.h and logging.h

## Self Reliance

We are not using libc at all to avoid any libc conflicts with
the running binary. It might be possible compile libc statically into
rtld_loader.so using musl or similar, but I had a hard time with musl in
last attempt. So:

* we call system calls directly for file system access
* we vendor or DIY string functions