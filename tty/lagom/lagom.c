#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#define MAXLINE 1024
#define SCROLL_ROWS 32
#define DISPLAY_ROWS 16
#define MARGIN_TOP 10
#define next(idx) (idx + 1) % SCROLL_ROWS
#define add(idx, incr) (idx + incr) % SCROLL_ROWS

struct termios saved_termios;
static char scroll_buffer[SCROLL_ROWS][MAXLINE];
static int read_idx = 0;
static int write_idx = 0;

// Read the next input line, truncated to MAXLINE
int read_line(char buff[MAXLINE]) {
  int i = 0, c;
  while ((c = getchar()) != EOF && c != '\n' && i < MAXLINE - 2)
      buff[i++] = c;
  if (i > 0 || c == '\n')
    buff[i++] = '\n';
  buff[i] = '\0';
  return i;
}

// Clear screen and move cursor to given row
void reset(int row) {
  printf("\x1b[2J\x1b[%dd", row);
}

void draw(int row) {
  int i = 0, j = read_idx;
  reset(row);
  for (i = 0; i < DISPLAY_ROWS; i++, j = next(j)) {
    if (*scroll_buffer[j] == '\0')
      printf("~\n");
    else
      printf(" %s", scroll_buffer[j]);
  }
}

void init() {
  int i;
  for (i = 0; i < SCROLL_ROWS; i++)
    read_line(scroll_buffer[i]);
  read_idx = write_idx = 0;
}

void refill() {
  while (write_idx != read_idx) {
    read_line(scroll_buffer[write_idx]);
    write_idx = next(write_idx);
  }
}

void err_exit(char *s) {
  perror(s);
  exit(1);
}


int open_tty() {
  int fd;
  struct termios t;

  if ((fd = open("/dev/tty", O_RDWR)) == -1)
    err_exit("open");

  if (tcgetattr(fd, &saved_termios) == -1)
    err_exit("tcgetattr");

  t = saved_termios;
  t.c_lflag &= ~(ICANON | ECHO);
  t.c_lflag |= ISIG;

  t.c_iflag &= ~ICRNL;

  t.c_cc[VMIN] = 1;
  t.c_cc[VTIME] = 0;

  if (tcsetattr(fd, TCSAFLUSH, &t) == -1)
    err_exit("tcsetattr");

  return fd;
}

void play() {
  while (*scroll_buffer[read_idx] != '\0'){
    draw(MARGIN_TOP);
    sleep(1);
    if (add(read_idx, DISPLAY_ROWS) == write_idx)
      refill();
    read_idx = next(read_idx);
  }
}

void interactive() {
  int c, fd;
  ssize_t n;
  fd = open_tty();
  while(true) {
    draw(MARGIN_TOP);

    if ((n = read(fd, &c, 1)) == -1)
      err_exit("read");

    if (c == 'q' || n == 0 || *scroll_buffer[read_idx] == '\0')
      break;

    if (c == 'n')
      read_idx = add(read_idx, 1);
    else if (c == 'f')
      read_idx = add(read_idx, DISPLAY_ROWS - 1);
    refill();
  }
  if (tcsetattr(fd, TCSAFLUSH, &saved_termios) == -1)
    err_exit("tcsetattr");
  reset(0);
}

int main() {
  int i, c;
  init();
  interactive();
}
