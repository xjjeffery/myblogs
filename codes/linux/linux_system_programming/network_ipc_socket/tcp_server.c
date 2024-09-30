#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <time.h>

#define PORT "9090"
#define FMT_STAMP "%lld\r\n" // 格式化参数
#define IPSTRSIZE 40
#define BUFSIZE 1024

static void server_job(int sd) {
    char buf[BUFSIZE];
    int len;
    // 将格式化数据写入到 buf 中,返回写入的字符总数
    len = sprintf(buf, FMT_STAMP, (long long)time(NULL));
    if(send(sd, buf, len, 0) < 0) {
        perror("send()");
        exit(1);
    }
}

int main() {
  // 1. 创建套接字
  int sfd = socket(AF_INET, SOCK_STREAM, 0);
  if (-1 == sfd) {
    perror("socket() erorr");
    exit(EXIT_FAILURE);
  }

  int val = 1;
  if (-1 == setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val))) {
    perror("setsockopt() error");
    exit(EXIT_FAILURE);
  }

  // 2. 绑定套接字
  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(atoi(PORT));
  if (-1 == bind(sfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) {
    perror("bind() error");
    exit(EXIT_FAILURE);
  }

  // 3. 启动监听
  if (-1 == listen(sfd, 10)) {
    perror("listen() error");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in clnt_addr;
  socklen_t addr_len = sizeof(clnt_addr);
  while (1) {
    // 4. 接收连接
    int cfd = accept(sfd, (struct sockaddr *)&clnt_addr, &addr_len);
    if (-1 == cfd) {
      perror("accept() error");
      exit(EXIT_FAILURE);
    }
    char ipstr[IPSTRSIZE] = {0};
    inet_ntop(AF_INET, &clnt_addr.sin_addr.s_addr, ipstr, IPSTRSIZE);
    printf("Client: %s:%d\n", ipstr, ntohs(clnt_addr.sin_port));
    // 5. 数据处理
    server_job(cfd);
    close(cfd);
  }

  close(sfd);

  return 0;
}