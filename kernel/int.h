/* kernel/int.h */

#ifndef INT_H_INCLUDED
#define INT_H_INCLUDED
#include <stdint.h>

extern void init_pic(void);
extern void inthandler21(const uint16_t *);
extern void inthandler27(const uint16_t *);
extern void inthandler2c(const uint16_t *);

#define PIC_MASTER_ICW1       0x0020
#define PIC_MASTER_OCW2       0x0020
#define PIC_MASTER_IMR        0x0021
#define PIC_MASTER_ICW2       0x0021
#define PIC_MASTER_ICW3       0x0021
#define PIC_MASTER_ICW4       0x0021
#define PIC_SLAVE_ICW1        0x00a0
#define PIC_SLAVE_OCW2        0x00a0
#define PIC_SLAVE_IMR         0x00a1
#define PIC_SLAVE_ICW2        0x00a1
#define PIC_SLAVE_ICW3        0x00a1
#define PIC_SLAVE_ICW4        0x00a1

#define ICW1_ICW4_ENABLE      0b00000001
#define ICW1_ICW4_DISABLE     0b00000000
#define ICW1_PIC_CASCADE      0b00000000 /* another PIC is connected to an IRQ of a PIC */
#define ICW1_PIC_NON_CASCADE  0b00000010
#define ICW1_IRQ_EDGETRIGER   0b00000000
#define ICW1_IRQ_LEVELTRIGER  0b00001000
#define ICW1_RESERVED         0b00010000

#define ICW4_X86MODE          0b00000001
#define ICW4_MCS8085MODE      0b00000000
#define ICW4_AEOI_AUTO        0b00000000
#define ICW4_AEOI_MANUAL      0b00000010
#define ICW4_BUFFER_MASTER    0b00000000
#define ICW4_BUFFER_SLAVE     0b00000100
#define ICW4_BUFFER_MODE      0b00010000
#define ICW4_NON_BUFFER_MODE  0b00000000
#define ICW4_IRQ_NEST_DISABLE 0b00000000
#define ICW4_IRQ_NEST_ENABLE  0b00100000
#define ICW4_RESERVED         0b00000000

#endif /* INT_H_INCLUDED */

