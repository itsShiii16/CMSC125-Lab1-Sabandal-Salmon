CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude  # Add -Iinclude to specify the include directory
SRC = src/main.c src/parser.c
TARGET = mysh

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)
