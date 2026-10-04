CC = gcc
CFLAGS = -Wall -Wextra -Iinclude

SRCS = $(wildcard src/*.c)
HDRS = $(wildcard include/*.h)

maths: $(SRCS) $(HDRS)
	$(CC) $(CFLAGS) -o $@ $(SRCS)

clean:
	rm -f maths

.PHONY: clean
