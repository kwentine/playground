// Going beyond boundaries
#include "mmap.h"

// $ lsblk -l
#define DISK_SECTOR_SIZE 512
// $ getconf PAGE_SIZE
#define PAGE_SIZE 4069
// File consists of 1024 ASCII 1 characters. See Makefile
#define MAPPED_FILE "page.txt"
#define MAPPED_FILE_SIZE 1024
#define MAPPED_LENGTH 512

int main(void) {
  char *ptr;
  int fd;
  char nl = 0xa;

  fd = open(MAPPED_FILE, O_RDONLY);
  ptr = mmap(NULL, MAPPED_LENGTH, PROT_READ, MAP_PRIVATE, fd, 0);
  // within requested mapped range: outputs 1
  write(STDOUT_FILENO, ptr, 8);
  // outside of requested range, but whithin file range: still outputs 1
  write(STDOUT_FILENO, ptr + MAPPED_LENGTH, 8);
  // outside of file range, but whithin a page size: outputs 0x0
  write(STDOUT_FILENO, ptr + MAPPED_FILE_SIZE, 8);
  return 0;
}
