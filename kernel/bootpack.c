/* kernel/bootpack.c */

#include <stdint.h>

#include "../include/osutils.h"
#include "bootpack.h"

/* the entry point of HariboteOS */
void HariMain(void)
{
    bootinfo *binfo = (bootinfo *) 0x0ff0;
    char s[40], mcursor[16 * 16];

    init_gdtidt();
    init_palette();
    init_screen8(binfo->vram, binfo->scrnx, binfo->scrny);
    const int mx = (binfo->scrnx - 16) / 2;
    const int my = (binfo->scrny - 28 - 16) / 2;
    init_mouse_cursor8(mcursor, DARK_LIGHT_BRUE);
    putblock8_8(binfo->vram, binfo->scrnx, 16, 16, mx, my, mcursor, 16);
    sprintf(s, "(%d, %d)", mx, my);
    putfonts8_asc(binfo->vram, binfo->scrnx, 16, 64, WHITE, s);

    for (;;)
        _io_hlt();
}

