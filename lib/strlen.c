/* strlen()
 * the functions belong to ../include/osutils.h
 */

#include <stddef.h>

size_t strlen(const char *str)
{
    size_t i = 0;

    for (; str[i] ; i++) {}

    return i;
}

