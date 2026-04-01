#pragma once

#include <time.h>

union prm_open {
    struct {
        const char *name;
        int oflag;
        unsigned int mode;
        unsigned int value;
    };
    void *sem;
};

struct prm_timedwait {
    void *sem;
    const struct timespec *abstime;
};

enum shmbridge_unix_funcs {
    unix_sem_open,
    unix_sem_post,
    unix_sem_wait,
    unix_sem_trywait,
    unix_sem_timedwait,
    unix_sem_unlink,
    unix_sem_close,
};
