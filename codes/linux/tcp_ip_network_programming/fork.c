#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int gval = 10;
int main(int argc, char *argv[]) {
  int lval = 20;
  gval++;
  lval += 5;
  pid_t pid = fork();
  if (-1 == pid) {
    perror("fork() error");
    exit(EXIT_FAILURE);
  }

  // 父进程返回子进程的 pid，子进程返回 0
  if (0 < pid) {
    gval += 2;
    lval += 2;
  } else if (0 == pid) {
    gval -= 2;
    lval -= 2;
  }

  if (0 == pid)
    printf("Child process: gval = %d, lval = %d\n", gval, lval);
  else if (0 < pid)
    printf("Parent process: gval = %d, lval = %d\n", gval, lval);

  getchar();
  return 0;
}
