include ../Makefile.inc

CFLAGS        += -O2

BIN_KEN       := ../$(KERNEL)

SRC_BOTPAKS   := _bootpack.S
OBJ_BOTPAKS   := ../$(OS_BIN_DIR)/_bootpack.o
BIN_BOTPAKS   := ../$(OS_BIN_DIR)/_bootpack.bin

SRC_BOTPAKC   := bootpack.c
OBJ_BOTPAKC   := ../$(OS_BIN_DIR)/bootpack.o
BIN_BOTPAKC   := ../$(OS_BIN_DIR)/bootpack.bin

HRB_BOTPAK    := ../$(OS_BIN_DIR)/bootpack.hrb

KERNEL_LOADER := ../$(OS_BIN_DIR)/asmhead.bin
FONT_DATA     := ../$(OS_BIN_DIR)/hankaku.o

default: kernel

kernel: $(HRB_BOTPAK) $(KERNEL_LOADER)
	cat ../$(OS_BIN_DIR)/asmhead.bin $(HRB_BOTPAK) > $(BIN_KEN)

$(HRB_BOTPAK): $(OBJ_BOTPAKS) $(OBJ_BOTPAKC) $(FONT_DATA)
	$(LD) $(LDFLAGS) -T./kernel.lds -o $@ $(OBJ_BOTPAKC) $(OBJ_BOTPAKS) $(FONT_DATA)

$(OBJ_BOTPAKS):
	$(AS) $(ASFLAGS) -o $@ $(SRC_BOTPAKS)

$(OBJ_BOTPAKC):
	$(CC) $(CFLAGS) -c -o $@ $(SRC_BOTPAKC)

$(KERNEL_LOADER):
	$(MAKE) asmhead -C ../boot/

$(FONT_DATA):
	$(MAKE) font -C ../include/

