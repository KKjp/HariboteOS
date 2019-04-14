.file "_bootpack.S"
.code32

.globl _io_hlt, _io_cli, _io_sti, _io_stihlt
.globl _io_in8, _io_in16, _io_in32
.globl _io_out8, _io_out16, _io_out32
.globl _io_load_eflags, _io_store_eflags

.text

_io_hlt:
    hlt
    ret

_io_cli:
    cli
    ret

_io_sti:
    sti
    ret

_io_stihlt:
    sti
    hlt
    ret

_io_in8:
    movl    4(%esp), %edx       # port
    movl    $0, %eax
    in      %dx, %al
    ret

_io_in16:
    movl    4(%esp), %edx       # port
    movl    $0, %eax
    in      %dx, %ax
    ret

_io_in32:
    movl    4(%esp), %edx       # port
    in      %dx, %eax
    ret

_io_out8:
    movl    4(%esp), %edx       # port
    movl    8(%esp), %eax       # data
    out     %al, %dx
    ret

_io_out16:
    movl    4(%esp), %edx       # port
    movl    8(%esp), %eax       # data
    out     %ax, %dx
    ret

_io_out32:
    movl    4(%esp), %edx       # port
    movl    8(%esp), %eax       # data
    out     %eax, %dx
    ret

_io_load_eflags:
    pushf
    pop     %eax
    ret

_io_store_eflags:
    movl    4(%esp), %eax
    push    %eax
    popf
    ret

