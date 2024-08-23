#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFERSIZE 1024

int main(int argc, char *argv[]) {
  if (3 != argc) {
    fprintf(stderr, "Usage: %s <ip> <port>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // 1. 创建套接字
  int clnt_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (-1 == clnt_fd) {
    perror("socket error");
    exit(EXIT_FAILURE);
  }

  // 2. 向服务器发送连接请求
  struct sockaddr_in clnt_addr;
  clnt_addr.sin_family = AF_INET;
  clnt_addr.sin_addr.s_addr = inet_addr(argv[1]);
  clnt_addr.sin_port = htons(atoi(argv[2]));
  if (-1 == connect(clnt_fd, (struct sockaddr *)&clnt_addr, sizeof(clnt_addr))) {
    perror("connect error");
    exit(EXIT_FAILURE);
  }

  char buffer[BUFFERSIZE] = {0};
  ssize_t len = read(clnt_fd, buffer, BUFFERSIZE);
  if (-1 == len) {
    perror("read error");
    exit(EXIT_FAILURE);
  }

  printf("read from server: %s\n", buffer);
  close(clnt_fd);

  return 0;
}
