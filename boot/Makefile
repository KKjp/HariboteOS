include ../Makefile.inc

SRC_MBR := helloos03.S
OUTPUT := ../$(MBR)
OBJFILE := ../$(OUT_DIR)/helloos03.o
BINFILE := ../$(OUT_DIR)/helloos03.bin

all: mbr

mbr: $(SRC_MBR) $(OUTPUT)
	$(AS) $(ASFLAGS) -o $(OBJFILE) $(SRC_MBR)
	$(LD) $(LDFLAGS) -o $(BINFILE) $(OBJFILE)
	$(OBJCOPY) $(OCFLAGS) $(BINFILE) $(OUTPUT) 

