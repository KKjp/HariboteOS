include ../Makefile.inc

SRC_MBR := haribote.S
OUTPUT := ../$(KERNEL)
OBJFILE := ../$(OUT_DIR)/haribote.o
BINFILE := ../$(OUT_DIR)/haribote.bin

default: kernel

kernel: $(SRC_MBR) $(OUTPUT)
	$(AS) $(ASFLAGS) -o $(OBJFILE) $(SRC_MBR)
	$(LD) $(LDFLAGS) -Ttext=0xc200 -o $(BINFILE) $(OBJFILE)
	$(OBJCOPY) $(OCFLAGS) $(BINFILE) $(OUTPUT)

$(OUTPUT):
	touch $(OUTPUT)

