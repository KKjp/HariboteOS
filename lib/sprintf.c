/* memset()
 * the functions belong to ../include/osutils.h
 */

#include <stdarg.h>
#include <stddef.h>

#include "../include/osutils.h"

int sprintf(char *str, const char *fmt, ...)
{
    va_list list;
    int ret = 0;

    va_start(list, fmt);

    ret = vsprintf(str, fmt, list);

    va_end(list);

    return ret;
}

