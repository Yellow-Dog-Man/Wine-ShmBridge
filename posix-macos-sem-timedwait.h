#ifndef POSIX_MACOS_SEM_TIMEDWAIT_H
#define POSIX_MACOS_SEM_TIMEDWAIT_H

#ifdef __APPLE__

#include <errno.h>
#include <semaphore.h>
#include <time.h>

/* Darwin has named POSIX semaphores, but no sem_timedwait.
 * Poll native handles at intervals of up to 1 ms. The deadline is absolute
 * CLOCK_REALTIME, as required by sem_timedwait; posting does not wake the sleep.
 */
static inline int macos_sem_timedwait(sem_t *sem, const struct timespec *abstime)
{
	/* An immediately available semaphore does not require a valid deadline. */
	if (sem_trywait(sem) == 0)
		return 0;
	if (errno != EAGAIN)
		return -1;

	if (abstime->tv_nsec < 0 || abstime->tv_nsec >= 1000000000L)
	{
		errno = EINVAL;
		return -1;
	}

	for (;;)
	{
		struct timespec now;
		struct timespec delay = { .tv_sec = 0, .tv_nsec = 1000000L };

		if (clock_gettime(CLOCK_REALTIME, &now) < 0)
			return -1;
		if (now.tv_sec > abstime->tv_sec ||
		    (now.tv_sec == abstime->tv_sec && now.tv_nsec >= abstime->tv_nsec))
		{
			errno = ETIMEDOUT;
			return -1;
		}

		if (now.tv_sec == abstime->tv_sec)
		{
			long remaining = abstime->tv_nsec - now.tv_nsec;
			if (remaining < delay.tv_nsec)
				delay.tv_nsec = remaining;
		}
		else if (now.tv_sec + 1 == abstime->tv_sec)
		{
			long remaining = 1000000000L - now.tv_nsec + abstime->tv_nsec;
			if (remaining < delay.tv_nsec)
				delay.tv_nsec = remaining;
		}

		/* Preserve EINTR rather than silently restarting an interrupted wait. */
		if (nanosleep(&delay, NULL) < 0)
			return -1;
		if (sem_trywait(sem) == 0)
			return 0;
		if (errno != EAGAIN)
			return -1;
	}
}

#define sem_timedwait macos_sem_timedwait

#endif /* __APPLE__ */

#endif /* POSIX_MACOS_SEM_TIMEDWAIT_H */
