# --------------------------------------------------------------------------- #
#                              Global variables                               #
# --------------------------------------------------------------------------- #

CC=gcc
CFLAGS=-Wall -Wextra -Wno-unused-parameter

# Checkpoint main executable
LDFLAGS=-lpthread -lbcm2835 -ludev
CHECKPOINTOBJ=src/configuration.o src/display.o src/kineis.o src/log.o src/rfid.o src/usb_key.o

# RFID
LIBRC522OBJ=lib/rc522/rc522.o lib/rc522/rc522_config.o lib/rc522/rc522_main.o lib/rc522/rc522_rfid.o lib/rc522/rc522_value.o

# --------------------------------------------------------------------------- #
#                                Main targets                                 #
# --------------------------------------------------------------------------- #

all: src/checkpoint

clean:
	rm -f src/*.o src/checkpoint test/test_display
	cd lib/rc522 && make clean

src/checkpoint: src/checkpoint.c src/checkpoint.h $(CHECKPOINTOBJ) $(LIBRC522OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS) -L./lib/libssd1306/lib -l:libssd1306_i2c.so.0.0.3

src/configuration.o: src/configuration.c src/configuration.h
	$(CC) $(CFLAGS) -c $< -o $@

src/display.o: src/display.c src/display.h
	$(CC) $(CFLAGS) -c $< -o $@ -L./lib/libssd1306/lib -l:libssd1306_i2c.so.0.0.3

src/kineis.o: src/kineis.c src/kineis.h
	$(CC) $(CFLAGS) -c $< -o $@

src/log.o: src/log.c src/log.h
	$(CC) $(CFLAGS) -c $< -o $@

src/rfid.o: src/rfid.c src/rfid.h lib/rc522
	$(CC) $(CFLAGS) -c $< -o $@ -lbcm2835

src/usb_key.o: src/usb_key.c src/usb_key.h
	$(CC) $(CFLAGS) -c $< -o $@ -ludev

# --------------------------------------------------------------------------- #
#                                    Tests                                    #
# --------------------------------------------------------------------------- #

test/test_display: test/test_display.c src/display.o
	$(CC) $(CFLAGS) -c $^ -o $@ -L./lib/libssd1306/lib -l:libssd1306_i2c.so.0.0.3

# --------------------------------------------------------------------------- #
#                                  Libraries                                  #
# --------------------------------------------------------------------------- #

.PHONY: lib/rc522
lib/rc522:
	cd lib/rc522 && make
