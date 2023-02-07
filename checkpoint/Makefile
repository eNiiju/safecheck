CC=gcc
CFLAGS=-Wall -Wextra -Wno-unused-parameter

all: checkpoint

clean:
	rm -f checkpoint

checkpoint: checkpoint.c checkpoint.h
	$(CC) $(CFLAGS) $^ -o $@
