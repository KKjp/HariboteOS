include ../Makefile.mk

.DEFAULT_GOAL := osutils
.PHONY: default

SRC_CFILES  := $(wildcard *.c)
OBJ_CFILES  := $(addprefix ../$(LIB_OBJ_DIR)/, $(patsubst %.c, %.o, $(SRC_CFILES))) # $(SRC_CFILES:%.c=%.o)

# %.o: %.c
# 	$(CC) $(CFLAGS) -c -o ../$(OS_BIN_DIR)/$@ $^

osutils: $(OBJ_CFILES)
	ar rc ../$(LIB_DIR)/libosutils.a $^

../$(LIB_OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

