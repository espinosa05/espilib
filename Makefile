CC=gcc
CFLAGS=-Wall -std=c99 -ggdb

TEST_FILE=test.c
OUT=out/

.PHONY: all clean

all: $(OUT)test

clean: $(OUT)
	rm -rf $(OUT)

$(OUT)test: $(TEST_FILE) $(OUT)
	$(CC) $(TEST_FILE) $(CFLAGS) -o $@

$(OUT):
	mkdir $@
