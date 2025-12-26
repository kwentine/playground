CC ?= cc
# Warnings on + debug-friendly defaults for teaching.
CFLAGS ?= -Wall -Wextra -g -O0 -fno-omit-frame-pointer

.PHONY: all build clean

all: casperd

build: casperd

casperd: casperd.c read_line.c
	$(CC) $(CFLAGS) -o $@ $^

egg: egg.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o casperd egg
