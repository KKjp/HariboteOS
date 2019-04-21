include ../Makefile.inc

CFLAGS_TOOLS += -Wall -O2 -g -std=gnu99

SRC_MAKEFONT := makefont.c
OBJ_MAKEFONT := ../$(TOOL_DIR)/makefont

default: tools

tools:
	$(CC) $(CFLAGS_TOOLS) -o $(OBJ_MAKEFONT) $(SRC_MAKEFONT)

