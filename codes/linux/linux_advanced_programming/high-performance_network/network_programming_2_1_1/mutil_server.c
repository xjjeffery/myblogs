#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#define BUFFER_LENGTH 1024

void *data_handler(void *arg) {
  int fd = *(int *)arg;
  char message[BUFFER_LENGTH] = {0};
  while (1) {
    memset(message, 0, BUFFER_LENGTH);
    int ret = recv(fd, message, BUFFER_LENGTH, 0);
    if (0 == ret) {
      printf("client is disconnected: %d\n", fd);
      break;
    } else if (0 > ret) {
      perror("recv() error");
      exit(EXIT_FAILURE);
    }

    printf("RECV: %s\n", message);
    ret = send(fd, message, sizeof(message), 0);
    printf("SEND: %d\n", ret);
  }
}

int main(int argc, char *argv[]) {
  if (2 != argc) {
    fprintf(stderr, "Usage: %s <port>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // 创建套接字
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (-1 == socket_fd) {
    perror("socket() error");
    exit(EXIT_FAILURE);
  }

  // 绑定网络地址
  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(atoi(argv[1]));
  if (-1 == bind(socket_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) {
    perror("bind() error");
    exit(EXIT_FAILURE);
  }

  // 监听地址
  if (-1 == listen(socket_fd, 10)) {
    perror("listen() error");
    exit(EXIT_FAILURE);
  }

  // 受理客户端连接
  struct sockaddr_in clnt_addr;
  socklen_t addr_len = sizeof(clnt_addr);
  int clnt_fd = 0;
  while (1) {
    if (-1 == (clnt_fd = accept(socket_fd, (struct sockaddr *)&clnt_addr, &addr_len))) {
      perror("accept() error");
      exit(EXIT_FAILURE);
    }
    printf("clint is conncted: %d\n", clnt_fd);

    pthread_t thread;
    int err = pthread_create(&thread, NULL, data_handler, &clnt_fd);
    if (err) {
      fprintf(stderr, "pthread_create error: %s\n", strerror(err));
      exit(EXIT_FAILURE);
    }
  }

  // 有打开就要有关闭
  close(socket_fd);
  return 0;
}