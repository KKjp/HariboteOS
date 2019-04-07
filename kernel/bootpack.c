/* kernel/bootpack.c
 * io_hlt() is written in kernel/_bootpack.S
 */

#include <stdint.h>

extern void io_hlt(void);

void HariMain(void)
{
    static uint8_t *i;

    for (i = (uint8_t *)0xa0000; i <= (uint8_t *)0xaffff; i++)
        *i = 15;

    for (;;)
        io_hlt();

}

