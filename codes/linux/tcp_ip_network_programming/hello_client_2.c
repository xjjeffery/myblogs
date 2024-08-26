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
  int clntfd = socket(AF_INET, SOCK_STREAM, 0);
  if (-1 == clntfd) {
    perror("socket() error");
    exit(EXIT_FAILURE);
  }

  // 2. 发送连接请求
  struct sockaddr_in clnt_addr;
  memset(&clnt_addr, 0, sizeof(clnt_addr));
  clnt_addr.sin_family = AF_INET;
  clnt_addr.sin_addr.s_addr = inet_addr(argv[1]);
  clnt_addr.sin_port = htons(atoi(argv[2]));
  if (-1 == connect(clntfd, (struct sockaddr *)&clnt_addr, sizeof(clnt_addr))) {
    perror("connect() error");
    close(clntfd);
    exit(EXIT_FAILURE);
  } else {
    printf("Connected ......\n");
  }

  char message[1024] = {0};
  int len = 0;
  int index = 0;
  int read_sum = 0;
  while (len = read(clntfd, &message[index++], 1)){
    if (-1 == len) {
      perror("read() error");
      close(clntfd);
      exit(EXIT_FAILURE);
    }
    read_sum += len;
  }

  printf("read message from server: %s\n", message);
  printf("function read call count: %d\n", read_sum);
  close(clntfd);

  return 0;
}

