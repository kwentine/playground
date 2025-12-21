#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

void err_exit(char *msg) {
  perror(msg);
  exit(1);
}

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
  bind(sockfd, (struct sockaddr *) &saddr, sizeof(saddr));
  listen(sockfd, 5);

  struct sockaddr_in client;
  int clientfd;
  socklen_t size = sizeof(client);
  for (;;) {
    clientfd = accept(sockfd, (struct sockaddr *) &client, &size);
    printf("Connection from: %d\n", client.sin_port);
    close(clientfd);
  }

}
