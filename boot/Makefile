include ../Makefile.mk

.DEFAULT_GOAL := default
.PHONY: mbr asmhead

CFLAGS      += -ffreestanding -O0

SRC_MBR     := ipl10.S
OBJ_MBR     := ../$(OS_BIN_DIR)/ipl10.o
BIN_MBR     := ../$(MBR)

SRC_ASMHEAD := asmhead.S
OBJ_ASMHEAD := ../$(OS_BIN_DIR)/asmhead.o
BIN_ASMHEAD := ../$(OS_BIN_DIR)/asmhead.bin

default: mbr asmhead

mbr: $(OBJ_MBR)
	$(LD) $(LDFLAGS) -T./mbr.x -o $(BIN_MBR) $^

$(OBJ_MBR):
	$(AS) $(ASFLAGS) -o $@ $(SRC_MBR)

# -Ttext=0x7c00 - プログラムはこのアドレスにロードされるものとし、アドレス計算も
#                 この値に基づいて行われる

asmhead: $(OBJ_ASMHEAD)
	$(LD) $(LDFLAGS) -T./asmhead.x -o $(BIN_ASMHEAD) $^

$(OBJ_ASMHEAD):
	$(AS) $(ASFLAGS) -o $@ $(SRC_ASMHEAD)

