.code16
.equ        CYLS, 10
.text

osstart:
    jmp     entry
    .byte   0x90
    .ascii  "HARIBOTE"
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
    .ascii  "HARIBOTEOS "
    .ascii  "FAT12   "
    .space  18, 0

entry:
    movw    $0x0, %ax
    movw    %ax, %ss
    movw    $0x7c00, %sp
    movw    %ax, %ds
    movw    %ax, %es
    movw    $msg, %si

putloop:
    movb    (%si), %al
    addw    $1, %si
    cmp     $0, %al
    je      readdisk
    movb    $0x0e, %ah
    movw    $15, %bx
    int     $0x10
    jmp     putloop

/* read FAT12 disk */
readdisk:
    movw    $0x0820, %ax     # the instruction "movw $0x0820, %es" is not exist
    movw    %ax, %es         # read disk to 0x8200
    movw    $0x0, %bx        # buffer address is indicated by [ES:BX]
    movb    $0x0, %ch        # cylinder number is 0
    movb    $0x0, %dh        # head number is 0
    movb    $0x2, %cl        # sector number is 2

readloop:
    movw    $0x0, %si        # failure counter

retry:
    movb    $0x02, %ah       # disk read mode
    movb    $0x1, %al        # read a sector
    movb    $0x00, %dl       # read A drive
    int     $0x13            # call disk bios
    jnc     next             # if no error occur, go to next
    addw    $0x1, %si
    cmp     $0x5, %si 
    jae     error            # if an error occur five times, go to error
    movb    $0x00, %ah       # drive reset
    movb    $0x00, %dl       # A drive
    int     $0x13
    jmp     retry

next:
    movw    %es, %ax    
    addw    $0x0020, %ax     # add 512 to the address
    movw    %ax, %es
    addb    $1, %cl          # for (sectors = 3, cylinder = head = 0; sector <= 18; sector++)
    cmp     $18, %cl         #     readloop();
    jbe     readloop         # for (cylinder = 1; cylinder < CYLS; cylinder++)
    movb    $1, %cl          #     for (head = 0; head < 2; head++)
    addb    $1, %dh          #         for (sector = 1; sector <= 18; sector++)
    cmp     $2, %dh          #             readloop();
    jb      readloop
    movb    $0, %dh
    addb    $1, %ch
    cmp     $CYLS, %ch
    jb      readloop
    jmp     succeed

print:
    movb    (%si), %al
    addw    $1, %si
    cmp     $0, %al
    je      return
    movb    $0x0e, %ah
    movw    $15, %bx
    int     $0x10
    jmp     print

error:
    movw    $errormsg, %si
    call    print

succeed:
    movw    $succeedmsg, %si
    call    print
    movb    %ch, 0x0ff0      # store the length of ipl10.S reading disk
    jmp     0xc200           # jump to ./asmhead.S

return:
    ret

msg:
    .byte   0x0a, 0x0a
    .ascii  "hello world!"
    .byte   0x0a
    .byte   0x0d
    .byte   0x00

errormsg:
    .byte   0x0a, 0x0a
    .ascii  "disk read error!"
    .byte   0x0a
    .byte   0x0d
    .byte   0x00

succeedmsg:
    .byte   0x0a, 0x0a
    .ascii  "disk read succeed!"
    .byte   0x0a
    .byte   0x0d
    .byte   0x00

empty:
    .space  osstart + 510 - ., 0

    .byte   0x55, 0xaa       # boot signeture

