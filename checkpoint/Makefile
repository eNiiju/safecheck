CC=gcc
CFLAGS=-Wall -Wextra -Wno-unused-parameter
LDFLAGS=-lpthread

all: checkpoint

clean:
	rm -f checkpoint

checkpoint: checkpoint.c checkpoint.h
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
