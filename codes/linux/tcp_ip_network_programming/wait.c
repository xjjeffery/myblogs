#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int arhc, char *argv[]) {
  pid_t pid = fork();
  if (-1 == pid) {
    perror("fork() error");
    exit(EXIT_FAILURE);
  }

  int status;
  if (0 == pid) {
    return 3;
  } else {
    printf("Child pid: %d\n", pid);
    pid = fork();
    if (-1 == pid) {
      perror("fork2() error");
      exit(EXIT_FAILURE);
    }

    if (0 == pid) {
      exit(7);
    } else {
      printf("Child pid: %d\n", pid);
      wait(&status);  // 如果没有已终止的进程，程序会一直阻塞在此处
      if (WIFEXITED(status))
        printf("Child send one: %d\n", WEXITSTATUS(status));

      wait(&status);
      if (WIFEXITED(status))
        printf("Child send one: %d\n", WEXITSTATUS(status));
      sleep(30);
    }
  }

  return 0;
}
