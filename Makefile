CC=gcc
CFLAGS=-Wall -Wextra -std=c99

TARGETS=main

build: $(TARGETS)

star: main.c
	$(CC) $(CFLAGS) main.c -o main

clean:
	rm -f ./out
