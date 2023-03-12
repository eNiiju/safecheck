CC=gcc
CFLAGS=-Wall -Wextra -Wno-unused-parameter
LDFLAGS=-lpthread -ludev

all: checkpoint

clean:
	rm -f *.o checkpoint

checkpoint: checkpoint.c checkpoint.h configuration.o display.o kineis.o log.o rfid.o usb_key.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

configuration.o: configuration.c configuration.h
	$(CC) $(CFLAGS) -c $< -o $@

display.o: display.c display.h
	$(CC) $(CFLAGS) -c $< -o $@

kineis.o: kineis.c kineis.h
	$(CC) $(CFLAGS) -c $< -o $@

log.o: log.c log.h
	$(CC) $(CFLAGS) -c $< -o $@

rfid.o: rfid.c rfid.h
	$(CC) $(CFLAGS) -c $< -o $@

usb_key.o: usb_key.c usb_key.h
	$(CC) $(CFLAGS) -c $< -o $@
