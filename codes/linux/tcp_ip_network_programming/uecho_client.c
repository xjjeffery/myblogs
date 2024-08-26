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

  struct sockaddr_in caddr;
  memset(&caddr, 0, sizeof(caddr));
  caddr.sin_family = AF_INET;
  caddr.sin_addr.s_addr = inet_addr(argv[1]);
  caddr.sin_port = htons(atoi(argv[2]));
  while (1) {
    printf("Input message(q/Q to quit): ");
    char message[1024] = {0};
    fgets(message, 1024, stdin);
    if (!strcmp(message, "Q\n") || !strcmp(message, "q\n"))
      break;

    sendto(sockfd, message, 1024, 0, (struct sockaddr *)&caddr, sizeof(caddr));

    socklen_t addr_len = sizeof(caddr);
    recvfrom(sockfd, message, 1024, 0, (struct sockaddr *)&caddr, &addr_len);
    printf("message from server: %s\n", message);
  }

  close(sockfd);

  return 0;
}
