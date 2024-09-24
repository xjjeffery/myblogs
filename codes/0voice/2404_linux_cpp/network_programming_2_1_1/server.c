#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUFFERSIZE 1024

void *thr_func(void *arg);

int main(int argc, char *argv[]) {
  if (2 != argc) {
    fprintf(stderr, "Usage: %s <port>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // 1. 创建 socket ——> 在 Linux 中创建 socket 只能使用这一种方式
  int sfd = socket(AF_INET, SOCK_STREAM, 0);
  if (-1 == sfd) {
    perror("socket error");
    exit(EXIT_FAILURE);
  }

  // 2. 绑定网络地址信息
  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);  // INADDR_ANY 表示 0.0.0.0，代表所有网段
  serv_addr.sin_port = htons(atoi(argv[1])); // 0~1023 是系统默认的，端口号建议使用 1024 以后的端口号，端口一旦绑定就不能再次绑定
  if (-1 == bind(sfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) {
    close(sfd);
    perror("bind error");
    exit(EXIT_FAILURE);
  }

  // 3. 连接状态由主动改为被动，即等待连接，此时客户端申请连接成功会进入连接队列中
  printf("before listen\n");
  if (-1 == listen(sfd, 10)) {
    close(sfd);
    perror("listen error");
    exit(EXIT_FAILURE);
  }
  printf("after listen\n");

#if 0

  // 4. 从连接队列中获取一个连接，没有则阻塞等待连接
  struct sockaddr_in clnt_addr;
  memset(&clnt_addr, 0, sizeof(clnt_addr));
  socklen_t addr_len = sizeof(clnt_addr);
  int cfd = accept(sfd, (struct sockaddr *)&clnt_addr, &addr_len);
  if (-1 == cfd) {
    close(sfd);
    perror("accept error");
    exit(EXIT_FAILURE);
  }

  char message[BUFFERSIZE] = {0};
  int rlen = 0, wlen = 0;
  while (1) {
    rlen = recv(cfd, message, BUFFERSIZE, 0);
    if (-1 == rlen) {
      close(cfd);
      close(sfd);
      perror("recv() error");
      exit(EXIT_FAILURE);
    } else if (0 == rlen) { // 客户端断开连接
      printf("client %d is disconnect\n", cfd);
      break;
    }

    printf("RECV: %s", message);
    wlen = send(cfd, message, rlen, 0);
    printf("SEND: %d\n", wlen);
  }

  close(cfd);

#elif 0

  while (1) {
    struct sockaddr_in clnt_addr;
    memset(&clnt_addr, 0, sizeof(clnt_addr));
    socklen_t addr_len = sizeof(clnt_addr);
    int cfd = accept(sfd, (struct sockaddr *)&clnt_addr, &addr_len);
    if (-1 == cfd) {
      close(sfd);
      perror("accept error");
      exit(EXIT_FAILURE);
    }

    char message[BUFFERSIZE] = {0};
    int rlen = 0, wlen = 0;
    while (1) {
      rlen = recv(cfd, message, BUFFERSIZE, 0);
      if (-1 == rlen) {
        close(cfd);
        close(sfd);
        perror("recv() error");
        exit(EXIT_FAILURE);
      } else if (0 == rlen) { // 客户端断开连接
        printf("client %d is disconnect\n", cfd);
        break;
      }

      printf("RECV: %s", message);
      wlen = send(cfd, message, rlen, 0);
      printf("SEND: %d\n", wlen);
    }
    close(cfd);
  }

#elif 1

  while (1) {
    struct sockaddr_in clnt_addr;
    memset(&clnt_addr, 0, sizeof(clnt_addr));
    socklen_t addr_len = sizeof(clnt_addr);
    int cfd = accept(sfd, (struct sockaddr *)&clnt_addr, &addr_len);
    if (-1 == cfd) {
      close(sfd);
      perror("accept error");
      exit(EXIT_FAILURE);
    }

    pthread_t tid;
    pthread_create(&tid, NULL, thr_func, &cfd);
    pthread_detach(tid);
  }

#endif

  close(sfd);

  return 0;
}

void* thr_func(void *arg) {
  int cfd = *(int *)arg;

  char message[BUFFERSIZE] = {0};
  int rlen = 0, wlen = 0;
  while (1) {
    rlen = recv(cfd, message, BUFFERSIZE, 0);
    if (-1 == rlen) {
      close(cfd);
      perror("recv() error");
      pthread_exit(NULL);
    } else if (0 == rlen) {
      close(cfd);
      printf("client %d is disconnect\n", cfd);
      break;
    }

    printf("RECV: %s", message);
    wlen = send(cfd, message, rlen, 0);
    printf("SEND: %d\n", wlen);
  }

  pthread_exit(NULL);
}
