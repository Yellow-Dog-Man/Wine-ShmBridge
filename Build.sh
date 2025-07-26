#!/bin/sh

LIB_VERSION="$*"


# TODO: Maybe validate the version number a little more thoroughly

if [ ${#LIB_VERSION} -lt 5 ]; then
    LIB_VERSION="0.0.1"
fi

winegcc -shared -lrt -mno-cygwin -O3 -DVERSION_INFO="\"$LIB_VERSION\"" version.c shmbridge.c shmbridge.spec -o shmbridge.dll