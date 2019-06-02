include Makefile.mk

.DEFAULT_GOAL := img

# makeを引数なしで実行した場合に実行される
# 引数なしの場合、img:を実行する

.PHONY: clean run img

# カレントディレクトリにclean, run, imgというファイルがあっても実行する
#
###########################################################################
# FORCE:
# .PHONY: FORCE
# とやると、例えばclean: FORCEなどと指定すればcleanはmake実行時に必ず実行される

img:
	$(MAKE) -C tools
	$(MAKE) -C include
	$(MAKE) -C lib
	$(MAKE) -C data
	$(MAKE) $(IMG)

$(IMG): $(MBR) $(KERNEL)    # $(IMG)を作るためには$(MBR)と$(KERNEL)が必要
	mformat -f 1440 -C -B $(MBR) -i $@ ::
	mcopy -i $@ $(KERNEL) ::

# mformat - floppy-diskイメージを作るコマンド
#   -f 1440 - 1440KBディスクを指定
#   -C - MS-DOSファイルシステムを作成する
#        これでディスクイメージの0x200と0x1400のf0ffffff0fを書いている
#   -B $(MBR) - $(MBR)=build/HariboteOS,mbrをブートセクタに指定する
#   -i $@ - $@は作りたいファイル名(:の前の$(IMG))を表すマクロ
#           -iは出力イメージファイルの指定
# mcopy - floppy-diskイメージにファイルを追加するコマンド
#   -i - ディスクイメージの指定
#
# :: - よくわかんないけどないと動かない

$(MBR):
	$(MAKE) -C boot     # -C boot - ./boot/に移動し、そこにあるMakefileを実行する

$(KERNEL):
	$(MAKE) -C kernel

run:
	$(MAKE) img
	$(QEMU) $(QFLAGS) -drive file=$(IMG),format=raw,if=floppy

# 展開すると
# qemu-system-i386 -m 32 -localtime -vga std -boot a -drive file=build/HariboteOS.img,format=raw,if=floppy
#   -m 32 - 32MiBのメモリを使うという意味
#   -localtime - ゲストOSの時間にホストOSの時間を利用する
#   -vga std - VGAグラフィックスモード(スタンダード, 1280*1024:16bit-color)
#   -boot a - floppy-diskを最初にブートする
#   -drive ... - "-fda"オプションで単純にフロッピーイメージを指定しただけだと起動しなかったので
#                より詳細に設定できるこのオプションを使った。

debug: $(IMG)
	hexdump -C $^
	objdump -D -b binary -m i8086 --start-address=0x50 $^

debugq: $(IMG)
	qemu-system-i386 -m 32 -localtime -vga std -boot a -drive file=$(IMG),format=raw,if=floppy -gdb tcp::10000 -S &
	gdb --eval-command="target remote localhost:10000"

clean:
	find $(OUT_DIR) -type f -print | xargs rm -f

develop:
	gcc -march=i386 -m32 -Wall -nostdlib -fno-pic -fno-pie -nostartfiles -g -std=gnu99 -ffreestanding -c -O2 -o ./build/HariboteOS/sprintf.o ./include/sprintf.c
	gcc -march=i386 -m32 -Wall -nostdlib -fno-pic -fno-pie -nostartfiles -g -std=gnu99 -ffreestanding -c -O2 -o ./build/HariboteOS/memset.o  ./include/memset.c
	gcc -march=i386 -m32 -Wall -nostdlib -fno-pic -fno-pie -nostartfiles -g -std=gnu99 -ffreestanding -c -O2 -o ./build/HariboteOS/strlen.o  ./include/strlen.c
	gcc -march=i386 -m32 -Wall -nostdlib -fno-pic -fno-pie -nostartfiles -g -std=gnu99 -ffreestanding -c -O2 -o ./build/HariboteOS/strcat.o  ./include/strcat.c
	gcc -march=i386 -m32 -Wall -nostdlib -fno-pic -fno-pie -nostartfiles -g -std=gnu99 -ffreestanding -c -O2 -o ./build/HariboteOS/strncat.o ./include/strncat.c
	ld -g -nostdlib -T./kernel/kernel.x -o\
        ./build/HariboteOS/bootpack.hrb\
        ./build/HariboteOS/bootpack.o\
        ./build/HariboteOS/_bootpack.o\
        ./build/HariboteOS/sprintf.o\
        ./build/HariboteOS/memset.o\
        ./build/HariboteOS/strlen.o\
        ./build/HariboteOS/strcat.o\
        ./build/HariboteOS/strncat.o\
        ./build/HariboteOS/hankaku.o

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
