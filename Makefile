include Makefile.inc

all: img

img:
	$(MAKE) $(IMG)

$(IMG): $(MBR)
	mformat -f 1440 -C -B $(MBR) -i $@

$(MBR):
	$(MAKE) -C boot

run: $(IMG)
	$(QEMU) $(QFLAGS) $(IMG)

debug: $(IMG)
	hexdump -C $^
	objdump -D -b binary -m i8086 --start-address=0x50 $^

# helloos02.o: helloos02.S
# 	as -mtune=i386 -o $@ $^

# helloos02.bin: helloos02.o
# 	objcopy -O binary --only-section=.text $^ $@

# helloos02.img: helloos02.bin
# 	mformat -f 1440 -C -B $^ -i $@

# run: helloos02.img
# 	qemu-system-i386 -drive file=$^,format=raw,if=floppy -boot a

# debug: helloos02.img
# 	hexdump -C $^
# 	objdump -D -b binary -m i8086 --start-address=0x50 $^

# 使えそうなコマンド
# dd - disk convert.
# fdisk - set disk partition.
# mcopy - copy file from ext2 to fat--disk-image-file.
# objdump -D -b binary -m i386:x86-64 --start-address=[address(16進数)] [file] - disassemble binary file(i386).
# objdump -D -b binary -m i8086 --start-address=[address(16進数)] [file] - disassemble binary file(i8086).
