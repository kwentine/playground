#include <sys/mman.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define errexit(func) printf("ERROR: " func ": %s\n", strerror(errno)), exit(1)

// CS:APP -- Exercise 9.14
void main() {
  char *ptr;
  int fd;

  if ((fd = open("hello.txt", O_RDWR)) == -1) {
    errexit("open");
  }

  if ((ptr = mmap(NULL, 1, PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
    errexit("mmap");
  };

  close(fd);
  *ptr = 'J';
}
