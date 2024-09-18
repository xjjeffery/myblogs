#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
  umask(0);

  pid_t pid = fork();
  if (-1 == pid) {
    perror("fork() error");
    exit(EXIT_FAILURE);
  }

  if (0 == pid) {
    // 调用 setsid
    if (-1 == setsid()) {
      perror("setsid() error");
      exit(EXIT_FAILURE);
    }

    // 更改工作目录
    if (chdir("/") < 0)
      fprintf(stderr, "%s: can't change directory to /", argv[0]);

    // 关闭所有继承的文件描述符
    for (int i = 0; i < 1024; ++i)
      close(i);

    // 重定向
    int fd0 = open("/dev/null", O_RDWR);
    if (-1 == fd0) {
      perror("open() error");
      exit(EXIT_FAILURE);
    }

    int fd1 = dup(0);
    int fd2 = dup(0);

    for (;;) {
      // 守护进程要完成的任务
    }

  } else {
    exit(EXIT_SUCCESS);
  }

  return 0;
}
