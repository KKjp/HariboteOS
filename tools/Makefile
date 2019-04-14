include ../Makefile.inc

CFLAGS_TOOLS += -Wall -O2

SRC_MAKEFONT := makefile.c
OBJ_MAKEFONT := ../$(TOOL_DIR)/makefont

default: tools

tools:
	$(CC) $(CFLAGS_TOOLS) -o $(OBJ_MAKEFONT) $(SRC_MAKEFONT)

