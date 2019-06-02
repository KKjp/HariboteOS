/* kernel/bootpack.c */

#include <stdint.h>

#include "../include/osutils.h"

/* declared in _bootpack.S */
extern void     _io_hlt(void);
extern void     _io_cli(void);
extern void     _io_out8(int32_t, int32_t);
extern int32_t  _io_load_eflags(void);
extern void     _io_store_eflags(int32_t);

/* prototype declare */
static void init_palette(void);
static void init_screen(char *, int32_t, int32_t);
static void set_palette(int32_t, int32_t, unsigned char *);
static void putfont8(char *, int32_t, int32_t, int32_t, char, char *);
static void putfonts8_asc(char *, int32_t, int32_t, int32_t, char, char *);
static void boxfill8(char *vram, int32_t xsize, char c, int32_t x0, int32_t y0, int32_t x1, int32_t y1);

/* All of members of this structure is seted by either 
 * ../boot/ipl10.S or ../boot/asmhead.S
 */
struct BOOTINFO {
    char  cyls, leds, vmode, reserve;
    uint16_t scrnx, scrny;
    char  *vram;
} __attribute__((__packed__));

#define COL8_000000     0
#define COL8_FF0000     1
#define COL8_00FF00     2
#define COL8_FFFF00     3
#define COL8_0000FF     4
#define COL8_FF00FF     5
#define COL8_00FFFF     6
#define COL8_FFFFFF     7
#define COL8_C6C6C6     8
#define COL8_840000     9
#define COL8_008400     10
#define COL8_848400     11
#define COL8_000084     12
#define COL8_840084     13
#define COL8_008484     14
#define COL8_848484     15

static void init_palette(void)
{
    static unsigned char table_rgb[] = {
        0x00, 0x00, 0x00,   /*  0:black */
        0xff, 0x00, 0x00,   /*  1:blight red */
        0x00, 0xff, 0x00,   /*  2:blight green */
        0xff, 0xff, 0x00,   /*  3:blight yelow */
        0x00, 0x00, 0xff,   /*  4:bright brue */
        0xff, 0x00, 0xff,   /*  5:blight purple */
        0x00, 0xff, 0xff,   /*  6:blight light brue */
        0xff, 0xff, 0xff,   /*  7:white */
        0xc6, 0xc6, 0xc6,   /*  8:blight gray */
        0x84, 0x00, 0x00,   /*  9:dark red */
        0x00, 0x84, 0x00,   /* 10:dark green */
        0x84, 0x84, 0x00,   /* 11:dark yelow */
        0x00, 0x00, 0x84,   /* 12:dark brue */
        0x84, 0x00, 0x84,   /* 13:dark purple */
        0x00, 0x84, 0x84,   /* 14:dark light brue */
        0x84, 0x84, 0x84    /* 15:dark gray */
    };

    set_palette(0, 15, table_rgb);

    return;
}

#define BLACK               COL8_000000
#define BRIGHT_RED          COL8_FF0000
#define BRIGHT_GRREN        COL8_00FF00
#define BRIGHT_YELLOW       COL8_FFFF00
#define BRIGHT_BRUE         COL8_0000FF
#define BRIGHT_PURPLE       COL8_FF00FF
#define BRIGHT_LIGHT_BRUE   COL8_00FFFF
#define WHITE               COL8_FFFFFF
#define BRIGHT_GRAY         COL8_C6C6C6
#define DARK_RED            COL8_840000
#define DARK_GREEN          COL8_008400
#define DARK_YELLOW         COL8_848400
#define DARK_BRUE           COL8_000084
#define DARK_PURPLE         COL8_840084
#define DARK_LIGHT_BRUE     COL8_008484
#define DARK_GRAY           COL8_848484

/* the entry point of HariboteOS */
void HariMain(void)
{
    struct BOOTINFO *binfo = (struct BOOTINFO *) 0x0ff0;
    char s[40];

    init_palette();
    init_screen(binfo->vram, (int32_t)binfo->scrnx, (int32_t)binfo->scrny);
    putfonts8_asc(binfo->vram, binfo->scrnx,  8,  8, WHITE, (char *) "ABC 123");
    putfonts8_asc(binfo->vram, binfo->scrnx, 31, 31, BLACK, (char *) "Haribote OS.");
    putfonts8_asc(binfo->vram, binfo->scrnx, 30, 30, WHITE, (char *) "Haribote OS.");
    sprintf(s, "scrnx = %-7.5d!", binfo->scrnx);
    putfonts8_asc(binfo->vram, binfo->scrnx, 16, 64, WHITE, s);

    for (;;)
        _io_hlt();

}

/* the function which set palette
 * In simply, this function's structure similar to:
 *
 *      for (int i = start; i <= end; i++)
 *          _palette_initalizing_hardware(rgb + i, rgb + i + 1, rgb + i + 2);
 * 
 * _palette_initalizing_hardware() is not true function(not exist),
 * but the handling likewise is 
 * implemented with using io_out8(): written in assembly.
 */
static void set_palette(int32_t start, int32_t end, unsigned char *rgb)
{
    int32_t i, eflags;

    eflags = _io_load_eflags();
    _io_cli();
    _io_out8(0x03c8, start);

    for (i = start; i <= end; i++) {
        _io_out8(0x03c9, rgb[0] / 4);
        _io_out8(0x03c9, rgb[1] / 4);
        _io_out8(0x03c9, rgb[2] / 4);
        rgb += 3;
    }

    _io_store_eflags(eflags);
    return;
}

/* the function which write a square
 * arguments:
 *      vram  - the address of start of vram
 *      xsize - the size of size of screen width on current video mode
 *      c     - the color number(defined in set_palette())
 *      x0    - the address of upper left  of the square
 *      x1    - the address of lower right of the square
 *      y0    - the address of upper left  of the square
 *      y1    - the address of lower right of the square
 */
static void boxfill8(char *vram, int32_t xsize, char c, int32_t x0, int32_t y0, int32_t x1, int32_t y1)
{
    int32_t x, y;

    for (y = y0; y <= y1; y++)
        for (x = x0; x <= x1; x++)
            vram[y * xsize + x] = c;

    return;
}

/* the function which initalize screen
 * arguments:
 *      vram - the address of start of vram(depend to
 *             graphics mode determinded by ../boot/asmhead.S)
 *      x    - the number of size of width  of screen
 *      y    - the number of size of height of screen
 */
static void init_screen(char *vram, int32_t x, int32_t y)
{
    /* boxfill8(*, *, *, x of upper left, y of upper left, x of lower right, y of lower right) */
    /* write the base of desktop */
    boxfill8(vram, x, DARK_LIGHT_BRUE,      0,      0, x -  1, y - 29);
    boxfill8(vram, x,     BRIGHT_GRAY,      0, y - 28, x -  1, y - 28);
    boxfill8(vram, x,           WHITE,      0, y - 27, x -  1, y - 27);
    boxfill8(vram, x,     BRIGHT_GRAY,      0, y - 26, x -  1, y -  1);

    /* write the left box */
    boxfill8(vram, x,           WHITE,      3, y - 24,     59, y - 24);
    boxfill8(vram, x,           WHITE,      2, y - 24,      2, y -  4);
    boxfill8(vram, x,       DARK_GRAY,      3, y -  4,     59, y -  4);
    boxfill8(vram, x,       DARK_GRAY,     59, y - 23,     59, y -  5);
    boxfill8(vram, x,           BLACK,      2, y -  3,     59, y -  3);
    boxfill8(vram, x,           BLACK,     60, y - 24,     60, y -  3);

    /* write the right box */
    boxfill8(vram, x,       DARK_GRAY, x - 47, y - 24, x -  4, y - 24);
    boxfill8(vram, x,       DARK_GRAY, x - 47, y - 23, x - 47, y -  4);
    boxfill8(vram, x,           WHITE, x - 47, y -  3, x -  4, y -  3);
    boxfill8(vram, x,           WHITE, x -  3, y - 24, x -  3, y -  3);

    return;
}

/* the function write a character
 * arguments:
 *      vram  - the address of start of vram
 *      xsize - the number of size of width of the screen
 *      x     - the address of horizontal direction the character will be written
 *      y     - the address of vertical direction the character will be written
 *      c     - the color of the character
 *      font  - the font data of the character(same as vram: horizontal data is continuous)
 */
static void putfont8(char *vram, int32_t xsize, int32_t x, int32_t y, char c, char *font)
{
    int32_t i;
    char *p, d;

    for (i = 0; i < 16; i++) {
        p = (char *) vram + (y + i) * xsize + x;      /* the address of the charactor */
        d = font[i];
        if (d & 0b10000000) p[0] = c;
        if (d & 0b01000000) p[1] = c;
        if (d & 0b00100000) p[2] = c;
        if (d & 0b00010000) p[3] = c;
        if (d & 0b00001000) p[4] = c;
        if (d & 0b00000100) p[5] = c;
        if (d & 0b00000010) p[6] = c;
        if (d & 0b00000001) p[7] = c;
    }

    return;
}

static void putfonts8_asc(char *vram, int32_t xsize, int32_t x, int32_t y, char c, char *s)
{
    extern char _hankaku[];
    for (; *s != 0x00; s++) {
        putfont8(vram, xsize, x, y, c, _hankaku + *s * 16);
        x += 8;
    }

    return;
}

