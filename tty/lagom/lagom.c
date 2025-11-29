#include <stdio.h>
#include <unistd.h>

#define MAXLINE 1024
#define SCROLL_ROWS 32
#define DISPLAY_ROWS 16

char scroll_buffer[SCROLL_ROWS][MAXLINE];

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

void enter_cbreak() {}

void draw(int row, int last_line, char sbuff[][MAXLINE]) {
  int i = 0, j = 0;
  reset(row);
  // Find where to start from
  j = (last_line - DISPLAY_ROWS + SCROLL_ROWS) % SCROLL_ROWS;
  if (*sbuff[j] == '\0')
    j = 0;
  for (i = 0; i < DISPLAY_ROWS; i++) {
    if (*sbuff[j] == '\0') {
      printf("~\n");
    } else {
      printf(" %s", sbuff[j]);
    }
    j = (j + 1) % SCROLL_ROWS;
  }
}

int main() {
  int i, start_line, c;
  // Initialize scroll buffer
  for (i = 0; i < SCROLL_ROWS; i++) {
    scroll_buffer[i][0] = '\0';
  }
  i = 0;
  while (read_line(scroll_buffer[i])){
    draw(10, i, scroll_buffer);
    sleep(1);
    i = (i + 1) % SCROLL_ROWS;
  }
  // Wait for sigint
  while (true)
    sleep(1);
}
