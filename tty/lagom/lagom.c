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
  if (c == '\n')
    buff[i++] = '\n';
  buff[i] = '\0'
  return i;
}

int main() {
  int n;
  while (n = read_line(scroll_buffer[0]))
    printf("%s\n", scroll_buffer[0]);
}
