#include "casperd.h"
#include "read_line.h"

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

void handle_client(int);
void serve(int);

int main() {
  int listenfd;
  struct sockaddr_in serv_addr;
  char ip_str[INET_ADDRSTRLEN];

  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  if (listenfd == -1)
    err_exit("socket");

  bzero(&serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
  serv_addr.sin_port = htons(PORT_NUM);

  if (bind(listenfd, (SA *) &serv_addr, sizeof(serv_addr)) == -1)
    err_exit("bind");

  if (listen(listenfd, LISTENQ) == -1)
    err_exit("listen");

  printf("Casperd, friendly little daemon, listens on: %s:%d\n",
         inet_ntop(AF_INET, &serv_addr.sin_addr, ip_str, INET_ADDRSTRLEN),
         PORT_NUM);
  serve(listenfd);
}

void serve(int fd) {
  int connfd;
  struct sockaddr_in client_addr;
  socklen_t size = sizeof(client_addr);
  char client_addr_str[INET_ADDRSTRLEN];

  for ( ; ; ) {
    connfd = accept(fd, (SA *) &client_addr, &size);
    if (connfd == -1)
      err_exit("accept");

    printf("Connection from: %s:%d\n",
           inet_ntop(AF_INET,
                     &client_addr.sin_addr,
                     client_addr_str,
                     INET_ADDRSTRLEN),
           client_addr.sin_port);

    handle_client(connfd);

    if (close(connfd) == -1)
      err_exit("close");
    printf("Connection closed.\n");
  }
}

void handle_client(int fd) {
  char buff[MAX_LINE];
  int i;
  ssize_t n_read;
  rlbuf_t rlbuf;

  if (rlbuf_init(fd, &rlbuf) == NULL)
    err_exit("rlbuf_init");

  while ( (n_read = read_line_buf(&rlbuf, buff, MAX_LINE)) > 0) {
    for (i = 0; i < n_read; i++) {
      if (isupper(buff[i]))
        buff[i] = buff[i] - ('A' - 'a');
    }
    if (write(fd, buff, n_read) == -1)
      err_exit("write");
  }
  if (n_read  == -1)
    err_exit("read_line");
}
