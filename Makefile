CC=gcc
CFLAGS=-Wall -std=c99 -a

AR=ar
ARFLAGS=-r

SYSTEM_LIB_DIR=/usr/lib/
SYSTEM_INCLUDE_DIR=/usr/include/

OUT_DIR=out/
OBJ_DIR=out/obj
BIN_DIR=out/bin

SRC_DIR=src/

API_HEADERS=$(SRC_DIR)tagged_buffer.h		\
	    $(SRC_DIR)header_only/types.h	\
	    $(SRC_DIR)header_only/getopt.h

SRC=

all: libespi

clean: $(OUT_DIR)
	rm -rf $@

libespi: $(SYSTEM_LIB_DIR)libespi.a

$(SYSTEM_LIB_DIR)libespi.a: $(BIN_DIR)libespi.a
	cp $^ $@

$(BIN_DIR)libespi.a: $(LIB_OBJ)
	$(AR) $(ARFLAGS) $^ -o $@

$(LIB_OBJ): $(LIB_SRC)
	$(CC) $(CFLAGS) $< -o $@

$(OUT_DIR):
	mkdir $@

$(OBJ_DIR): $(OUT_DIR)
	mkdir $@

$(BIN_DIR): $(OUT_DIR)
	mkdir $@
