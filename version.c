#include "version.h"
#include <minwindef.h>

// Offset by the size of the prepended IDs so that they
// aren't present when calling these functions.

char *WINAPI get_product_info()
{
    return sizeof(PRODUCT_ID) + PRODUCT;
}

char *WINAPI get_version_info()
{
    return sizeof(VERSION_ID) + VERSION;
}