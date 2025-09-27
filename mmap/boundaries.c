// Going beyond boundaries
#include "mmap.h"

// $ lsblk -l
#define DISK_SECTOR_SIZE 512
// $ getconf PAGE_SIZE
#define PAGE_SIZE 4096
// File consists of 2048 ASCII 1 characters. See Makefile
#define MAPPED_FILE "halfpage.txt"
#define MAPPED_FILE_SIZE 2048


int main(void) {
  char *ptr;
  int fd;
  char tmp;

  fd = open(MAPPED_FILE, O_RDONLY);

  /* Map half the file
     |##......|   Requested mapped size
     |1111....|   File size
     |vvvvvvvv|   Actual mapped size (rounded up)
     |11110000|   Seen
   */
  ptr = mmap(NULL, MAPPED_FILE_SIZE / 2, PROT_READ, MAP_PRIVATE, fd, 0);
  write(STDOUT_FILENO, ptr, 8);
  write(STDOUT_FILENO, ptr + MAPPED_FILE_SIZE / 2, 8);
  write(STDOUT_FILENO, ptr + MAPPED_FILE_SIZE, 8);

  /* Map two pages
     |########|########|   Requested map size
     |1111....|........|   File size
     |vvvvvvvv|vvvvvvvv|
     |11110000|BBBBBBBB|   Seen; B=SIGBUS
   */
  fd = open(MAPPED_FILE, O_RDWR);
  ptr = mmap(NULL, 2 * PAGE_SIZE, PROT_WRITE, MAP_SHARED, fd, 0);
  tmp = *(ptr + PAGE_SIZE);
  return 1;
}
