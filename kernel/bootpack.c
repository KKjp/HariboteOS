/* kernel/bootpack.c
 * io_hlt() is written in kernel/_bootpack.S
 */

#include <stdint.h>

extern void io_hlt(void);

void HariMain(void)
{
    uint32_t *i;

    for (i = 0xa0000; i <= 0xaffff; i++)
        *i = 15;

    for (;;)
        io_hlt();

}

