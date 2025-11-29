#include <stdio.h>
#include <unistd.h>

#define MAXLINE 1024
#define SCROLL_ROWS 32
#define DISPLAY_ROWS 16
#define MARGIN_TOP 10
#define next(idx) (idx + 1) % SCROLL_ROWS

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
  for (i = 0; i < DISPLAY_ROWS; i++, j = (j + 1) % SCROLL_ROWS) {
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
    write_idx = (write_idx + 1) % SCROLL_ROWS;
  }
}

int main() {
  int i, c;
  init();
  while (*scroll_buffer[read_idx] != '\0'){
    draw(MARGIN_TOP);
    sleep(1);
    if ((read_idx + DISPLAY_ROWS) % SCROLL_ROWS == write_idx)
      refill();
    read_idx = (read_idx + 1) % SCROLL_ROWS;
  }
  while (true)
    sleep(1);
}
