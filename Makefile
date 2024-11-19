CC=gcc
CFLAGS=-Wall -std=c99 -ggdb

TEST_FILE=test.c

.PHONY: all clean

all: test
clean:
	rm test

test: $(TEST_FILE)
	$(CC) $(CFLAGS) $^ -o $@
