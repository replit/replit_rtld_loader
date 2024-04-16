# Replit RTLD Loader

The Replit RTLD Loader allows dynamically loaded shared libraries (.so)
to work seamlessly in Repls. It uses
the [rtld-audit API](https://man7.org/linux/man-pages/man7/rtld-audit.7.html)
to observe a process's library loading activities. When the native loader ld-linux
fails to find a library, RTLD loader steps in resolves the desired library using
directories in the `REPLIT_LD_LIBRARY_PATH` variable. It is a better alternative
than using `LD_LIBRARY_PATH` because rather than overriding the default behavior
of the system loader, it acts as a fallback.

## Background and Motivation

At Replit we use Nix to deliver almost all our software to users. However, we noticed
users experiencing programs crashing with errors like these:

```
symbol lookup error: /nix/store/dg8mpqqykmw9c7l0bgzzb5znkymlbfjw-glibc-2.37-8/lib/libc.so.6: undefined symbol: _dl_audit_symbind_alt, version GLIBC_PRIVATE
```

```
/nix/store/dg8mpqqykmw9c7l0bgzzb5znkymlbfjw-glibc-2.37-8/lib/libm.so.6: version `GLIBC_2.38' not found (required by /nix/store/8w6mm5q1n7i7cs1933im5vkbgvjlglfn-python3-3.10.13/lib/libpython3.10.so.1.0)
```

glibc is the GNU standard C library, a fundamental library used by virtually all programs.
These errors mean there is a mismatch between the version of glibc required by a library and the one that's available.
This can happen when we have programs and libraries from different Nix channels interacting with each other.

For example, if a Python program uses libcairo (maybe via pycairo), an entry containing the `libcairo.so` shared library would be added to the `LD_LIBRARY_PATH` variable, telling the system library loader to look for libraries there in addition to the normal places. But if Python is built from a different Nix channel from libcairo, they may depend on different versions of glibc. Python will get to choose its desired glibc version, but if it is incompatible with libcairo because its Nix channel is older than that of libcairo, the program will crash when we try to load libcairo.

We found the approach of using `LD_LIBRARY_PATH` too heavy-handed: it forced programs to abide by it even if the program already knows where its required its compatible libraries are, via its own [runpath](https://amir.rachum.com/shared-libraries/). A tamer approach is called for. With the RTLD loader, we now use the `REPLIT_LD_LIBRARY_PATH` variable, which will be used to search libraries only after loader fails to find the required libraries within the program's runpath. This plus delivering our software on the latest Nix channel will help us get rid of those glibc version mis-match problems.

## How it Works

1. Activate the loader via the LD_AUDIT variable when running a program, ex: `LD_AUDIT=rtld_loader.so python main.py`
2. If the system loader fails to locate a library, say `libcairo.so`, it will search the directories within `REPLIT_LD_LIBRARY_PATH`
for a library with that name.

How does it tell the system loader has failed to load a library? We use a `la_objsearch` hook
in the [rtld_loader API](https://man7.org/linux/man-pages/man7/rtld-audit.7.html). If the `flag` argument
passed in is equal to `LA_SER_DEFAULT`, that means the system loader has failed to find the requested library
from the `runpath` entries of the binary executable and is instead defaulting to searching the system library
paths. RTLD loader detects this when it occurs and intercepts the request, searches for the requested library
via directories listed in `REPLIT_LD_LIBRARY_PATH`, and returns the full path of the library if it is found.

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