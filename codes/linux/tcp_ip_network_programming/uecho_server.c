#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
  if (2 != argc) {
    fprintf(stderr, "Usage: %s <port>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // 1. 创建套接字
  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (-1 == sockfd) {
    perror("socket() error");
    exit(EXIT_FAILURE);
  }

  // 2. 使用 bind 分配 IP 地址，减轻 sendto 的功能
  struct sockaddr_in saddr;
  memset(&saddr, 0, sizeof(saddr));
  saddr.sin_family = AF_INET;
  saddr.sin_addr.s_addr = htonl(INADDR_ANY);
  saddr.sin_port = htons(atoi(argv[1]));
  if (-1 == bind(sockfd, (struct sockaddr *)&saddr, sizeof(saddr))) {
    perror("bind() error");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in caddr;
  memset(&caddr, 0, sizeof(caddr));
  socklen_t addr_len = sizeof(caddr);
  while (1) {
    char message[1024] = {0};
    int ret = recvfrom(sockfd, message, 1024, 0, (struct sockaddr *)&caddr, &addr_len);
    if (-1 == ret) {
      perror("recvfrom() error");
      close(sockfd);
      exit(EXIT_FAILURE);
    }

    sendto(sockfd, message, 1024, 0, (struct sockaddr *)&caddr, addr_len);
  }

  close(sockfd);

  return 0;
}
