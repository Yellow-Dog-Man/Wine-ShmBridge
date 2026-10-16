#include <windows.h>
#include <wine/unixlib.h>
#include <winternl.h>
#include "common.h"

#ifndef SHMBRIDGE_TARGET_SO
#define SHMBRIDGE_TARGET_SO L"shmbridge.so"
#endif

#ifndef SHMBRIDGE_WINE_COMPAT
static unixlib_module_t shmbridge_unix_module;
static BOOL shmbridge_unix_loaded;
#endif

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved) {

  WCHAR path[MAX_PATH];
  WCHAR *slash;
  UNICODE_STRING so_name;
  DWORD len;

  switch (reason) {
  case DLL_PROCESS_ATTACH:
#ifdef SHMBRIDGE_WINE_COMPAT
    /*
     * Wine < 11.2:
     * Ask Wine to locate and attach this builtin DLL's Unixlib. This
     * uses builtin semantics, meaning that the .dll and the .so files need to
     * be placed in a special directory structure and added with the WINEDLLPATH
     * env variable.
     */
    if (__wine_init_unix_call())
      return FALSE;
#else
    /*
     * Wine >= 11.2:
     * We can use the new method, which directly tries to
     * find the .so file using its name. Much more flexible and uses
     * standard native DLL semantics, so no environment variable needed.
     */

    len = GetModuleFileNameW(instance, path, ARRAY_SIZE(path));

    // Hopefully, this never happens.
    if (!len || len == ARRAY_SIZE(path))
      return FALSE;

    // This is probably going to be a Windows path, but you never really
    // know.
    slash = wcsrchr(path, L'\\');
    if (!slash)
      slash = wcsrchr(path, L'/');

    // The module isn't loaded from a path??
    if (!slash)
      return FALSE;

    // Another safety check. Technically, we don't know how long
    // SHMBRIDGE_TARGET_SO is, and we don't want to exceed the max
    // path...
    slash++;
    if ((SIZE_T)(slash - path) + wcslen(SHMBRIDGE_TARGET_SO) + 1 >
        ARRAY_SIZE(path))
      return FALSE;

    wcscpy(slash, SHMBRIDGE_TARGET_SO);
    RtlInitUnicodeString(&so_name, path);

    // Directly try loading the .so file that is next to this .dll file.
    if (__wine_load_unix_lib(&so_name, &shmbridge_unix_module,
                             &__wine_unixlib_handle))
      return FALSE;
    shmbridge_unix_loaded = TRUE;
#endif
    break;

  case DLL_PROCESS_DETACH:
#ifndef SHMBRIDGE_WINE_COMPAT
    // On detach, if reserved is nullptr, the process is terminating,
    // and so cleanup is not necessary.
    if (!reserved && shmbridge_unix_loaded) {
      // We don't really care about the result anymore. The DLL is
      // being unloaded and the return value at this point is ignored. Goodbye!
      (void)__wine_unload_unix_lib(shmbridge_unix_module);
      shmbridge_unix_loaded = FALSE;
      shmbridge_unix_module = 0;
      __wine_unixlib_handle = 0;
    }
#endif
    break;
  }

  return TRUE;
}

// Proxy to POSIX semaphore functions through Wine unixlib.

void *WINAPI sem_open_native(const char *name, int oflag, unsigned int mode,
                             unsigned int value) {
  struct sem_open_unix_params params = {
      .name = name,
      .oflag = oflag,
      .mode = mode,
      .value = value,
  };
  NTSTATUS status = WINE_UNIX_CALL(sem_open_unix, &params);

  if (status) {
    RtlSetLastWin32Error(status);
    return SHMBRIDGE_SEM_FAILED;
  } else if (params.result_handle == SHMBRIDGE_SEM_FAILED) {
    RtlSetLastWin32Error(params.errno_out);
  } else {
    RtlSetLastWin32Error(0);
  }

  return params.result_handle;
}

int WINAPI sem_post_native(void *sem) {
  struct sem_post_unix_params params = {.handle = sem};
  NTSTATUS status = WINE_UNIX_CALL(sem_post_unix, &params);

  if (status) {
    RtlSetLastWin32Error(status);
    return -1;
  } else if (params.result < 0) {
    RtlSetLastWin32Error(params.errno_out);
  } else {
    RtlSetLastWin32Error(0);
  }

  return params.result;
}

int WINAPI sem_wait_native(void *sem) {
  struct sem_wait_unix_params params = {.handle = sem};
  NTSTATUS status = WINE_UNIX_CALL(sem_wait_unix, &params);

  if (status) {
    RtlSetLastWin32Error(status);
    return -1;
  } else if (params.result < 0) {
    RtlSetLastWin32Error(params.errno_out);
  } else {
    RtlSetLastWin32Error(0);
  }

  return params.result;
}

int WINAPI sem_trywait_native(void *sem) {
  struct sem_trywait_unix_params params = {.handle = sem};
  NTSTATUS status = WINE_UNIX_CALL(sem_trywait_unix, &params);

  if (status) {
    RtlSetLastWin32Error(status);
    return -1;
  } else if (params.result < 0) {
    RtlSetLastWin32Error(params.errno_out);
  } else {
    RtlSetLastWin32Error(0);
  }

  return params.result;
}

int WINAPI sem_timedwait_native(void *sem,
                                const struct timespec *restrict abstime) {
  struct sem_timedwait_unix_params params = {
      .handle = sem,
      .abstime = abstime,
  };
  NTSTATUS status = WINE_UNIX_CALL(sem_timedwait_unix, &params);

  if (status) {
    RtlSetLastWin32Error(status);
    return -1;
  } else if (params.result < 0) {
    RtlSetLastWin32Error(params.errno_out);
  } else {
    RtlSetLastWin32Error(0);
  }

  return params.result;
}

int WINAPI sem_unlink_native(const char *name) {
  struct sem_unlink_unix_params params = {.name = name};
  NTSTATUS status = WINE_UNIX_CALL(sem_unlink_unix, &params);

  if (status) {
    RtlSetLastWin32Error(status);
    return -1;
  } else if (params.result < 0) {
    RtlSetLastWin32Error(params.errno_out);
  } else {
    RtlSetLastWin32Error(0);
  }

  return params.result;
}

int WINAPI sem_close_native(void *sem) {
  struct sem_close_unix_params params = {.handle = sem};
  NTSTATUS status = WINE_UNIX_CALL(sem_close_unix, &params);

  if (status) {
    RtlSetLastWin32Error(status);
    return -1;
  } else if (params.result < 0) {
    RtlSetLastWin32Error(params.errno_out);
  } else {
    RtlSetLastWin32Error(0);
  }

  return params.result;
}
