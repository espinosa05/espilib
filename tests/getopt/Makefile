CC=gcc
CFLAGS=-Wall -std=c99 -ggdb
SRC_DIR=../../
HEADER=getopt.h
CLI=cli/

TEST_FILE=test.c
OUT=out/

.PHONY: all clean

all: $(OUT)test

clean: $(OUT) $(CLI)$(HEADER)
	rm -rf $(OUT)
	rm -rf $(CLI)

$(OUT)test: $(TEST_FILE) $(OUT) $(CLI)$(HEADER)
	$(CC) $(TEST_FILE) $(CFLAGS) -o $@

$(CLI):
	mkdir $@

$(OUT):
	mkdir $@

$(CLI)$(HEADER): $(SRC_DIR)$(CLI)getopt.h $(CLI)
	cp $(SRC_DIR)$(CLI)getopt.h $(CLI)

