#include </usr/include/semaphore.h>
#include </usr/include/errno.h>
#include <winternl.h>
#include <minwindef.h>

// Direct proxy to Linux semaphore functions.

sem_t *WINAPI sem_open_native(const char *name, int oflag, unsigned int mode, unsigned int value)
{
    sem_t *sem_ptr = sem_open(name, oflag, mode, value);
    int err = errno;

    if (sem_ptr == SEM_FAILED)
    {
        RtlSetLastWin32Error(err);
    }
    else
    {
        RtlSetLastWin32Error(0);
    }

    return sem_ptr;
}

int WINAPI sem_post_native(sem_t *sem)
{
    int sem_result = sem_post(sem);
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

int WINAPI sem_wait_native(sem_t *sem)
{
    int sem_result = sem_wait(sem);
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

int WINAPI sem_trywait_native(sem_t *sem)
{
    int sem_result = sem_trywait(sem);
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

int WINAPI sem_timedwait_native(sem_t *sem, const struct timespec *restrict abstime)
{
    int sem_result = sem_timedwait(sem, abstime);
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

int WINAPI sem_unlink_native(const char *name)
{
    int sem_result = sem_unlink(name);
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

int WINAPI sem_close_native(sem_t *sem)
{
    int sem_result = sem_close(sem);
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
