#include <sys/mman.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  printf("Practice problem 9.5\n");
  void *ptr;
  int fd;
  struct stat st;
  if (argc != 2) {
    printf("Usage: %s FILENAME\n", argv[0]);
    return 1;
  }
  if (-1 == (fd = open(argv[1], O_RDONLY))) {
    printf("open error: %s: %s\n", argv[1], strerror(errno));
    return 1;
  }
  if (-1 == fstat(fd, &st)) {
    printf("fstat error: %s", strerror(errno));
    return 1;
  };
  ptr = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (ptr == MAP_FAILED) {
    printf("mmap error: %s", strerror(errno));
    return 1;
  }
  if (st.st_size != write(0, ptr, st.st_size)) {
    printf("write error: %s", strerror(errno));
    return 1;
  };
}
