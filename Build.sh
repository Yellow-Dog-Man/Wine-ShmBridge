#!/bin/sh

winegcc -shared -lrt -mno-cygwin -O3 shmbridge.c shmbridge.spec -o shmbridge.dll