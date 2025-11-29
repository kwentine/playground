#include <stdio.h>
#include <unistd.h>

#define MAXLINE 1024
#define SCROLL_ROWS 32

char scroll_buffer[SCROLL_ROWS][MAXLINE];

// Read the next input line, truncated to MAXLINE
int read_line(int fd, char buff[MAXLINE]) {
  int i = 0;
  char c;
  while ((c = getchar()) != EOF && c != '\n') {
    if (i < MAXLINE - 1)
      buff[i++] = c;
  }
  if (c == EOF)
    return 0;
  buff[i++] = '\n';
  return i;
}

int main() {
  int n;
  while (n = read_line(0, scroll_buffer[0]))
    write(1, scroll_buffer[0], n);
}
