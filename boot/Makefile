include ../Makefile.inc

SRC_MBR := helloos03.S
OUTPUT := ../$(MBR)
OBJFILE := ../$(OUT_DIR)/helloos03.o

all: mbr

mbr: $(SRC_MBR) $(OUTPUT)
	$(AS) $(ASFLAGS) -o $(OBJFILE) $(SRC_MBR)
	$(OBJCOPY) $(OCFLAGS) $(OUTPUT) $(OBJFILE)

