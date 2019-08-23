/* kernel/dsctbl.h */

#ifndef DSCTBL_H_INCLUDED
#define DSCTBL_H_INCLUDED

#include <stdint.h>

typedef struct {
    uint64_t limit_low: 16;
    uint64_t base_low: 16;
    uint64_t base_mid: 8;
    uint64_t type: 4;
    uint64_t S: 1;
    uint64_t DPL: 2;
    uint64_t P: 1;
    uint64_t limit_high: 4;
    uint64_t AVL: 1;
    uint64_t const_0: 1;
    uint64_t D_B: 1;
    uint64_t G: 1;
    uint64_t base_high: 8;
} __attribute__((packed)) segment_descriptor;

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

extern void init_gdtidt(void);
extern void set_segmdesc(segment_descriptor *, uint32_t, const uint32_t, const uint8_t, const uint8_t, const uint8_t, const uint8_t, const uint8_t, const uint8_t, uint8_t);
extern void set_gatedesc(gate_descriptor *, const uint32_t, const uint16_t, const uint8_t, const uint8_t, const uint8_t, const uint8_t);

#endif /* DSCTBL_H_INCLUDED */

