#include <stdio.h>
#include <arpa/inet.h>

int main() {
  uint16_t h = 0x4142; // "AB"
  char *ptr = (char *) &h;
  printf("Host: %c %c\n", *ptr, *(ptr + 1));
  uint16_t n = htons(h);
  ptr = (char *) &n;
  printf("Net : %c %c\n", *ptr, *(ptr + 1));
}
