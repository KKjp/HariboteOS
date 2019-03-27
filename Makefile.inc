CC := gcc 
CXX := g++ 
AS := as
QEMU := qemu-system-i386
OBJCOPY := objcopy

OUT_DIR := build
INC_DIR := include
IMG := $(OUT_DIR)/HariboteOS.img
KERNEL := $(OUT_DIR)/kernel
MBR := $(OUT_DIR)/mbr
       
CFLAG := -Wall -nostdlib -fno-pic -fno-pie -nostartfiles -g -std=gnu99 -ffreestanding -O2 -I $(INC_DIR)
CXXFLAGS := -Wall -nostdlib -fno-pic -fno-pie -nostartfiles -g -std=c++11 -ffreestanding -O2 -I $(INC_DIR) 
ASFLAGS := -Wall -nostdlib -mtune=i386
LDFLAGS := -nostdlib -g
QFLAGS := -drive
OCFLAGS := -O binary --only-section=.text 

