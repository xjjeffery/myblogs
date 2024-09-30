#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>

#define PORT "9090"
#define FMT_STAMP "%lld\r\n" // 格式化参数

int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <ip>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // 1. 创建套接字
  int cfd = socket(AF_INET, SOCK_STREAM, 0);
  if (-1 == cfd) {
    perror("socket() error");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in clnt_addr;
  clnt_addr.sin_family = AF_INET;
  clnt_addr.sin_port = htons(atoi(PORT));
  inet_pton(AF_INET, argv[1], &clnt_addr.sin_addr.s_addr);

  // 2. 发送连接请求
  if (-1 == connect(cfd, (struct sockaddr *)&clnt_addr, sizeof(clnt_addr))) {
    perror("connect() error");
    exit(EXIT_FAILURE);
  }

  // 系统 io 转换为标准 io
  // r+表示打开可读写的文件，且该文件必须存在
  FILE *fp = fdopen(cfd, "r+");
  if (fp == NULL) {
    perror("fdopen() error");
    exit(EXIT_FAILURE);
  }

  // 根据数据格式 FMT_STAMP 从 fp 中读取数据到 stamp 中
  long long stamp;
  if (fscanf(fp, FMT_STAMP, &stamp) < 1) {
    fprintf(stderr, "Bad format!\n");
  } else {
    fprintf(stdout, "stamp = %lld\n", stamp);
  }

  // 按照标准 io 的方式关闭 fp
  fclose(fp);
  exit(0);
}