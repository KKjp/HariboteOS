.code16
.text

_start:
    jmp     entry
    .byte   0x90
    .ascii  "HELLOIPL"
    .word   512
    .byte   1
    .word   1
    .byte   2
    .word   224
    .word   2880
    .byte   0xf0
    .word   9
    .word   18
    .word   2
    .long   0
    .long   2880
    .byte   0, 0, 0x29
    .long   0xfffffff
    .ascii  "HELLO-OS   "
    .ascii  "FAT12   "
    .space  18, 0

entry:
    movw    $0x0, %ax
    movw    %ax, %ss
    movw    $0x7c00, %sp
    movw    %ax, %ds
    movw    %ax, %es
    movw    %ax, %di
    movw    $0x7c74, %si # movw msg, %si

putloop:
    movb    (%si), %al
    addw    $1, %si
    cmp     $0, %di
    jne     fin
    cmp     $0, %al
    je      readdisk
    movb    $0x0e, %ah
    movw    $15, %bx
    int     $0x10
    jmp     putloop

fin:
    hlt
    jmp     fin

msg:
    .byte   0x0a, 0x0a
    .ascii  "hello world!"
    .byte   0x0a
    .byte   0x00

/* The codes below is the difference between itself and helloos02.S; read FAT12 disk */
readdisk:
    movw    $0x0820, %ax
    movw    %es, %ax
    movw    $0x0, %bx   # buffer address is indicated by [ES:BX]
    movb    $0x0, %ch   # cylinder number is 0
    movb    $0x0, %dh   # head number is 0
    movb    $0x2, %cl   # sector number is 0

    movw    $0x0, %si   # failure counter

retry:
    movb    $0x02, %ah  # disk read mode
    movb    $0x1, %al   # read one sector
    movb    $0x00, %dl  # read A drive
    int     $0x13       # call disk bios
    jnc     fin         # if no error occur, go to fin
    addw    $0x1, %si
    cmp     $0x5, %si 
    jae     error       # if an error occur five times, go to error
    movb    $0x00, %ah  # drive reset
    movb    $0x00, %dh  # A drive
    int     $0x13
    jmp     retry

error:
    movw    errormsg, %si
    movw    $0x1, %di
    jmp     putloop

errormsg:
    .byte   0x0a, 0x0a
    .ascii  "disk read error!"
    .byte   0x0a
    .byte   0x00

empty:
    .space  _start + 510 - ., 0

    .byte   0x55, 0xaa # boot signeture

