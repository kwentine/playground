#include <stdio.h>
#include <unistd.h>

#define MAXLINE 1024
#define SCROLL_ROWS 32

char scroll_buffer[SCROLL_ROWS][MAXLINE];

int read_line(int fd, char buff[MAXLINE]) {
  int i;
  char c;
  while ((c = getchar()) != EOF && c != '\n' && i < MAXLINE - 1)
    buff[i++] = c;
  buff[i++] = '\n';
  return i;
}

int main() {
  int n;
  n = read_line(0, scroll_buffer[0]);
  write(1, scroll_buffer[0], n);
}
