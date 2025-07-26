#!/bin/sh

winegcc -shared -lrt -mno-cygwin -O3 -DVERSION_INFO="\"$*\"" version.c shmbridge.c shmbridge.spec -o shmbridge.dll