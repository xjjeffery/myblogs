#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFERSIZE 1024

void *client_thread(void *arg) {
  int clnt_fd = *(int *)arg;
  while (1) {
    char buffer[BUFFERSIZE] = {0};
    if (-1 != recv(clnt_fd, buffer, BUFFERSIZE, 0)) {
      printf("RECV: %s\n", buffer);
    } else {
      perror("recv error");
      exit(EXIT_FAILURE);
    }
  }
}

int main() {
  // 1. 创建 socket ——> 在 Linux 中创建 socket 只能使用这一种方式
  int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (-1 == sock_fd) {
    perror("socket error");
    exit(EXIT_FAILURE);
  }

  // 2. 绑定本地端口
  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // INADDR_ANY 表示 0.0.0.0，代表所有网段
  serv_addr.sin_port = htons(9090); // 0~1023 是系统默认的，端口号建议使用 1024 以后的，端口号冲突会绑定失败
  if (-1 == bind(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) {
    perror("bind error");
    exit(EXIT_FAILURE);
  }

  // 3. listen 打开可连接状态
  if (-1 == listen(sock_fd, 10)) {
    perror("listen error");
    exit(EXIT_FAILURE);
  }

#if 0
  // 4. 获取与客户端的联系
  struct sockaddr_in clnt_addr;
  socklen_t len = sizeof(clnt_addr);
  int clnt_fd = accept(sock_fd, (struct sockaddr *)&clnt_addr, &len);
  if (-1 == clnt_fd) {
    perror("accept error");
    exit(EXIT_FAILURE);
  }

  // 5. 取出客户端发来的数据
  char buffer[BUFFERSIZE] = {0};
  if (-1 != recv(clnt_fd, buffer, BUFFERSIZE, 0)) {
    printf("RECV: %s\n", buffer);
  } else {
    perror("recv error");
    exit(EXIT_FAILURE);
  }
#elif 0
  while (1) {
    // 4. 获取与客户端的联系
    struct sockaddr_in clnt_addr;
    socklen_t len = sizeof(clnt_addr);
    int clnt_fd = accept(sock_fd, (struct sockaddr *)&clnt_addr, &len);
    if (-1 == clnt_fd) {
      perror("accept error");
      exit(EXIT_FAILURE);
    }

    // 5. 取出客户端发来的数据
    char buffer[BUFFERSIZE] = {0};
    if (-1 != recv(clnt_fd, buffer, BUFFERSIZE, 0)) {
      printf("RECV: %s\n", buffer);
    } else {
      perror("recv error");
      exit(EXIT_FAILURE);
    }
  }
#elif 1
  while (1) {
    // 4. 获取与客户端的联系
    struct sockaddr_in clnt_addr;
    socklen_t len = sizeof(clnt_addr);
    int clnt_fd = accept(sock_fd, (struct sockaddr *)&clnt_addr, &len);
    if (-1 == clnt_fd) {
      perror("accept error");
      exit(EXIT_FAILURE);
    }

    pthread_t clnt_thread;
    if (0 != pthread_create(&clnt_thread, NULL, client_thread, &clnt_fd)) {
      perror("pthread create error");
      exit(EXIT_FAILURE);
    }
  }
#endif
  getchar();

  return 0;
}
