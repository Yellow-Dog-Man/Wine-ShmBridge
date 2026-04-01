#include <semaphore.h>
#include <errno.h>

#define WINE_UNIX_LIB
#include <wine/unixlib.h>
#include <minwindef.h>

#include "shmbridge.h"

// Direct proxy to Linux semaphore functions.

NTSTATUS sem_open_unix(void *args)
{
    union prm_open *a = args;
    sem_t *sem_ptr = sem_open(a->name, a->oflag, a->mode, a->value);
    int err = errno;

    a->sem = sem_ptr;

    if (sem_ptr == SEM_FAILED)
    {
        RtlSetLastWin32Error(err);
    }
    else
    {
        RtlSetLastWin32Error(0);
    }

    return 0;
}

NTSTATUS sem_post_unix(void *sem)
{
    int sem_result = sem_post((sem_t *)sem);
    int err = errno;

    if (sem_result < 0)
    {
        RtlSetLastWin32Error(err);
    }
    else
    {
        RtlSetLastWin32Error(0);
    }

    return sem_result;
}

NTSTATUS sem_wait_unix(void *sem)
{
    int sem_result = sem_wait((sem_t *)sem);
    int err = errno;

    if (sem_result < 0)
    {
        RtlSetLastWin32Error(err);
    }
    else
    {
        RtlSetLastWin32Error(0);
    }

    return sem_result;
}

NTSTATUS sem_trywait_unix(void *sem)
{
    int sem_result = sem_trywait((sem_t *)sem);
    int err = errno;

    if (sem_result < 0)
    {
        RtlSetLastWin32Error(err);
    }
    else
    {
        RtlSetLastWin32Error(0);
    }

    return sem_result;
}


NTSTATUS sem_timedwait_unix(void *args)
{
#if defined(__APPLE__)
    return 1;
#else
    struct prm_timedwait *a = args;
    int sem_result = sem_timedwait(a->sem, a->abstime);
    int err = errno;

    if (sem_result < 0)
    {
        RtlSetLastWin32Error(err);
    }
    else
    {
        RtlSetLastWin32Error(0);
    }

    return sem_result;
#endif
}


NTSTATUS sem_unlink_unix(void *name)
{
    int sem_result = sem_unlink((const char *)name);
    int err = errno;

    if (sem_result < 0)
    {
        RtlSetLastWin32Error(err);
    }
    else
    {
        RtlSetLastWin32Error(0);
    }

    return sem_result;
}

NTSTATUS sem_close_unix(void *sem)
{
    int sem_result = sem_close((sem_t *)sem);
    int err = errno;

    if (sem_result < 0)
    {
        RtlSetLastWin32Error(err);
    }
    else
    {
        RtlSetLastWin32Error(0);
    }

    return sem_result;
}

const unixlib_entry_t __wine_unix_call_funcs[] = {
    sem_open_unix,
    sem_post_unix,
    sem_wait_unix,
    sem_trywait_unix,
    sem_timedwait_unix,
    sem_unlink_unix,
    sem_close_unix,
};
