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

  while (1) {
    printf("Input message(Q/q to quit): ");
    char message[1024] = {0};
    fgets(message, 1024, stdin);
    if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
      break;

    int len = write(clntfd, message, sizeof(message));
    // 由于服务器端是循环发送，如果客户端一次接收，可能接收的不是完整信息，因此也建议循环读取
    int recv_len = 0;
    while (recv_len < len) {
      int rlen = read(clntfd, message, 1024);
      if (-1 == rlen) {
        perror("read() error");
        close(clntfd);
        exit(EXIT_FAILURE);
      }

      recv_len += rlen;
    }
    printf("read message from server: %s\n", message);
  }
  close(clntfd);

  return 0;
}

