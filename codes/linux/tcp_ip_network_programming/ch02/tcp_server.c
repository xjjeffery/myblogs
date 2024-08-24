#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFERSIZE 1024

int main(int argc, char *argv[]) {
  if (2 != argc) {
    fprintf(stderr, "Usage: %s <port>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // 1. 创建套接字
  int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (-1 == sock_fd) {
    perror("socket error");
    exit(EXIT_FAILURE);
  }

  // 2. 绑定地址信息
  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(atoi(argv[1]));
  if (-1 == bind(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) {
    perror("bind error");
    exit(EXIT_FAILURE);
  }

  // 3. 建立接收连接通道
  if (-1 == listen(sock_fd, 5)) {
    perror("listen error");
    exit(EXIT_FAILURE);
  }

  // 4. 接收连接请求
  struct sockaddr_in clnt_addr;
  socklen_t len = sizeof(clnt_addr);
  int clnt_fd = accept(sock_fd, (struct sockaddr *)&clnt_addr, &len);
  if (-1 == clnt_fd) {
    perror("accept error");
    exit(EXIT_FAILURE);
  }

  char messages[] = "hello world!";
  for (int i = 0; i < sizeof(messages); ++i) {
    write(clnt_fd, &messages[i], 1);
  }
  // write(clnt_fd, messages, sizeof(messages));

  close(clnt_fd);
  close(sock_fd);

  return 0;
}
