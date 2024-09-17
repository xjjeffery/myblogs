#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

int main(int argc, char *argv[]) {
  pid_t pid = fork();
  if (-1 == pid) {
    perror("fork() error");
    exit(EXIT_FAILURE);
  }

  if (0 == pid) {
    // execl("/usr/bin/date", "date", "+%Y", NULL);
    execl("/usr/bin/sleep", "httpd", "100", NULL);
    perror("execl() error");
    exit(EXIT_FAILURE);
  } else {
    wait(NULL);
  }

  return 0;
}
