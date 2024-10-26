#include <arpa/inet.h>
#include <assert.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define INET_ADDRSTRLEN 16
#define BUFFER_LENGTH 1024

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <port_number>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // 1. 创建 socket
  int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  assert(sock_fd >= 0);

  // 2. 命名 socket
  struct sockaddr_in serv_addr;
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(atoi(argv[1]));
  int ret = bind(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
  assert(ret != -1);

  // 3. 监听 socket
  ret = listen(sock_fd, 2);
  assert(ret != -1);

  // 4. 接受连接
  struct sockaddr_in clnt_addr;
  socklen_t addr_len = sizeof(clnt_addr);
  while (1) {
    int connfd = accept(sock_fd, (struct sockaddr *)&clnt_addr, &addr_len);
    if (-1 == connfd) {
      assert(connfd != -1);
    }
    
    char remote[INET_ADDRSTRLEN] = {0};
    printf("clinet connected with ip: %s and port: %d\n",
              inet_ntop(AF_INET, &clnt_addr.sin_addr, remote, INET6_ADDRSTRLEN), ntohs(clnt_addr.sin_port));
    
    char rbuf[BUFFER_LENGTH] = {0};
    while (1) {
      memset(rbuf, 0, BUFFER_LENGTH);
      ret = recv(connfd, rbuf, BUFFER_LENGTH, 0);
      if (-1 == connfd) {
        perror("recv() error");
        break;
      } else if (0 == ret) {
        printf("client disconnected: %d\n", connfd);
        close(connfd);
        break;
      }
      
      printf("RECV: %s\n", rbuf);
      ret = send(connfd, rbuf, strlen(rbuf)+1, 0);
      printf("SEND: %d\n", ret);
    }
  }
  close(sock_fd);

  return 0;
}