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
  struct termios saved, raw;
  int c;

  printf("To quit, press 'q'.\n");
  printf("%5s %10s %5s\n\r", "Hex", "Bin", "Dec");

  if (tcgetattr(STDIN_FILENO, &saved) == -1)
    errExit("tcgetattr");

  raw = saved;
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
    printf("%5x %10.08b %5d", c, c, c);
    if (c > 127) {
      printf("  ??\r\n");
    } else if (c < 0x20 || c == 0x7f) {
      printf("  ^%c\r\n", (c + 0x40) % 0x80);
    } else {
      printf("  %2c\r\n", c);
    }
    if (c == 'q')
      break;
  }

  /* Restore terminal state */
  if (tcsetattr(0, TCSAFLUSH, &saved) == -1)
    errExit("tcsetattr");

  exit(0);
}
