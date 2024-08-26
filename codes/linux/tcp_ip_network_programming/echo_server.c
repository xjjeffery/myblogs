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
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (-1 == sockfd) {
    perror("socket() error");
    exit(EXIT_FAILURE);
  }

  // 2. 绑定地址信息
  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(atoi(argv[1]));
  if (-1 == bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) {
    perror("bind() error");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  // 3. 打开可连接状态，进入监听
  if (-1 == listen(sockfd, 5)) {
    perror("listen() error");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in clnt_addr;
  memset(&clnt_addr, 0, sizeof(clnt_addr));
  socklen_t clnt_len = sizeof(clnt_addr);
  for (int i = 0; i < 5; ++i) {
    int clntfd = accept(sockfd, (struct sockaddr *)&clnt_addr, &clnt_len);
    if (-1 == clntfd) {
      perror("accept() error");
      close(sockfd);
      exit(EXIT_FAILURE);
    } else {
      printf("Connected clinet: %d\n", clntfd);
    }

    char message[1024] = {0};
    int str_len = 0;
    while (0 != (str_len = read(clntfd, message, 1024)))
      write(clntfd, message, str_len);

    close(clntfd);
  }
  close(sockfd);

  return 0;
}
