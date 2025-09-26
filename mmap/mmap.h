#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define errexit(func) printf("ERROR: " func ": %s\n", strerror(errno)), exit(1)
