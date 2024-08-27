#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

void read_routine(int sock, char *buf);
void write_routine(int sock, char *buf);

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

  pid_t pid = fork();
  if (-1 == pid) {
    perror("fork() error");
    close(clntfd);
    exit(EXIT_FAILURE);
  }

  char message[1024] = {0};
  if (0 == pid)
    write_routine(clntfd, message);
  else
    read_routine(clntfd, message);

  close(clntfd);

  return 0;
}

void read_routine(int sock, char *buf) {
  while(1) {
    int str_len = read(sock, buf, 1024);
    if (0 == str_len)
      return;

    printf("message from server: %s\n", buf);
  }
}

void write_routine(int sock, char *buf) {
  while (1) {
    printf("Input message(Q/q to quit): ");
    fgets(buf, 1024, stdin);
    if (!strcmp(buf, "q\n") || !strcmp(buf, "Q\n")) {
      shutdown(sock, SHUT_WR);
      return;
    }
    write(sock, buf, sizeof(buf));
  }
}
