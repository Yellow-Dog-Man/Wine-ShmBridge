#include <minwindef.h>

#ifndef VERSION_INFO
#define VERSION_INFO "0.0.1"
#endif

#define INFOMARK    "@(#)"
#define INFOSIZE    sizeof(INFOMARK)

#define PRODUCT     INFOMARK "Wine-ShmBridge:"          \
    "Provides a means to interface with native POSIX "  \
    "shared memory structures such as semaphores and "  \
    "memory-mapped files."

#define VERSION     INFOMARK VERSION_INFO


char *WINAPI get_product_info();
char *WINAPI get_version_info();