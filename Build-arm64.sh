#!/bin/sh

LIB_VERSION="$*"


# TODO: Maybe validate the version number a little more thoroughly

if [ ${#LIB_VERSION} -lt 5 ]; then
    LIB_VERSION="0.0.1"
fi

export PATH="/usr/lib/llvm-mingw64/bin/:$PATH"

clang shmbridge.c -O3 -shared -o shmbridge.so -I /usr/include/wine/windows/ -L /usr/lib/wine/aarch64-unix/ -l:ntdll.so -lrt
arm64ec-w64-mingw32-clang -shared -O3 -I /usr/include/wine -DVERSION_INFO="\"$LIB_VERSION\"" -L /usr/lib/wine/aarch64-windows/ -l:ntdll.dll -lwinecrt0 version.c winshim.c -o shmbridge.dll
dd bs=32 count=1 seek=2 conv=notrunc if=wine_builtin.bin of=shmbridge.dll
