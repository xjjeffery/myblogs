#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "proto.h"

#define IPSTRSIZE 64

int main() {
  // 创建协议为 ipv4 的报式套接字，0 为默认协议，即 UDP
  int sfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sfd < 0) {
    perror("socket()");
    exit(EXIT_FAILURE);
  }
  
  struct sockaddr_in laddr, raddr;
  laddr.sin_family = AF_INET;
  laddr.sin_port = htons(atoi(RCVPORT));
  laddr.sin_addr.s_addr = htonl(INADDR_ANY);
  // 关联地址和套接字
  if (bind(sfd, (struct sockaddr *)&laddr, sizeof(laddr)) < 0) {
    perror("bind()");
    exit(EXIT_FAILURE);
  }

  // 对端地址长度
  socklen_t raddr_len;
  // 接收到的结构体
  struct msg_st rbuf;
  // 存储点分十进制字符串的数组
  char ipstr[IPSTRSIZE];
  // 这里一定要初始化对端地址的大小！
  raddr_len = sizeof(raddr);
  while (1) {
    if (recvfrom(sfd, &rbuf, sizeof(rbuf), 0, (void *)&raddr, &raddr_len) < 0) {
      perror("recvfrom()");
      exit(EXIT_FAILURE);
    }
    // 整数转点分十进制的字符串
    inet_ntop(AF_INET, &raddr.sin_addr, ipstr, IPSTRSIZE);
    printf("---MESSAGE FROM %s:%d---\n", ipstr, ntohs(raddr.sin_port));
    // 单字节传输不涉及到大端小端的存储情况
    printf("Name = %s\n", rbuf.name);
    printf("Math = %d\n", ntohl(rbuf.math));
    printf("Chinese = %d\n", ntohl(rbuf.chinese));
  }

  close(sfd);
  exit(0);
}