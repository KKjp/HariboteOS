/* os.ls */
OUTPUT_FORMAT(binary);
OUTPUT_ARCH(i386);
ENTRY(HariMain);

SECTIONS {
    .head : AT (0x0) {
        LONG(0x00310000)        /* 0  : size(stack+.data+heap) */
        LONG(0x69726148)        /* 4  : "Hari" */
        LONG(0x00000000)        /* 8  : mmarea*/
        LONG(0x00310000)        /* 12 : stack$B=i4|CM(B & .data$BE>Aw@h(B */
        LONG(SIZEOF(.data))     /* 16 : size of .data */
        LONG(LOADADDR(.data))   /* 20 : size of .data */
        LONG(0xE9000000)        /* 24 : E9000000 */
        LONG(HariMain - 0x20)   /* 28 : entry - 0x20 */
        LONG(0x310000)          /* 32 : heap$BNN0h3+;O%"%I%l%9(B */
    }

    .text : AT ( SIZEOF(.head) ) {*(.text)}

    .data 0x00310000 : AT ( LOADADDR(.text) + SIZEOF(.text) ) {
        *(.data)
        *(.rodata*)
        *(.bss)
    }

    /DISCARD/ : { *(.eh_frame) }
}

