.code16
.text
harimain:
    movb    $0x13, %al      # VGA graphics 320*200, 8bit color
    movb    $0x00, %ah      # video bios setting mode
    int     $0x10           # call video bios

fin:
    hlt
    jmp     fin

