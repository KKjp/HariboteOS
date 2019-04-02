CC := gcc 
CXX := g++ 
AS := as
LD := ld
QEMU := qemu-system-i386
OBJCOPY := objcopy

OUT_DIR := build
INC_DIR := include
BINFILE := $(OUT_DIR)/HariboteOS.bin
MBR := $(OUT_DIR)/HariboteOS.mbr
KERNEL := $(OUT_DIR)/HariboteOS.ken
IMG := $(OUT_DIR)/HariboteOS.img
       
CFLAG := -m 32 -Wall -nostdlib -fno-pic -fno-pie -nostartfiles -g -std=gnu99 -ffreestanding -O0 -I $(INC_DIR)
CXXFLAGS := -m 32 -Wall -nostdlib -fno-pic -fno-pie -nostartfiles -g -std=c++11 -ffreestanding -O0 -I $(INC_DIR) 
ASFLAGS := -nostdlib -mtune=i386
LDFLAGS := -g -T/dev/null
QFLAGS := -m 32 -localtime -vga std -boot a
OCFLAGS := -O binary 

