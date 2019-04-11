.file "_bootpack.S"
.code32

.globl io_hlt, io_cli, io_sti, io_stihlt
.globl io_in8, io_in16, io_in32
.globl io_out8, io_out16, io_out32
.globl io_load_eflags, io_store_eflags

.text

io_hlt:
    hlt
    ret

io_cli:
    cli
    ret

io_sti:
    sti
    ret

io_stihlt:
    sti
    hlt
    ret

io_in8:
    movl    4(%esp), %edx       # port
    movl    $0, %eax
    in      %dx, %al
    ret

io_in16:
    movl    4(%esp), %edx       # port
    movl    $0, %eax
    in      %dx, %ax
    ret

io_in32:
    movl    4(%esp), %edx       # port
    in      %dx, %eax
    ret

io_out8:
    movl    4(%esp), %edx       # port
    movl    8(%esp), %eax       # data
    out     %al, %dx
    ret

io_out16:
    movl    4(%esp), %edx       # port
    movl    8(%esp), %eax       # data
    out     %ax, %dx
    ret

io_out32:
    movl    4(%esp), %edx       # port
    movl    8(%esp), %eax       # data
    out     %eax, %dx
    ret

io_load_eflags:
    pushf
    pop     %eax
    ret

io_store_eflags:
    movl    4(%esp), %eax
    push    %eax
    popf
    ret

