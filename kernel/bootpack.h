/* kernel/bootpack.h */

#ifndef BOOTPACK_H_INCLUDED
#define BOOTPACK_H_INCLUDED

#include "graphic.h"
#include "asmfunc.h"
#include "dsctbl.h"

/* All of members of this structure is seted by either 
 * ../boot/ipl10.S or ../boot/asmhead.S
 */
typedef struct {
    char  cyls, leds, vmode, reserve;
    uint16_t scrnx, scrny;
    char  *vram;
} __attribute__((__packed__)) bootinfo;

#define COL8_000000 0
#define COL8_FF0000 1
#define COL8_00FF00 2
#define COL8_FFFF00 3
#define COL8_0000FF 4
#define COL8_FF00FF 5
#define COL8_00FFFF 6
#define COL8_FFFFFF 7
#define COL8_C6C6C6 8
#define COL8_840000 9
#define COL8_008400 10
#define COL8_848400 11
#define COL8_000084 12
#define COL8_840084 13
#define COL8_008484 14
#define COL8_848484 15

#define BLACK             COL8_000000
#define BRIGHT_RED        COL8_FF0000
#define BRIGHT_GRREN      COL8_00FF00
#define BRIGHT_YELLOW     COL8_FFFF00
#define BRIGHT_BRUE       COL8_0000FF
#define BRIGHT_PURPLE     COL8_FF00FF
#define BRIGHT_LIGHT_BRUE COL8_00FFFF
#define WHITE             COL8_FFFFFF
#define BRIGHT_GRAY       COL8_C6C6C6
#define DARK_RED          COL8_840000
#define DARK_GREEN        COL8_008400
#define DARK_YELLOW       COL8_848400
#define DARK_BRUE         COL8_000084
#define DARK_PURPLE       COL8_840084
#define DARK_LIGHT_BRUE   COL8_008484
#define DARK_GRAY         COL8_848484

#endif /* BOOTPACK_H_INCLUDED */

