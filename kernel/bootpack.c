/* kernel/bootpack.c
 * io_hlt() is written in kernel/_bootpack.S
 */

#include <stdint.h>

/* declared in _bootpack.S */
extern void io_hlt(void);
extern void io_cli(void);
extern void io_out8(uint32_t, uint32_t);
extern uint32_t io_load_eflags(void);
extern void io_store_eflags(uint32_t);

/* prototype declare */
void init_palette(void);
void set_palette(uint32_t, uint32_t, uint8_t *);

void HariMain(void)
{
    uint8_t *i;

    for (i = (uint8_t *)0xa0000; i <= (uint8_t *)0xaffff; i++)
        *i = (uint8_t)i & 0x0f;

    for (;;)
        io_hlt();

}

void init_palette(void)
{
    static uint8_t table_rgb[] = {
        0x00, 0x00, 0x00,   /* #000000:black */
        0xff, 0x00, 0x00,   /* #ff0000:blight red */
        0x00, 0xff, 0x00,   /* #00ff00:blight green */
        0xff, 0xff, 0x00,   /* #ffff00:blight yelow */
        0x00, 0x00, 0xff,   /* #0000ff:bright brue */
        0xff, 0x00, 0xff,   /* #ff00ff:blight purple */
        0x00, 0xff, 0xff,   /* #00ffff:blight light brue */
        0xff, 0xff, 0xff,   /* #ffffff:white */
        0xc6, 0xc6, 0xc6,   /* #c6c6c6:blight gray */
        0x84, 0x00, 0x00,   /* #840000:dark red */
        0x00, 0x84, 0x00,   /* #008400:dark green */
        0x84, 0x84, 0x00,   /* #848400:dark yelow */
        0x00, 0x00, 0x84,   /* #000084:dark brue */
        0x84, 0x00, 0x84,   /* #840084:dark purple */
        0x00, 0x84, 0x84,   /* #008484:dark light brue */
        0x84, 0x84, 0x84    /* #848484:dark gray */
    };

    set_palette(0, 15, table_rgb);

    return;
}

void set_palette(uint32_t start, uint32_t end, uint8_t *rgb)
{
    uint32_t i, eflags;

    eflags = io_load_eflags();
    io_cli();
    io_out8(0x03c8, start);

    for (i = start; i <= end; i++) {
        io_out8(0x03c9, rgb[0] / 4);
        io_out8(0x03c9, rgb[1] / 4);
        io_out8(0x03c9, rgb[2] / 4);
        rgb += 3;
    }

    io_store_eflags(eflags);
    return;
}

