CC=gcc
CFLAGS=-Wall -Wextra -Werror -Wconversion -g -O2 -pedantic -std=c99
TARGET=lilystatic
SRC=lilystatic.c

.PHONY: all clean

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)
