@ stdcall sem_open (str long long long) sem_open_native
@ stdcall sem_post (ptr) sem_post_native
@ stdcall sem_wait (ptr) sem_wait_native
@ stdcall sem_trywait (ptr) sem_trywait_native
@ stdcall sem_timedwait (ptr ptr) sem_timedwait_native
@ stdcall sem_unlink (str) sem_unlink_native
@ stdcall sem_close (ptr) sem_close_native