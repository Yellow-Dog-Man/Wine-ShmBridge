#!/bin/sh

LIB_VERSION="$*"


# TODO: Maybe validate the version number a little more thoroughly

if [ ${#LIB_VERSION} -lt 5 ]; then
    LIB_VERSION="0.0.1"
fi

clang shmbridge.c -arch x86_64 -O3 -shared -o shmbridge.so -I ~/Documents/src/wine/include/wine/windows -I ~/Documents/src/wine/include/ /opt/homebrew/Caskroom/wine@devel/11.5/Wine\ Devel.app/Contents/Resources/wine/lib/wine/x86_64-unix/ntdll.so
