#include <minwindef.h>

#if !defined(VERSION_INFO)
    #define VERSION_INFO "0.0.1"
#endif

#define INFOMARK    "@(#)" // <-- This marker is added so that the product info and
                           // version number can be found externally if desired.

#define INFOSIZE    sizeof(INFOMARK)

#define PRODUCT_ID  INFOMARK "Product: "
#define VERSION_ID  INFOMARK "Version: "

#define PRODUCT     PRODUCT_ID  "Wine-ShmBridge: "                                 \
                                "Provides a means to interface with native POSIX "  \
                                "shared memory structures such as semaphores and "  \
                                "memory-mapped files."

#define VERSION     VERSION_ID VERSION_INFO


char *WINAPI get_product_info();
char *WINAPI get_version_info();