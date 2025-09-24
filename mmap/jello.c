#include <sys/mman.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

// CS:APP -- Exercise 9.14
int main() {
  char *ptr;
  int fd;
  fd = open("hello.txt", O_RDWR);
  if ((ptr = mmap(NULL, 1, PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
    printf("Error: %d %s\n", errno, strerror(errno));
    return errno;
  };
  close(fd);
  *ptr = 'J';
}
