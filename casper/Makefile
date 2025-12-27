CC ?= cc
# Warnings on + debug-friendly defaults for teaching.
CFLAGS ?= -Wall -Wextra -g -O0 -fno-omit-frame-pointer

.PHONY: all build clean

OBJS = casperd.o read_line.o

all: casperd

build: casperd

casperd: $(OBJS)

casperd.o: casperd.h

read_line.o: read_line.h

egg: egg.c

clean:
	rm -f *.o casperd egg
