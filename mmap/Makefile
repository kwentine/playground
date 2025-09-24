SHELL=/bin/bash
CC=gcc

.SILENT: test clean

all: jello

jello:

test:
	$(SHELL) test.sh

clean: hello.txt
	rm -f jello
	echo Hello > hello.txt
