/*
  Put terminal into raw mode and log keystrokes.
  Exit when 'q' is pressed.
*/

#include <ctype.h>
#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <errno.h>

struct termios savedTermios;

void errExit(char *s) {
  perror(s);
  exit(1);
}

void restore() {
  if (tcsetattr(0, TCSAFLUSH, &savedTermios) == -1)
    errExit("tcsetattr");
  printf("Restored terminal state. Bidding goodbye.\n");
}

int main() {
  struct termios raw;
  int c;

  printf("Logging keystrokes. To quit, press 'q'.\n\n");
  printf("%5s %5s %10s %5s\n\r", "Hex", "Oct", "Bin", "Dec");

  if (tcgetattr(0, &savedTermios) == -1)
    errExit("tcgetattr");

  if (atexit(restore) != 0)
    errExit("atexit");

  raw = savedTermios;
  raw.c_lflag &= ~(ICANON | ISIG | IEXTEN | ECHO);
  raw.c_iflag &= ~(BRKINT | ICRNL | IGNBRK | IGNCR | INLCR |
                   INPCK | ISTRIP | IXON | PARMRK);
  raw.c_oflag &= ~OPOST;

  // Block on read and return as soon as one character is available
  raw.c_cc[VMIN] = 1;
  raw.c_cc[VTIME] = 0;

  if (tcsetattr(0, TCSAFLUSH, &raw) == -1)
    errExit("tcsetattr");

  while ((c = getchar()) != EOF) {
    printf("%5x %5o %10.08b %5d", c, c, c, c);
    if (!isascii(c)) {
      printf("  ??\r\n");
    } else if (iscntrl(c)) {
      printf("  ^%c\r\n",  c ^ 0x40);
    } else {
      printf("  %2c\r\n", c);
    }
    if (c == 'q')
      break;
  }
  return 0;
}
