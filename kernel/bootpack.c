/* kernel/bootpack.c
 * io_hlt() is written in kernel/_bootpack.S
 */

extern void io_hlt(void);

void HariMain(void)
{
    
fin:
    io_hlt();
    goto fin;

}

