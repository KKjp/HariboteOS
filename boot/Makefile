include ../Makefile.inc

SRC_MBR := ipl10.S
OUTPUT := ../$(MBR)
OBJFILE := ../$(OUT_DIR)/ipl10.o
BINFILE := ../$(OUT_DIR)/ipl10.bin

default: mbr

mbr: $(SRC_MBR) $(OUTPUT)
	$(AS) $(ASFLAGS) -o $(OBJFILE) $(SRC_MBR)
	$(LD) $(LDFLAGS) -Ttext=0x7c00 -o $(BINFILE) $(OBJFILE)
	$(OBJCOPY) $(OCFLAGS) $(BINFILE) $(OUTPUT) 

# -Ttext=0x7c00 - .textセクションが0x7c00に読み込まれるとしてアセンブラのアドレスを計算する

$(OUTPUT):
	touch $(OUTPUT)

