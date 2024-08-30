#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <poll.h>

int main() {
  int servfd = socket(AF_INET, SOCK_STREAM, 0);
  if (-1 == servfd) {
    perror("socket() error");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(9090);
  if (-1 == bind(servfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) {
    perror("bind() error");
    close(servfd);
    exit(EXIT_FAILURE);
  }

  if (-1 == listen(servfd, 10)) {
    perror("listen() error");
    close(servfd);
    exit(EXIT_FAILURE);
  }

  struct pollfd fds[1024] = {0};
  fds[servfd].fd = servfd;
  fds[servfd].events = POLLIN;
  int maxfd = servfd;

  struct sockaddr_in clnt_addr;
  memset(&clnt_addr, 0, sizeof(clnt_addr));
  socklen_t addr_len = sizeof(clnt_addr);
  while (1) {
    int ret = poll(fds, 1024, -1);
    if (-1 == ret) {
      perror("select() error");
      break;
    } else if (0 == ret) {
      printf("No data");
      continue;
    }

    for (int i = 0; i < maxfd+1; ++i) {
      if (fds[i].revents & POLLIN) {
        if (i == servfd) {
          int clntfd = accept(servfd, (struct sockaddr *)&clnt_addr, &addr_len);
          if (-1 == clntfd) {
            perror("accept() error");
            break;
          } else {
            printf("client connect: %d\n", clntfd);
          }

          fds[clntfd].fd = clntfd;
          fds[clntfd].events = POLLIN;
          if (clntfd > maxfd)
            maxfd = clntfd;
        } else {
          char message[1024] = {0};
          int count = recv(i, message, 1024, 0);
          if (0 == count) {
            printf("client disconnect: %d\n", i);
            close(i);
            fds[i].fd = -1;
            fds[i].events = 0;
            continue;
          }

          printf("RECV: %s\n", message);
          count = send(i, message, count, 0);
          printf("Count: %d\n", count);
        }
      }
    }
  }

  close(servfd);

  return 0;
}

