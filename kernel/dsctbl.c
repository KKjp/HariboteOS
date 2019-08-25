/* kernel/dsctbl.c */

#include <stdint.h>
#include "../include/osutils.h"
#include "asmfunc.h"
#include "dsctbl.h"

void init_gdtidt(void)
{
    segment_descriptor *gdt = (segment_descriptor *) 0x00270000;
    gate_descriptor    *idt = (gate_descriptor *)    0x0026f800;

    /* initialize the gdt */
    memset(gdt, 0, sizeof(segment_descriptor) * 8192);
    /* a data segment: expand upward, reading and writing is enable, ring0, present on the memory, used for 16bit, the limit will be dealt with multiplied by 4KB value */
    set_segmdesc(gdt + 1, 0xffffffff, 0x00000000, DATA_SEGMENT | EXPAND_UPWARD | READABLE_AND_WRITEABLE | NOT_ACCESSED /* 0b0010 */,
            DATA_OR_CODE_SEGMENT /* 1 */, 0, SEGMENT_PRESENTS_ON_MEMORY /* 1 */, 0, SEGMENT_FOR_16BIT /* 0 */, THE_LIMIT_IS_INTERPRETED_AS_MULTIPLIED_BY_4KB_IS /* 1 */);
    /* a code segment: nonconformig - does not be run from low privilege level, reading and executing is enable, ring0, present on the memory, used for 16bit, the limit will be dealt with multiplied by 4KB value */
    set_segmdesc(gdt + 2, 0x0007ffff, 0x00280000, CODE_SEGMENT | NON_CONFORMING | EXECUTABLE_AND_READABLE | NOT_ACCESSED /* 0b1010 */,
            DATA_OR_CODE_SEGMENT /* 1 */, 0, SEGMENT_PRESENTS_ON_MEMORY /* 1 */, 0, SEGMENT_FOR_16BIT /* 0 */, THE_LIMIT_IS_INTERPRETED_AS_MULTIPLIED_BY_4KB_IS /* 1 */);
    _lgdt((uint16_t) (8192 * sizeof(segment_descriptor)), 0x00270000);

    memset(idt, 0, sizeof(gate_descriptor) * 256);
    _lidt((uint16_t) (256 * sizeof(gate_descriptor) - 1), 0x0026f800);
}

void set_segmdesc(segment_descriptor *sd, uint32_t limit, const uint32_t base, const uint8_t type, const uint8_t S, const uint8_t DPL, const uint8_t P, const uint8_t AVL, const uint8_t D_B, uint8_t G)
{
    if (limit > 0xfffff) {
        G = 1;
        limit /= 0x1000; /* the limit will be dealt with multiplied by 4 KB value */
    }
    sd->limit_low  =  limit & 0x0000ffff;
    sd->base_low   =  base  & 0x0000ffff;
    sd->base_mid   = (base  & 0x00ff0000) >> 16;
    sd->type       =  type  & 0b00001111;
    sd->S          =  S     & 0b00000001;
    sd->DPL        =  DPL   & 0b00000001;
    sd->limit_high = (limit & 0xffff0000) >> 16;
    sd->AVL        =  AVL   & 0b00000001;
    sd->const_0    =  0;
    sd->D_B        =  D_B   & 0b00000001;
    sd->G          =  G     & 0b00000001;
    sd->base_high  = (base  & 0xff000000) >> 24;
}

void set_gatedesc(gate_descriptor *gd, const uint32_t offset, const uint16_t selector, const uint8_t type, const uint8_t D, const uint8_t DPL, const uint8_t P)
{
    gd->offset_low  =  offset & 0x0000ffff;
    gd->selector    =  selector;
    gd->unused      =  0b000;
    gd->type        =  type   & 0b00001111;
    gd->D           =  D      & 0b00000001;
    gd->const_0     =  0;
    gd->DPL         =  DPL    & 0b00000011;
    gd->P           =  P      & 0b00000001;
    gd->offset_high = (offset & 0xffff0000) >> 16;
}

