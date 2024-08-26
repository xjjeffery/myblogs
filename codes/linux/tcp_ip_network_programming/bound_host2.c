#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
  if (3 != argc) {
    fprintf(stderr, "Usage: %s <ip> <port>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // 1. 创建套接字
  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (-1 == sockfd) {
    perror("socket() error");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in saddr;
  memset(&saddr, 0, sizeof(saddr));
  saddr.sin_family = AF_INET;
  saddr.sin_addr.s_addr = inet_addr(argv[1]);
  saddr.sin_port = htons(atoi(argv[2]));
  socklen_t addr_len = sizeof(saddr);
  if (-1 == connect(sockfd, (struct sockaddr *)&saddr, addr_len)) {
    perror("connect() error");
    close(sockfd);
    exit(EXIT_FAILURE);
  } else {
    printf("Connected ......\n");
  }

  char msg1[] = "Hi!";
  char msg2[] = "I'm another UDP host";
  char msg3[] = "Nice to meet you";
  sendto(sockfd, msg1, sizeof(msg1), 0, (struct sockaddr *)&saddr, sizeof(saddr));
  sendto(sockfd, msg2, sizeof(msg1), 0, (struct sockaddr *)&saddr, sizeof(saddr));
  sendto(sockfd, msg3, sizeof(msg1), 0, (struct sockaddr *)&saddr, sizeof(saddr));

  close(sockfd);

  return 0;
}
