/* kernel/graphics.h */

#include <stddef.h>
#include <stdint.h>
#include "bootpack.h"

void init_palette(void)
{
    uint8_t table_rgb[] = {
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
void set_palette(const int start, const int end, const uint8_t *rgb)
{
    const int32_t eflags = _io_load_eflags();
    _io_cli();
    _io_out8(0x03c8, start);

    for (size_t i = start, j = 0; i <= end; i++, j += 3) {
        _io_out8(0x03c9, rgb[j] / 4);
        _io_out8(0x03c9, rgb[j + 1] / 4);
        _io_out8(0x03c9, rgb[j + 2] / 4);
    }

    _io_store_eflags(eflags);
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
void boxfill8(char *vram, const int xsize, const char c, const int x0, const int y0, const int x1, const int y1)
{
    for (size_t y = y0; y <= y1; y++) {
        for (size_t x = x0; x <= x1; x++) {
            vram[y * xsize + x] = c;
        }
    }
}

/* the function which initalize screen
 * arguments:
 *      vram - the address of start of vram(depend to
 *             graphics mode determinded by ../boot/asmhead.S)
 *      x    - the number of size of width  of screen
 *      y    - the number of size of height of screen
 */
void init_screen8(char *vram, const int x, const int y)
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
void putfont8(char *vram, const int xsize, const int x, const int y, const char c, const char *font)
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
}

void putfonts8_asc(char *vram, const int xsize, const int x, const int y, const char c, const char *s)
{
    extern char _hankaku[];
    for (size_t i =0, j = 0; s[i]; i++, j += 8) {
        putfont8(vram, xsize, x + j, y, c, _hankaku + s[i] * 16);
    }
}

void init_mouse_cursor8(char *mouse, const char bc)
{
    static char cursor[16][16] = {
        "**************..",
        "*OOOOOOOOOOO*...",
        "*OOOOOOOOOO*....",
        "*OOOOOOOOO*.....",
        "*OOOOOOOO*......",
        "*OOOOOOO*.......",
        "*OOOOOOO*.......",
        "*OOOOOOOO*......",
        "*OOOO**OOO*.....",
        "*OOO*..*OOO*....",
        "*OO*....*OOO*...",
        "*O*......*OOO*..",
        "**........*OOO*.",
        "*..........*OOO*",
        "............*OO*",
        ".............***"
    };

    for (size_t y = 0; y < 16; y++) {
        for (size_t x = 0; x < 16; x++) {
            if (cursor[y][x] == '*') {
                mouse[y * 16 + x] = COL8_000000;
            }

            if (cursor[y][x] == 'O') {
                mouse[y * 16 + x] = COL8_FFFFFF;
            }

            if (cursor[y][x] == '.') {
                mouse[y * 16 + x] = bc;
            }
        }
    }
}

void putblock8_8(char *vram, const int vxsize, const int pxsize, const int pysize, const int px0, const int py0, const char *buf, const int bxsize)
{
    for (size_t y = 0; y < pysize; y++) {
        for (size_t x = 0; x < pxsize; x++) {
            vram[(py0 + y) * vxsize + (px0 + x)] = buf[y * bxsize + x];
        }
    }
}

