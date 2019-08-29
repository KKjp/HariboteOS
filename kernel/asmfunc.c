/* kernel/asmfunc.c */

void _asm_inthandler21(void)
{
    asm volatile (
        "pushw %es;"
        "pushw %ds;"
        "pushal;"
        "movl %esp, %eax;"
        "pushl %eax;"
        "movw %ss, %ax;"
        "movw %ax, %ds;"
        "movw %ax, %es;"
        "call inthandler21;"
        "popl %eax;"
        "popal;"
        "popw %ds;"
        "popw %es;"
        "iretl;"
    );
}

void _asm_inthandler27(void)
{
    asm volatile (
        "pushw %es;"
        "pushw %ds;"
        "pushal;"
        "movl %esp, %eax;"
        "pushl %eax;"
        "movw %ss, %ax;"
        "movw %ax, %ds;"
        "movw %ax, %es;"
        "call inthandler27;"
        "popl %eax;"
        "popal;"
        "popw %ds;"
        "popw %es;"
        "iretl;"
    );
}

void _asm_inthandler2c(void)
{
    asm volatile (
        "pushw %es;"
        "pushw %ds;"
        "pushal;"
        "movl %esp, %eax;"
        "pushl %eax;"
        "movw %ss, %ax;"
        "movw %ax, %ds;"
        "movw %ax, %es;"
        "call inthandler2c;"
        "popl %eax;"
        "popal;"
        "popw %ds;"
        "popw %es;"
        "iretl;"
    );
}

