# Wine Shared Memory Bridge

This library is a wrapper that exposes shared memory structures (such as semaphores) to programs running within Wine.

This lets the programmer interface more directly with programs running within Wine via shared memory.

## Building

Check the GitHub Actions file for a detailed guide on building the
library. In particular, a WINE development environment is needed with
`winegcc` and a proper `mingw-w64` compiler. Building is done with a
Makefile. The Makefile contains multiple targets:

- `native-linux` and `native-osx` build the same source code but with a
  different name and ABI. This reduces ambiguity as the filename also denotes
  which platform it was built for.
- `wine-builtin` builds a PE DLL marked as a WINE built-in for use with
  WINE versions < 11.2. This requires the directory with the DLL to be
  added to the `WINEDLLPATH` environment variable. The DLL is the same
  for Linux and for macOS, but they should be named differently and put
  in different locations with their corresponding native files to
  prevent Linux from trying to load an OSX native and vice versa.
- `wine-linux` and `wine-osx` both build a PE DLL. When `BUILTIN=1` is
  set, the DLL is marked as a WINE built-in for use with WINE versions <
  11.2. This requires the directory with the DLL to be added to the
  `WINEDLLPATH` environment variable. The DLL is the same for Linux and
  macOS, but the name is important for WINE's loading semantics. In
  particular, renaming the file does not change what .so WINE tries to
  load, which causes issues.

  When built without `BUILTIN=1`, the DLLs are built for use with WINE
  versions >= 11.2 without special shenanigans. They are named
  differently as they each try to load their respective platform's
  native .so file.

## Usage
For WINE >= 11.2, the PE .DLL file automatically tries to load the
appropriately named .so file next to it. The .so file it tries to load
can be changed by setting the `SHMBRIDGE_SO_LINUX` or `SHMBRIDGE_SO_OSX`
Makefile variable at compile-time. On Linux, this should be set to
`shmbridge-linux.so`, and on macOS, this should be set to
`shmbridge-osx.so`.

For WINE < 11.2, the PE .DLL file is compiled and marked as a WINE
built-in. Note that the built-in .DLL has no difference between the
Linux and macOS build. However, because it is marked as a WINE built-in,
the folder which contains it must be added to the `WINEDLLPATH`
environment variable, otherwise it will not be loaded. In addition, a
`.so` file with the same name must be present which WINE attempts to
load automatically. The following schemes work:

```
WINEDLLPATH/shmbridge-linux{.dll,.so}
WINEDLLPATH/shmbridge-osx{.dll,.so}
```

or
```
WINEDLLPATH/x86_64-windows/shmbridge-{linux,osx}.dll
WINEDLLPATH/x86_64-unix/shmbridge-{linux,osx}.osx
```

Note that the `builtin` DLL files work on WINE >= 11.2 as well, however,
they maintain all the same limitations of needing to be placed in the
`WINEDLLPATH`. As WINE versions mature, the hope is that the newer
interface becomes more supported, such that the environment variable
hack is no longer needed.

# To-Do

- [x] Semaphores
- [ ] Memory-mapped files
