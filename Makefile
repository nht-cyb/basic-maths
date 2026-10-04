CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

maths: src/main.c src/maths.c include/maths.h
	$(CC) $(CFLAGS) -o $@ src/main.c src/maths.c

clean:
	rm -f maths

.PHONY: clean
