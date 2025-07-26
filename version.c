#include "version.h"
#include <minwindef.h>

char *WINAPI get_product_info()
{
    return INFOSIZE + PRODUCT;
}

char *WINAPI get_version_info()
{
    return INFOSIZE + VERSION;
}