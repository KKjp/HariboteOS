include ../Makefile.mk

.DEFAULT_GOAL := kernel
.PHONY: kernel

BIN_KEN       := ../$(KERNEL)

SRCS          := $(wildcard *.c)
OBJS          := $(addprefix ../$(OS_BIN_DIR)/, $(patsubst %.c, %.o, $(SRCS)))

HRB_BOTPAK    := ../$(OS_BIN_DIR)/bootpack.hrb

KERNEL_LOADER := ../$(OS_BIN_DIR)/asmhead.bin
FONT_DATA     := ../$(OS_BIN_DIR)/hankaku.o

LIBRARY       := ../$(LIB_DIR)/libosutils.a


kernel: $(HRB_BOTPAK) $(KERNEL_LOADER)
	cat $(KERNEL_LOADER) $(HRB_BOTPAK) > $(BIN_KEN)

$(HRB_BOTPAK): $(OBJS) $(FONT_DATA) $(LIBRARY)
	$(LD) $(LDFLAGS) -T./kernel.x -o $@ $^

../$(OS_BIN_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c -O2 -o $@ $^

../$(OS_BIN_DIR)/dsctbl.o: dsctbl.c
	$(CC) $(CFLAGS) -c -O0 -o $@ $^

$(KERNEL_LOADER):
	$(MAKE) asmhead -C ../boot/

$(FONT_DATA):
	$(MAKE) font -C ../data/

$(LIBRARY):
	$(MAKE) osutils -C ../lib/

