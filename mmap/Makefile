SHELL=/bin/bash
CC=gcc
CFLAGS=-O0

.SILENT: test clean

all: jello boundaries

test: jello
	$(SHELL) test.sh

clean: hello.txt
	rm -f jello

halfpage.txt:
	dd if=<(yes 1 | tr -d '\n') of=$@ bs=512 count=4
