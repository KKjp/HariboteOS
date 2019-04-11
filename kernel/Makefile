include ../Makefile.inc

CFLAGS += -O2

BIN_KEN := ../$(KERNEL)

SRC_BOTPAKS := _bootpack.S
OBJ_BOTPAKS := ../$(OUT_DIR)/_bootpack.o
BIN_BOTPAKS := ../$(OUT_DIR)/_bootpack.bin

SRC_BOTPAKC := bootpack.c
OBJ_BOTPAKC := ../$(OUT_DIR)/bootpack.o
BIN_BOTPAKC := ../$(OUT_DIR)/bootpack.bin

HRB_BOTPAK := ../$(OUT_DIR)/bootpack.hrb

default: kernel

kernel: $(HRB_BOTPAK) ../$(OUT_DIR)/asmhead.bin
	cat ../$(OUT_DIR)/asmhead.bin $(HRB_BOTPAK) > $(BIN_KEN)

$(HRB_BOTPAK): $(OBJ_BOTPAKS) $(OBJ_BOTPAKC)
	$(LD) $(LDFLAGS) -T./kernel.lds -o $@ $(OBJ_BOTPAKC) $(OBJ_BOTPAKS)

$(OBJ_BOTPAKS):
	$(AS) $(ASFLAGS) -o $@ $(SRC_BOTPAKS)

$(OBJ_BOTPAKC):
	$(CC) $(CFLAGS) -c -o $@ $(SRC_BOTPAKC)

../$(OUT_DIR)/asmhead.bin:
	$(MAKE) asmhead -C ../boot

