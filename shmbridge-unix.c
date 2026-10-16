// this is the most important define. This tells WINE that this module
// is the UNIX-side module.
#define WINE_UNIX_LIB

#include <errno.h>
#include <semaphore.h>
#include <stdint.h>
#include <ntstatus.h>
#define WIN32_NO_STATUS
#include <wine/unixlib.h>
#include "common.h"

// Use shim on apple platforms
#ifdef __APPLE__
#include "posix-macos-sem-timedwait.h"
#endif

static NTSTATUS sem_open_unix_impl(void *args)
{
	struct sem_open_unix_params *params = args;
	sem_t *sem_ptr = sem_open(params->name, params->oflag, params->mode, params->value);
	params->errno_out = errno;
	params->result_handle = sem_ptr == SEM_FAILED ? SHMBRIDGE_SEM_FAILED : sem_ptr;

	return STATUS_SUCCESS;
}

static NTSTATUS sem_post_unix_impl(void *args)
{
	struct sem_post_unix_params *params = args;
	int result = sem_post(params->handle);
	params->errno_out = errno;
	params->result = result;

	return STATUS_SUCCESS;
}

static NTSTATUS sem_wait_unix_impl(void *args)
{
	struct sem_wait_unix_params *params = args;
	int result = sem_wait(params->handle);
	params->errno_out = errno;
	params->result = result;

	return STATUS_SUCCESS;
}

static NTSTATUS sem_trywait_unix_impl(void *args)
{
	struct sem_trywait_unix_params *params = args;
	int result = sem_trywait(params->handle);
	params->errno_out = errno;
	params->result = result;

	return STATUS_SUCCESS;
}

static NTSTATUS sem_timedwait_unix_impl(void *args)
{
	struct sem_timedwait_unix_params *params = args;
	int result = sem_timedwait(params->handle, params->abstime);
	params->errno_out = errno;
	params->result = result;

	return STATUS_SUCCESS;
}

static NTSTATUS sem_unlink_unix_impl(void *args)
{
	struct sem_unlink_unix_params *params = args;
	int result = sem_unlink(params->name);
	params->errno_out = errno;
	params->result = result;

	return STATUS_SUCCESS;
}

static NTSTATUS sem_close_unix_impl(void *args)
{
	struct sem_close_unix_params *params = args;
	int result = sem_close(params->handle);
	params->errno_out = errno;
	params->result = result;

	return STATUS_SUCCESS;
}

// This is how Wine knows what to call.
__attribute__((visibility("default")))
const unixlib_entry_t __wine_unix_call_funcs[] =
{
	sem_open_unix_impl,
	sem_post_unix_impl,
	sem_wait_unix_impl,
	sem_trywait_unix_impl,
	sem_timedwait_unix_impl,
	sem_unlink_unix_impl,
	sem_close_unix_impl,
};
