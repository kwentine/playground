#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

#define SA struct sockaddr
#define LISTENQ 5
#define PORT_NUM 8000
#define MAX_LINE 8

void err_exit(const char *msg) {
  fprintf(stderr, "ERROR:%s:%s: %s\n",
          msg,
          strerrorname_np(errno),
          strerror(errno));
  exit(1);
}

ssize_t read_line (FILE *, char *, size_t);

int main() {
  int listenfd, connfd;
  struct sockaddr_in servaddr, clientaddr;
  char ip_str[INET_ADDRSTRLEN];

  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  if (listenfd == -1)
    err_exit("socket");

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
  servaddr.sin_port = htons(PORT_NUM);

  if (bind(listenfd, (SA *) &servaddr, sizeof(servaddr)) == -1)
    err_exit("bind");

  if (listen(listenfd, LISTENQ) == -1)
    err_exit("listen");

  printf("Casperd, friendly little daemon, listens on: %s:%d\n",
         inet_ntop(AF_INET, &servaddr.sin_addr, ip_str, INET_ADDRSTRLEN),
         PORT_NUM);

  socklen_t size = sizeof(clientaddr);
  char buff[MAX_LINE] = {0};
  ssize_t n_read = 0;
  // This will be read from configuration
  int to_lower = 1;
  size_t i = 0;
  FILE *stream;

  for ( ; ; ) {
    connfd = accept(listenfd, (SA *) &clientaddr, &size);
    if (connfd == -1)
      err_exit("accept");

    if ((stream = fdopen(connfd, "r")) == NULL)
      err_exit("fdopen");

    printf("Connection from: %d\n", clientaddr.sin_port);
    while ( (n_read = read_line(stream, buff, MAX_LINE)) > 0) {
      for (i = 0; i < n_read; i++) {
        if (isupper(buff[i]))
          buff[i] = buff[i] - ('A' - 'a');
      }
      if (write(connfd, buff, n_read) == -1)
        err_exit("write");
    }

    if (n_read  == -1)
      err_exit("read_line");

    if (close(connfd) == -1)
      err_exit("close");

    printf("Connection closed.\n");
  }
}

ssize_t read_line(FILE *stream, char *buff, size_t lim) {
  int c, i = 0;
  while (i < lim - 1 && (c = getc(stream)) != EOF && c != '\n')
    buff[i++] = c;
  if (c == '\n')
    buff[i++] = '\n';
  buff[i] = '\0';
  return i;
}

ssize_t read_line_discard(FILE *stream, char *buff, size_t lim) {
  int c, i = 0;
  while (i < lim - 1 && (c = getc(stream)) != EOF && c != '\n')
    buff[i++] = c;
  if (i == lim - 1)
    while ((c = getc(stream)) != EOF && c != '\n')
      ;
  else if (c == '\n')
    buff[i++] = '\n';
  buff[i] = '\0';
  return i;
}
