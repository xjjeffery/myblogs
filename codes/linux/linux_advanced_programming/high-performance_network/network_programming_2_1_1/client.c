#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUFFERSIZE 1024

int main(int argc, char *argv[]) {
  if (3 != argc) {
    fprintf(stderr, "Usage: %s <ip> <port>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // 1. 创建套接字
  int cfd = socket(AF_INET, SOCK_STREAM, 0);
  if (-1 == cfd) {
    perror("socket() error");
    exit(EXIT_FAILURE);
  }

  // 向服务器端发送连接请求
  struct sockaddr_in clnt_addr;
  clnt_addr.sin_family = AF_INET;
  inet_pton(AF_INET, argv[1], &clnt_addr.sin_addr.s_addr);
  clnt_addr.sin_port = htons(atoi(argv[2]));
  if (-1 == connect(cfd, (struct sockaddr *)&clnt_addr, sizeof(clnt_addr))) {
    perror("connect() error");
    exit(EXIT_FAILURE);
  }

  char message[BUFFERSIZE] = {0};
  while (1) {
    memset(message, 0, BUFFERSIZE);
    printf("Please input message(q/Q to quit): ");
    fgets(message, BUFFERSIZE-1, stdin);
    if (!strcmp(message, "Q\n") || !strcmp(message, "q\n"))
      break;

    int wlen = write(cfd, message, sizeof(message));
    printf("WRITE: %s", message);
    int rlen = read(cfd, message, BUFFERSIZE);
    if (rlen < 0) {
      perror("read() error");
      break;
    }

    printf("READ: %s", message);
  }

  close(cfd);

  return 0;
}