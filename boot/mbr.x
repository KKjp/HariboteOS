/* mbr.lds */
OUTPUT_FORMAT(binary);
OUTPUT_ARCH(i8086);

SECTIONS {
    .text 0x7c00 : AT (0x0) {*(.text)}
}

