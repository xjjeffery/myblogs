#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "proto.h"

int main(int argc, char *argv[]) {  
  if (argc < 2) {
    fprintf(stderr, "Usage...\n");
    exit(EXIT_FAILURE);
  }

  // 创建套接字
  int cfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (cfd < 0) {
    perror("socket()");
    exit(EXIT_FAILURE);
  }

  struct msg_st sbuf;
  memset(&sbuf, '\0', sizeof(sbuf));
  strcpy(sbuf.name, "Alan");
  sbuf.math = htonl(rand() % 100);
  sbuf.chinese = htonl(rand() % 100);

  // 对端地址的配置
  struct sockaddr_in raddr;
  raddr.sin_family = AF_INET;
  raddr.sin_port = htons(atoi(RCVPORT));
  inet_pton(AF_INET, argv[1], &raddr.sin_addr);
  if (sendto(cfd, &sbuf, sizeof(sbuf), 0, (void *)&raddr, sizeof(raddr)) < 0) {
    perror("sendto()");
    exit(EXIT_FAILURE);
  }
  puts("OK!");
  close(cfd);
  exit(0);
}