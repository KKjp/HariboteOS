helloos02.elf: helloos02.S
	as -mtune=i386 -o $@ $^

helloos02.bin: helloos02.elf
	objcopy -O binary --only-section=.text $^ $@

helloos02.img: helloos02.bin
	mformat -f 1440 -C -B $^ -i $@

run: helloos02.img
	qemu-system-i386 -boot a $^

