CC          := gcc
CXX         := g++ 
AS          := as
LD          := ld
QEMU        := qemu-system-i386
OBJCOPY     := objcopy
            
OUT_DIR     := build
INC_DIR     := include
OS_BIN_DIR  := $(OUT_DIR)/HariboteOS
TOOL_DIR    := $(OUT_DIR)/tools
LIB_DIR     := $(OUT_DIR)/lib/lib
LIB_OBJ_DIR := $(OUT_DIR)/lib/obj
MBR         := $(OS_BIN_DIR)/ipl10.bin
KERNEL      := $(OS_BIN_DIR)/kernel.bin
IMG         := $(OS_BIN_DIR)/HariboteOS.img
            
CFLAGS      := -march=i386 -m32 -Wall -nostdlib -fno-pic -fno-pie -nostartfiles -g -std=gnu99 -ffreestanding
CXXFLAGS    := -march=i386 -m32 -Wall -nostdlib -fno-pic -fno-pie -nostartfiles -g -std=g++11 -ffreestanding
ASFLAGS     := -nostdlib --32 -mtune=i386 --traditional-format -n
LDFLAGS     := -g -nostdlib -L$(LIB_DIR)
QFLAGS      := -m 32 -localtime -vga std -boot a
OCFLAGS     := -O binary 

