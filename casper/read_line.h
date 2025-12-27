#define _GNU_SOURCE
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#define RLBUF_SIZE 8

typedef struct {
  int fd;
  char buf[RLBUF_SIZE];
  ssize_t len;
  int next;
} rlbuf_t;

rlbuf_t *rlbuf_init(int, rlbuf_t *);
int read_line_buf(rlbuf_t *, char *, size_t);
ssize_t read_line (int, char *, size_t);
ssize_t read_line_stream(FILE *, char *, size_t, int);
