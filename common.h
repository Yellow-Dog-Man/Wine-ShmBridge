#pragma once

/* POSIX says this is platform defined so let's just hard-code it for
 * the bridge. */
#define SHMBRIDGE_SEM_FAILED ((void *)(intptr_t)-1)

struct timespec;

// Indices into the unixlib array
enum {
	sem_open_unix = 0,
	sem_post_unix,
	sem_wait_unix,
	sem_trywait_unix,
	sem_timedwait_unix,
	sem_unlink_unix,
	sem_close_unix,
};

struct sem_open_unix_params {
	const char* name;
	int oflag;
	unsigned int mode;
	unsigned int value;
	int errno_out;
	void* result_handle;
};

struct sem_post_unix_params {
	void* handle;
	int errno_out;
	int result;
};

struct sem_wait_unix_params {
	void* handle;
	int errno_out;
	int result;
};

struct sem_trywait_unix_params {
	void* handle;
	int errno_out;
	int result;
};

struct sem_timedwait_unix_params {
	void* handle;
	const struct timespec* abstime;
	int errno_out;
	int result;
};

struct sem_unlink_unix_params {
	const char* name;
	int errno_out;
	int result;
};

struct sem_close_unix_params {
	void* handle;
	int errno_out;
	int result;
};
