#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

int main(int argc, char *argv[]) {
  if (3 > argc) {
    fprintf(stderr, "Usage: %s <userid> [command]\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  pid_t pid = fork();
  if (-1 == pid) {
    perror("fork() error");
    exit(EXIT_FAILURE);
  }

  if (0 == pid) {
    setuid(atoi(argv[1]));
    execvp(argv[2], argv+2);
    perror("execvp() error");
    exit(EXIT_FAILURE);
  } else {
    wait(NULL);
  }

  return 0;
}
