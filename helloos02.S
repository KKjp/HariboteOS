.code16
.text
.globl _start
_start:
    jmp entry
    .byte 0x90
    .ascii "HELLOIPL"
    .word 512
    .byte 1
    .word 1
    .byte 2
    .word 224
    .word 2880
    .byte 0xf0
    .word 9
    .word 18
    .word 2
    .long 0
    .long 2880
    .byte 0, 0, 0x29
    .long 0xfffffff
    .ascii "HELLO-OS   "
    .ascii "FAT12   "
    .space 18, 0

entry:
    movw $0x0, %ax
    movw %ax, %ss
    movw $0x7c00, %sp
    movw %ax, %ds
    movw %ax, %es
    movw $0x7c74, %si

putloop:
    movb (%si), %al
    addw $1, %si
    cmp $0, %al
    je fin
    movb $0x0e, %ah
    movw $15, %bx
    int $0x10
    jmp putloop

fin:
    hlt
    jmp fin

msg:
    .byte 0x0a, 0x0a
    .ascii "hello world!"
    .byte 0x0a
    .byte 0x00

empty:
    .space _start + 510 - ., 0

bootsigneture:
    .byte 0x55, 0xaa

