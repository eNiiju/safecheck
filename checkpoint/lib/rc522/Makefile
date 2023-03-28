CC=gcc
CFLAGS=-Wall -Wextra

all: rc522.o rc522_config.o rc522_main.o rc522_rfid.o rc522_value.o

clean:
	rm -f *.o

# rc522.o
rc522.o: rc522.c rc522.h
	$(CC) $(CFLAGS) -c $< -o $@

# rc522_config.o
rc522_config.o: rc522_config.c rc522_config.h
	$(CC) $(CFLAGS) -c $< -o $@

# rc522_main.o
rc522_main.o: rc522_main.c rc522_main.h
	$(CC) $(CFLAGS) -c $< -o $@

# rc522_rfid.o
rc522_rfid.o: rc522_rfid.c rc522_rfid.h
	$(CC) $(CFLAGS) -c $< -o $@

# rc522_value.o
rc522_value.o: rc522_value.c rc522_value.h
	$(CC) $(CFLAGS) -c $< -o $@
