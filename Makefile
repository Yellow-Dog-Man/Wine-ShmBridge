# Where Wine is installed to
WINE_DIR ?= /usr
# Include directory: contains wine/unixlib.h
WINE_INCLUDE_DIR ?= $(WINE_DIR)/include

# These can be changed if needed.
SHMBRIDGE_SO_LINUX ?= shmbridge-linux.so
SHMBRIDGE_SO_OSX ?= shmbridge-osx.so

# Windows vs UNIX C compiler
WINE_CC ?= $(WINE_DIR)/bin/winegcc
MINGW_CC ?= x86_64-w64-mingw32
NATIVE_CC ?= cc
VERSION ?= 1.1.0
CFLAGS ?= -O2 -Wall -Wextra
LDFLAGS ?=

# Set to 1 to build WINE built-in libs.
BUILTIN ?= 0

BUILTIN_DEFINE_0 =
BUILTIN_DEFINE_1 = -DSHMBRIDGE_WINE_COMPAT -Wl,--wine-builtin
BUILTIN_DEFINE = $(BUILTIN_DEFINE_$(BUILTIN))

SHMBRIDGE_SO_LINUX_DEFINE = -DSHMBRIDGE_TARGET_SO='L"$(SHMBRIDGE_SO_LINUX)"'
SHMBRIDGE_SO_OSX_DEFINE = -DSHMBRIDGE_TARGET_SO='L"$(SHMBRIDGE_SO_OSX)"'
WINE_INCLUDE = -I"$(WINE_INCLUDE_DIR)" -I"$(WINE_INCLUDE_DIR)/wine/windows"

.PHONY: all wine-linux wine-osx native-linux native-osx
.DELETE_ON_ERROR:
all: wine-linux native-linux
wine-linux: shmbridge-linux.dll
wine-osx: shmbridge-osx.dll

# There isn't really a difference here, but they need to be named
# differently as there's no good way to automatically load the correct
# one per platform.
native-linux: shmbridge-linux.so
native-osx: shmbridge-osx.so

shmbridge-linux.dll: shmbridge-win.c version.c version.h common.h shmbridge-linux.def
	$(WINE_CC) -b $(MINGW_CC) -shared -std=gnu17 $(CFLAGS) $(WINE_INCLUDE) \
		-D__WINE_PE_BUILD -D__WINESRC__ -DWIN32_LEAN_AND_MEAN -DVERSION_INFO='"$(VERSION)"' $(SHMBRIDGE_SO_LINUX_DEFINE) \
		$(BUILTIN_DEFINE) \
		shmbridge-win.c version.c shmbridge-linux.def \
		$(LDFLAGS) \
		-o $@

shmbridge-osx.dll: shmbridge-win.c version.c version.h common.h shmbridge-osx.def
	$(WINE_CC) -b $(MINGW_CC) -shared -std=gnu17 $(CFLAGS) $(WINE_INCLUDE) \
		-D__WINE_PE_BUILD -D__WINESRC__ -DWIN32_LEAN_AND_MEAN -DVERSION_INFO='"$(VERSION)"' $(SHMBRIDGE_SO_OSX_DEFINE) \
		$(BUILTIN_DEFINE) \
		shmbridge-win.c version.c shmbridge-osx.def \
		$(LDFLAGS) \
		-o $@

shmbridge-linux.so: shmbridge-unix.c common.h
	$(NATIVE_CC) -shared -std=gnu17 $(CFLAGS) $(WINE_INCLUDE) \
		-fPIC -fshort-wchar -fvisibility=hidden \
		shmbridge-unix.c  \
		$(LDFLAGS)  \
		-o $@

# Wine uses the .so suffix, even on macOS.
shmbridge-osx.so: shmbridge-unix.c common.h posix-macos-sem-timedwait.h
	$(NATIVE_CC) -shared -std=gnu17 $(CFLAGS) $(WINE_INCLUDE) \
		-fPIC -fshort-wchar -fvisibility=hidden \
		shmbridge-unix.c  \
		$(LDFLAGS)  \
		-o $@

clean:
	rm -f shmbridge-linux.dll shmbridge-osx.dll shmbridge-linux.so shmbridge-osx.so
