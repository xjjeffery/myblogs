#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arpa/inet.h>

void read_childproc(int sig);

int main(int argc, char *argv[]) {
  if (2 != argc) {
    fprintf(stderr, "Usage: %s <port>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int servfd = socket(AF_INET, SOCK_STREAM, 0);
  if (-1 == servfd) {
    perror("socket() error");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(atoi(argv[1]));
  if (-1 == bind(servfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) {
    perror("bind() error");
    close(servfd);
    exit(EXIT_FAILURE);
  }

  if (-1 == listen(servfd, 5)) {
    perror("listen() error");
    close(servfd);
    exit(EXIT_FAILURE);
  }

  struct sigaction act;
  act.sa_handler = read_childproc;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;
  sigaction(SIGCHLD, &act, 0);

  for (int i = 0; i < 5; ++i) {
    struct sockaddr_in clnt_addr;
    memset(&clnt_addr, 0, sizeof(clnt_addr));
    int addr_len = sizeof(clnt_addr);
    int str_len = 0;
    char message[1024] = {0};
    int clntfd = accept(servfd, (struct sockaddr *)&clnt_addr, &addr_len);
    if (-1 == clntfd) {
      perror("accept() error");
      continue;
    } else {
      puts("new client connect...");
    }

    pid_t pid = fork();
    if (-1 == pid) {
      perror("fork() error");
      close(clntfd);
      continue;
    }

    if (0 == pid) {
      // 因为fork以后会将父进程的所有东西都进行了复制，如套接字
      // 因此在子进程中需要将复制过来的套接字关闭
      close(servfd);
      while (0 != (str_len = read(clntfd, message, 1024)))
        write(clntfd, message, str_len);

      close(clntfd);
      puts("client disconnected...");
      return 0;
    } else {
      close(clntfd);
    }
  }

  close(servfd);

  return 0;
}

void read_childproc(int sig) {
  pid_t pid;
  int status;
  pid = waitpid(-1, &status, WNOHANG);
  printf("remove proc id: %d \n", pid);
}
