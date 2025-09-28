// Going beyond boundaries
#include "mmap.h"
#include <signal.h>

// $ lsblk -l
#define DISK_SECTOR_SIZE 512
// $ getconf PAGE_SIZE
#define PAGE_SIZE 4096
// File consists of 2048 ASCII 1 characters. See Makefile
#define MAPPED_FILE "halfpage.txt"
#define MAPPED_FILE_SIZE 2048


void handler(int signum) {
  if (signum == SIGBUS) {
    printf("Caught SIGBUS!\n");
  }
  exit(1);
}

int main(void) {
  char *ptr;
  int fd;
  char c;

  fd = open(MAPPED_FILE, O_RDONLY);

  /* Map half the file
     |##......|   Requested mapped size
     |1111....|   File size
     |vvvvvvvv|   Actual mapped size (rounded up)
     |11110000|   Seen
   */
  ptr = mmap(NULL, MAPPED_FILE_SIZE / 2, PROT_READ, MAP_PRIVATE, fd, 0);
  c = *ptr;
  printf("|##......|\n|1111....|\n|v v v   |\n");
  printf("|%c.......|\n", c);
  c = *(ptr + MAPPED_FILE_SIZE / 2);
  printf("|..%c.....|\n", c);
  c = *(ptr + MAPPED_FILE_SIZE);
  printf("|....%1x...|\n", c);

  /* Map two pages
     |########|########|   Requested map size
     |1111....|........|   File size
     |vvvvvvvv|vvvvvvvv|
     |11110000|BBBBBBBB|   Seen; B=SIGBUS
   */
  ptr = mmap(NULL, 2 * PAGE_SIZE, PROT_READ, MAP_PRIVATE, fd, 0);
  signal(SIGBUS, &handler);
  c = *(ptr + PAGE_SIZE);
  return 1;
}
