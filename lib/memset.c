/* memset()
 * the functions belong to ../include/osutils.h
 */

#include <stddef.h>

void memset(void *pointer, int set, size_t size)
{
    for (size_t i = 0; i < size; i++) {
        ((unsigned char *)pointer)[i] = (unsigned char) set;
    }
}

