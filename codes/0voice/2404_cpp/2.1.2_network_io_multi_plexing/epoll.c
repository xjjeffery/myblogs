#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

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

  int epfd = epoll_create(1);
  if (-1 == epfd) {
    perror("epoll create error");
    close(servfd);
    exit(EXIT_FAILURE);
  }

  struct epoll_event ev;
  ev.events = EPOLLIN;
  ev.data.fd = servfd;
  epoll_ctl(epfd, EPOLL_CTL_ADD, servfd, &ev);

  struct sockaddr_in clnt_addr;
  memset(&clnt_addr, 0, sizeof(clnt_addr));
  socklen_t addr_len = sizeof(clnt_addr);
  while (1) {
    struct epoll_event events[1024] = {0};
    int ret = epoll_wait(epfd, events, 1024, -1);
    if (-1 == ret) {
      perror("select() error");
      break;
    }

    for (int i = 0; i < ret; ++i) {
      if (events[i].events & EPOLLIN) {
        int curfd = events[i].data.fd;
        if (curfd == servfd) {
          int clntfd = accept(servfd, (struct sockaddr *)&clnt_addr, &addr_len);
          if (-1 == clntfd) {
            perror("accept() error");
            break;
          } else {
            printf("client connect: %d\n", clntfd);
          }

          ev.events = EPOLLIN;
          ev.data.fd = clntfd;
          epoll_ctl(epfd, EPOLL_CTL_ADD, clntfd, &ev);
        } else {
          char message[1024] = {0};
          int count = recv(curfd, message, 1024, 0);
          if (0 == count) {
            printf("client disconnect: %d\n", curfd);
            close(curfd);
            epoll_ctl(epfd, EPOLL_CTL_DEL, curfd, NULL);
            continue;
          }

          printf("RECV: %s\n", message);
          count = send(curfd, message, count, 0);
          printf("Count: %d\n", count);
        }
      }
    }
  }

  close(servfd);

  return 0;
}
