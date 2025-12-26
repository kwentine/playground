#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

#define SIZE 8

typedef struct {
  int fd;
  char buf[SIZE];
  size_t len;
  size_t next;
} rlbuf_t;

rlbuf_t *rlbuf_init(int fd, rlbuf_t *rlbuf) {
  if (rlbuf == NULL) {
    errno = EINVAL;
    return NULL;
  }
  rlbuf->fd = fd;
  rlbuf->len = 0;
  rlbuf->next = 0;
}

int read_line_buf(rlbuf_t *rlbuf, char *buff, size_t n) {

  ssize_t n_read;
  size_t tot_read;
  char c;

  if (buff == NULL || n < 1) {
    errno = EINVAL;
    return -1;
  }

  tot_read = 0;
  for ( ; ; ) {

    if (rlbuf->next == rlbuf->len) {
      printf("Refill needed!\n");
      n_read = read(rlbuf->fd, &rlbuf->buf, SIZE);
      if (n_read == -1)
        return -1;
      if (n_read == 0) {
        if (tot_read == 0)
          return 0;
        break;
      }
      rlbuf->len = n_read;
      rlbuf->next = 0;
    } else {
      c = rlbuf->buf[rlbuf->next++];
      if (tot_read < n - 1) {
        *buff++ = c;
        tot_read++;
      }
      if (c == '\n')
        break;
    }
  }

  *buff = '0';
  return tot_read;
}

// Read line, unbuffered.
ssize_t read_line (int fd, char *buff, size_t n) {

  ssize_t n_read;
  size_t tot_read;
  char c;

  if (buff == NULL || n < 1) {
    errno = EINVAL;
    return -1;
  }

  tot_read = 0;
  for (;;) {
    n_read = read(fd, &c, 1);

    if (n_read == -1)
      return -1;

    if (n_read == 0) {
      if (tot_read == 0)
        return 0;
      break;
    }

    if (tot_read < n - 1) {
      tot_read++;
      *buff++ = c;
    }

    if (c == '\n')
      break;
  }

  *buff = '0';
  return tot_read;
}
