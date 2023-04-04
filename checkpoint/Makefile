CC=gcc
CFLAGS=-Wall -Wextra -Wno-unused-parameter
CHECKPOINTOBJ=src/configuration.o src/display.o src/kineis.o src/log.o src/rfid.o src/usb_key.o
LIBRC522OBJ=lib/rc522/rc522.o lib/rc522/rc522_config.o lib/rc522/rc522_main.o lib/rc522/rc522_rfid.o lib/rc522/rc522_value.o
LDFLAGS=-lpthread -lbcm2835 -ludev

all: src/checkpoint

clean:
	rm -f src/*.o src/checkpoint
	cd lib/rc522 && make clean

# Main program
src/checkpoint: src/checkpoint.c src/checkpoint.h $(CHECKPOINTOBJ) $(LIBRC522OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

src/configuration.o: src/configuration.c src/configuration.h
	$(CC) $(CFLAGS) -c $< -o $@

src/display.o: src/display.c src/display.h
	$(CC) $(CFLAGS) -c $< -o $@
#export LD_LIBRARY_PATH=/home/matheo/safecheck/checkpoint/lib/libssd1306/lib
#gcc -c ./src/display.c -o ./src/display.o -l:libssd1306_i2c.so.0.0.3 -L/home/matheo/safecheck/checkpoint/lib/libssd1306/libc
#gcc ./src/display.o ./test/test_display.c -o ./test/test_display -l:libssd1306_i2c.so.0.0.3 -L/home/matheo/safecheck/checkpoint/lib/libssd1306/lib

src/kineis.o: src/kineis.c src/kineis.h
	$(CC) $(CFLAGS) -c $< -o $@

src/log.o: src/log.c src/log.h
	$(CC) $(CFLAGS) -c $< -o $@

src/rfid.o: src/rfid.c src/rfid.h lib/rc522
	$(CC) $(CFLAGS) -c $< -o $@ -lbcm2835

src/usb_key.o: src/usb_key.c src/usb_key.h
	$(CC) $(CFLAGS) -c $< -o $@ -ludev

# Libraries
.PHONY: lib/rc522
lib/rc522:
	cd lib/rc522 && make
