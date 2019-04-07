/* asmhead.lds */
OUTPUT_FORMAT(binary);
OUTPUT_ARCH(i386);

SECTIONS {
    .text 0xc200 : AT (0x0) {*(.text)}
}


