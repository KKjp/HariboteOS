/* sprintf()
 * the functions belong to ../include/osutils.h
 */

#include <stddef.h>

#include "../include/osutils.h"

char *strcat(char *dest, const char *src)
{
    size_t i = 0; 
    size_t  dest_len = strlen(dest);

    for (i = 0; src[i]; i++) {
        dest[dest_len + i] = src[i];
    }

    dest[dest_len + i] = '\0';

    return dest;
}

