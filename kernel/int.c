/* kernel/int.c */

#include <stdint.h>
#include "bootpack.h"

void init_pic(void)
{
    _io_out8(PIC_MASTER_IMR, 0b11111111);  /* all interrupt aren't accepted */
    _io_out8(PIC_SLAVE_IMR, 0b11111111);   /* all interrupt aren't accepted */

    _io_out8(PIC_MASTER_ICW1, ICW1_IRQ_EDGETRIGER | ICW1_ICW4_ENABLE | ICW1_RESERVED);
    _io_out8(PIC_MASTER_ICW2, 0x20);       /* IRQ0-7 is received by int32-39 */
    _io_out8(PIC_MASTER_ICW3, 0b00000100); /* slave PIC is connected to IRQ2 */
    _io_out8(PIC_MASTER_ICW4, ICW4_X86MODE | ICW4_AEOI_AUTO | ICW4_BUFFER_MASTER | ICW4_NON_BUFFER_MODE | ICW4_IRQ_NEST_DISABLE | ICW4_RESERVED);

    _io_out8(PIC_SLAVE_ICW1, ICW1_IRQ_EDGETRIGER | ICW1_ICW4_ENABLE | ICW1_RESERVED);
    _io_out8(PIC_SLAVE_ICW2, 0x28);        /* IRQ8-15 is received by int40-47 */
    _io_out8(PIC_SLAVE_ICW3, 2);           /* slave PIC is connected to IRQ2 */
    _io_out8(PIC_SLAVE_ICW4, ICW4_X86MODE | ICW4_AEOI_AUTO | ICW4_BUFFER_MASTER | ICW4_NON_BUFFER_MODE | ICW4_IRQ_NEST_DISABLE | ICW4_RESERVED);

    _io_out8(PIC_MASTER_IMR, 0b11111111);  /* all interrupt aren't accepted */
    _io_out8(PIC_SLAVE_IMR, 0b11111111);   /* all interrupt aren't accepted */
}

void inthandler21(const uint16_t *esp)
{
    boxfill8(binfo->vram, binfo->scrnx, BLACK, 0, 0, 32 * 8 - 1, 15);
    putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, WHITE, "INT 21 (IRQ-1) : PS/2 keyboard");
    for (;;) {
        _io_hlt();
    }
}

void inthandler2c(const uint16_t *esp)
{
    boxfill8(binfo->vram, binfo->scrnx, BLACK, 0, 0, 32 * 8 - 1, 15);
    putfonts8_asc(binfo->vram, binfo->scrnx, 0, 0, WHITE, "INT 2C (IRQ-12) : PS/2 mouse");
    for (;;)  {
        _io_hlt();
    }
}

void inthandler27(const uint16_t *esp)
{
    _io_out8(PIC_MASTER_OCW2, 0x67); /* send a notification that an interrupt have been dealt with already */
}

