SHELL=/bin/bash
CC=gcc

.SILENT: test clean

all: jello boundaries

test: jello
	$(SHELL) test.sh

clean: hello.txt
	rm -f jello

page.txt:
	dd if=<(yes 1 | tr -d '\n') of=$@ bs=512 count=2
