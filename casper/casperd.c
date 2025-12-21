#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

void err_exit(const char *msg) {
  fprintf(stderr, "ERROR:%s:%s: %s\n",
          msg,
          strerrorname_np(errno),
          strerror(errno));
  exit(1);
}

ssize_t read_line (int fd, char *buff, size_t n);

int main() {
  int sockfd;

  struct in_addr ip;
  struct sockaddr_in saddr;

  // Internet address, from ASCII to network
  inet_aton("127.0.0.1", &ip);

  saddr.sin_family = AF_INET;
  saddr.sin_addr = ip;
  saddr.sin_port = htons(8000);

  printf("Casperd, friendly little daemon, listens on port 8000.\n");

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1)
    err_exit("socket");

  if (bind(sockfd, (struct sockaddr *) &saddr, sizeof(saddr)) == -1)
    err_exit("bind");

  if (listen(sockfd, 5) == -1)
    err_exit("listen");

  struct sockaddr_in client;
  int clientfd;
  socklen_t size = sizeof(client);
  char buff[1024] = {0};
  ssize_t n_read = 0;
  for (;;) {
    clientfd = accept(sockfd, (struct sockaddr *) &client, &size);
    if (clientfd == -1)
      err_exit("accept");

    printf("Connection from: %d\n", client.sin_port);
    while ( (n_read = read_line(clientfd, buff, 1024)) > 0) {
      if (write(clientfd, buff, n_read) == -1)
        err_exit("write");
    }
    if (n_read  == -1)
      err_exit("read_line");

    if (close(clientfd) == -1)
      err_exit("close");

    printf("Connection closed.\n");
  }
}

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
