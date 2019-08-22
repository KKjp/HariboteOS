/* kernel/dsctbl.c */

#include <stdint.h>
#include "../include/osutils.h"

typedef struct {
    uint64_t limit_low: 16;
    uint64_t base_low: 16;
    uint64_t base_mid: 16;
    uint64_t type: 4;
    uint64_t S: 1;
    uint64_t DPL: 2;
    uint64_t P: 1;
    uint64_t limit_high: 4;
    uint64_t AVL: 1;
    uint64_t : 1;
    uint64_t D_B: 1;
    uint64_t G: 1;
    uint64_t base_high: 16;
} __attribute__((packed)) segment_desriptor;

typedef struct {
    uint64_t offset_low: 16;
    uint64_t selector: 16;
    uint64_t : 5;
    uint64_t unused: 3;
    uint64_t type: 3;
    uint64_t D: 1;
    uint64_t const_0: 1;
    uint64_t DPL: 2;
    uint64_t P: 1;
    uint64_t offset_high: 16;
} __attribute__((packed)) gate_descriptor;

void set_segmdesc(segment_desriptor *, uint32_t, const uint32_t, const uint8_t, const uint8_t, const uint8_t, const uint8_t, const uint8_t, const uint8_t, uint8_t);
void set_gatedesc(gate_descriptor *, const uint32_t, const uint16_t, const uint8_t, const uint8_t, const uint8_t, const uint8_t);

void init_gdtidt(void)
{
    segment_desriptor *gdt = (segment_descriptor *) 0x00270000;
    gate_descriptor   *idt = (gate_descriptor *)    0x0026f800;

    /* initialize the gdt */
    memset(gdt, 0, sizeof(segment_desriptor) * 8192);
    /* a data segment: expand upward, reading and writing is enable, ring0, present on the memory, used for 16bit, the limit will be dealt with multiplied by 4KB value */
    set_segmdesc(gdt + 1, 0xffffffff, 0x00000000, 0b0010, 0b1, 0b00, 0b1, 0b0, 0b0, 0b1);
    /* a code segment: nonconformig - does not be run from low privilege level, reading and executing is enable, ring0, present on the memory, used for 16bit, the limit will be dealt with multiplied by 4KB value */
    set_segmdesc(gdt + 2, 0x0007ffff, 0x00280000, 0b1010, 0b1, 0b00, 0b1, 0b0, 0b0, 0b1);
}

void set_segmdesc(segment_desriptor *sd, uint32_t limit, const uint32_t base, const uint8_t type, const uint8_t S, const uint8_t DPL, const uint8_t P, const uint8_t AVL, const uint8_t D_B, uint8_t G)
{
    if (limit > 0xfffff) {
        G = 1;
        limit /= 0x1000; /* the limit will be dealt with multiplied by 4 KB value */
    }
    sd->limit_low  = limit & 0x0000ffff;
    sd->base_low   = base  & 0x0000ffff;
    sd->base_mid   = base  & 0x00ff0000;
    sd->type       = type  & 0b00001111;
    sd->S          = S     & 0b00000001;
    sd->DPL        = DPL   & 0b00000001;
    sd->limit_high = limit & 0xffff0000;
    sd->AVL        = AVL   & 0b00000001;
    sd->D_B        = D_B   & 0b00000001;
    sd->G          = G     & 0b00000001;
    sd->base_high  = base  & 0xffff0000;
}

void set_gatedesc(gate_descriptor *gd, const uint32_t offset, const uint16_t selector, const uint8_t type, const uint8_t D, const uint8_t DPL, const uint8_t P)
{
    gd->offset_low  = offset & 0x0000ffff;
    gd->selector    = selector;
    gd->unused      = 0b000;
    gd->type        = type   & 0b00001111;
    gd->D           = D      & 0b00000001;
    gd->const_0     = 0b0;
    gd->DPL         = DPL    & 0b00000011;
    gd->P           = P      & 0b00000001;
    gd->offset_high = offset & 0xffff0000;
}

