include ../Makefile.inc

CFLAGS += -ffreestanding

SRC_MBR := ipl10.S
OBJ_MBR := ../$(OUT_DIR)/ipl10.o
BIN_MBR := ../$(MBR)

SRC_ASMHEAD := asmhead.S
OBJ_ASMHEAD := ../$(OUT_DIR)/asmhead.o
BIN_ASMHEAD := ../$(OUT_DIR)/asmhead.bin

default: mbr asmhead

mbr: $(OBJ_MBR)
	$(LD) $(LDFLAGS) -T./mbr.lds -o $(BIN_MBR) $(OBJ_MBR)

$(OBJ_MBR):
	$(AS) $(ASFLAGS) -o $@ $(SRC_MBR)

# -Ttext=0x7c00 - .textセクションが0x7c00に読み込まれるとしてアセンブラのアドレスを計算する

asmhead: $(OBJ_ASMHEAD)
	$(LD) $(LDFLAGS) -T./asmhead.lds -o $(BIN_ASMHEAD) $(OBJ_ASMHEAD)

$(OBJ_ASMHEAD):
	$(AS) $(ASFLAGS) -o $@ $(SRC_ASMHEAD)

