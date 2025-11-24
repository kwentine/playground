/*
  Put terminal into raw mode and log keystrokes.
  Exit when 'q' is pressed.
*/

#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#define errExit(syscall) printf("ERROR: " syscall ": %s\n", strerror(errno)), exit(1)

char *FMT = "0x%-6x %-10.08b %-10d %c\n\r";

void restore(struct termios *saved) {
  if (tcsetattr(0, TCSAFLUSH, saved) == -1)
    errExit("tcsetattr");
}

int main() {
  printf("To quit, enter:\n");
  printf(FMT, 'q', 'q', 'q', 'q');

  struct termios saved, raw;

  if (tcgetattr(STDIN_FILENO, &saved) == -1)
    errExit("tcgetattr");

  raw = saved;
  raw.c_lflag &= ~(ICANON | ISIG | IEXTEN | ECHO);
  raw.c_iflag &= ~(BRKINT | ICRNL | IGNBRK | IGNCR | INLCR |
                   INPCK | ISTRIP | IXON | PARMRK);
  raw.c_oflag &= ~OPOST;
  raw.c_cc[VMIN] = 1;                   /* Character-at-a-time input */
  raw.c_cc[VTIME] = 0;                  /* with blocking */

  if (tcsetattr(0, TCSAFLUSH, &raw) == -1)
    errExit("tcsetattr");

  unsigned char c;
  ssize_t n;
  while (n = read(STDIN_FILENO, &c, 1)) {
    if (n == -1) {
      restore(&saved);
      errExit("read");
    }
    if (c == 'q')
      break;
    printf(FMT, c, c, c, c);
  }
  restore(&saved);
  exit(0);
}
