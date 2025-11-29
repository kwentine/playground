#include <stdio.h>
#include <unistd.h>

#define MAXLINE 1024
#define SCROLL_ROWS 32

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


int main() {
  int n;
  reset(10);
  while (n = read_line(scroll_buffer[0]))
    printf("%s", scroll_buffer[0]);
}
