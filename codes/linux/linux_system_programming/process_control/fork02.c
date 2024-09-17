#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
  printf("[%u] Begin!\n", getpid());

  fflush(NULL); // 在 fork 之前刷新所有的缓冲区
  pid_t pid = fork();
  if (-1 == pid) {
    perror("fork() error");
    exit(EXIT_FAILURE);
  }

  if (0 == pid) { // 子进程
    printf("[%d] is child process\n", getpid());
  } else {  // 父进程
    printf("[%d] is parent process\n", getpid());
  }

  printf("[%u] End!\n", getpid());

  // getchar();

  return 0;
}
