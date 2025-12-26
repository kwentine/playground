#define _GNU_SOURCE
#include <arpa/inet.h>
#include <ctype.h>
#include <errno.h>
#include <netinet/in.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define SA struct sockaddr
#define RLBUF_SIZE 8

typedef struct {
  int fd;
  char buf[RLBUF_SIZE];
  size_t len;
  size_t next;
} rlbuf_t;

rlbuf_t *rlbuf_init(int, rlbuf_t *);
int read_line_buf(rlbuf_t *, char *, size_t);
ssize_t read_line (int, char *, size_t);
ssize_t read_line_stream(FILE *, char *, size_t, int);
