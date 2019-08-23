.code16
.text

.set    BOTPAK, 0x00280000      # bootpack.bin will be loaded
.set    DSKCAC, 0x00100000      # disk cache will be loaded
.set    DSKCAC0, 0x00008000     # disk cache in real-mode will be loaded

# bootinfo
.set    CYLS, 0x0ff0            # seted by boot-sector
.set    LEDS, 0x0ff1
.set    VMODE, 0x0ff2           # the information about color bit
.set    SCRNX, 0x0ff4           # x resolution
.set    SCRNY, 0x0ff6           # y resolution
.set    VRAM, 0x0ff8            # the start of graphics memory

_asmhead:

# set graphics mode
    movb    $0x13, %al          # VGA graphics, 320*200:8bit-color
    movb    $0x00, %ah
    int     $0x10
    movb    $8, VMODE           # write graphics mode(referd by C)
    movw    $320, SCRNX
    movw    $200, SCRNY
    movl    $0x000a0000, VRAM

# get keyboard's status by BIOS
    movb    $0x02, %ah
    int     $0x16               # call keyboard BIOS
    movb    %al, LEDS

# According to the specification of PC/AT machines,
# if execute CLI without executing the code below(prevent PIC from accpting any interrupts)
# when initialize PIC, the machine will bankup sometimes.
    movb    $0xff, %al
    out     %al, $0x21          # prevent PIC from accpting any interrupts
    nop                         # for the machines which cannot cope OUT instruction continuous
    out     %al, $0xa1

    cli                         # prohibit any interrupts in the CPU level

# set A20GATE for use memory above 1MB from the CPU
    call    waitkbdout
    movb    $0xd1, %al
    out     %al, $0x64
    call    waitkbdout
    movb    $0xdf, %al          # enable A20
    out     %al, $0x60
    call    waitkbdout

# switch to protected mode
    lgdt    GDTR0               # set GDTR(a register)
    movl    %cr0, %eax
    andl    $0x7fffffff, %eax   # set 0 to bit31 for disenable paging
    or      $0x00000001, %eax   # set 1 to bit0 for switch protected mode
    movl    %eax, %cr0
    jmp     pipelineflush

pipelineflush:
    movw    $0x0008, %ax        # read-write segment
    movw    %ax, %ds
    movw    %ax, %es
    movw    %ax, %fs
    movw    %ax, %gs
    movw    %ax, %ss

# bootpack.c transfer 
    movl    $bootpack, %esi     # transfer source
    movl    $BOTPAK, %edi       # transfer destination
    movl    $512*1024/4, %ecx   # size
    call    _memcpy

# transfer disk data too
# at first, boot sector
    movl    $0x7c00, %esi       # transfer source
    movl    $DSKCAC, %edi       # transfer destination
    movl    $512/4, %ecx
    call    _memcpy
 
# all the rest
    movl    $DSKCAC0+512, %esi  # transfer source
    movl    $DSKCAC+512, %edi   # transfer destination
    movl    $0, %ecx
    movb    CYLS, %cl
    imul    $512*18*2/4, %ecx   # calcurate the number of memory address that be allocated
    subl    $512/4, %ecx        # subtract address be used to the ipl(mbr)
    call    _memcpy

# the end of the main process of asmhead.S
# start-up of bootpack.c
#   In HariboteOS, any applications be formated as the ".hrb" type,
#   so the code below was written according to the ".hrb" rule.
    movl    $BOTPAK, %ebx
    movl    16(%ebx), %ecx
    addl    $3, %ecx
    shr     $2, %ecx            # the instruction which shift right
    jz      skip                # if nothing should be transferred is exist, jump to "skip"
    movl    20(%ebx), %esi      # transfer source
    addl    %ebx, %esi          #    is 0x00280000 + .data's position in a file
    movl    12(%ebx), %edi      # transfer destination
    call    _memcpy

skip:
    movl    12(%ebx), %esp      # set stack-pointer to the initalizative address
    ljmpl   $0x10, $0x0000001b  # jump to ../kernel/bootpack.c

waitkbdout:
    in      $0x64, %al
    andb    $0x02, %al
    jnz     waitkbdout
    ret

_memcpy:
    movl    (%esi), %eax
    addl    $4, %esi
    movl    %eax, (%edi)
    addl    $4, %edi
    subl    $1, %ecx
    jnz     _memcpy
    ret

    .align  16

# if the GDT doesn't exist in the address which is a multiple of 8, 
# the i386 processor will work at low performance

GDT0:
    .space  8, 0                # null selecter
    .word   0xffff, 0x0000, 0x9200, 0x00cf  # read-write segment
    .word   0xffff, 0x0000, 0x9a28, 0x0047  # executable segment

    .word   0x0000
    
GDTR0:
    .word   8*3-1
    .long   GDT0

    .align  16

bootpack:

