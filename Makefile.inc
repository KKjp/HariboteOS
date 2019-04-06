CC := gcc 
CXX := g++ 
AS := as
LD := ld
QEMU := qemu-system-i386
OBJCOPY := objcopy

OUT_DIR := build
INC_DIR := include
MBR := $(OUT_DIR)/ipl10.bin
KERNEL := $(OUT_DIR)/kernel.bin
IMG := $(OUT_DIR)/HariboteOS.img
       
CFLAGS := -march=i386 -m32 -Wall -nostdlib -fno-pic -fno-pie -nostartfiles -g -std=gnu99 -ffreestanding -O0
CXXFLAGS := -m32 -Wall -nostdlib -fno-pic -fno-pie -nostartfiles -g -std=c++11 -ffreestanding -O0 -I $(INC_DIR) 
ASFLAGS := -nostdlib --32 -mtune=i386 --traditional-format -n
LDFLAGS := -g -nostdlib
QFLAGS := -m 32 -localtime -vga std -boot a
OCFLAGS := -O binary 

