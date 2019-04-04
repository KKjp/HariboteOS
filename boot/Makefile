include ../Makefile.inc

SRC_MBR := ipl10.S
OBJ_MBR := ../$(OUT_DIR)/ipl10.o
OBJ2_MBR := ../$(OUT_DIR)/ipl10.o2
BIN_MBR := ../$(MBR)

SRC_ASMHEAD := asmhead.S
OBJ_ASMHEAD := ../$(OUT_DIR)/asmhead.o
OBJ2_ASMHEAD := ../$(OUT_DIR)/asmhead.o2
BIN_ASMHEAD := ../$(OUT_DIR)/asmhead.bin

default: mbr asmhead

mbr: $(OBJ_MBR)
	$(LD) $(LDFLAGS) -T/dev/null --oformat binary -Ttext=0x7c00 -o $(BIN_MBR) $(OBJ_MBR)

$(OBJ_MBR):
	$(AS) $(ASFLAGS) -o $(OBJ_MBR) $(SRC_MBR)

# -Ttext=0x7c00 - .textセクションが0x7c00に読み込まれるとしてアセンブラのアドレスを計算する

asmhead: $(OBJ_ASMHEAD)
	$(LD) $(LDFLAGS) -T/dev/null --oformat binary -Ttext=0xc200 -o $(BIN_ASMHEAD) $(OBJ_ASMHEAD)

$(OBJ_ASMHEAD):
	$(AS) $(ASFLAGS) -o $(OBJ_ASMHEAD) $(SRC_ASMHEAD)

