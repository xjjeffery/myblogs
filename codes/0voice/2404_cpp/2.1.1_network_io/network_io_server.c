#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <pthread.h>

void * client_thread(void *arg);

int main() {
  // 1. 创建 socket ——> 在 Linux 中创建 socket 只能使用这一种方式
  int servfd = socket(AF_INET, SOCK_STREAM, 0);
  if (-1 == servfd) {
    perror("socket error");
    exit(EXIT_FAILURE);
  }

  // 2. 绑定网络地址信息
  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // INADDR_ANY 表示 0.0.0.0，代表所有网段
  serv_addr.sin_port = htons(9090); // 0~1023 是系统默认的，端口号建议使用 1024 以后的端口号，端口一旦绑定就不能再次绑定
  if (-1 == bind(servfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) {
    perror("bind error");
    close(servfd);
    exit(EXIT_FAILURE);
  }

  // 3. 进入可连接状态
  printf("before listen\n");
  if (-1 == listen(servfd, 10)) {
    perror("listen error");
    close(servfd);
    exit(EXIT_FAILURE);
  }
  printf("after listen\n");

#if 0
  struct sockaddr_in clnt_addr;
  memset(&clnt_addr, 0, sizeof(clnt_addr));
  int addr_len = sizeof(clnt_addr);
  int clntfd = accept(servfd, (struct sockaddr *)&clnt_addr, &addr_len);
  if (-1 == clntfd) {
    perror("accept error");
    close(servfd);
    exit(EXIT_FAILURE);
  }

  char message[1024] = {0};
  int count = recv(clntfd, message, 1024, 0);
  printf("RECV: %s\n", message);
  count = send(clntfd, message, count, 0);
  printf("SEND: %d\n", count);
#elif 0
  while (1) {
    struct sockaddr_in clnt_addr;
    memset(&clnt_addr, 0, sizeof(clnt_addr));
    int addr_len = sizeof(clnt_addr);
    int clntfd = accept(servfd, (struct sockaddr *)&clnt_addr, &addr_len);
    if (-1 == clntfd) {
      perror("accept error");
      close(servfd);
      exit(EXIT_FAILURE);
    }

    char message[1024] = {0};
    int count = recv(clntfd, message, 1024, 0);
    printf("RECV: %s\n", message);
    count = send(clntfd, message, count, 0);
    printf("SEND: %d\n", count);
  }
#else
  while (1) {
    struct sockaddr_in clnt_addr;
    memset(&clnt_addr, 0, sizeof(clnt_addr));
    int addr_len = sizeof(clnt_addr);
    int clntfd = accept(servfd, (struct sockaddr *)&clnt_addr, &addr_len);
    if (-1 == clntfd) {
      perror("accept error");
      close(servfd);
      exit(EXIT_FAILURE);
    }

    pthread_t cthread;
    pthread_create(&cthread, NULL, client_thread, &clntfd);
  }
#endif
  getchar();
  close(servfd);

  return 0;
}

void* client_thread(void *arg) {
  int clntfd = *(int *)arg;

  while (1) {
    char message[1024] = {0};
    int count = recv(clntfd, message, 1024, 0);
    printf("RECV: %s\n", message);
    count = send(clntfd, message, count, 0);
    printf("SEND: %d\n", count);
  }
}
