#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>

#define LEFT  30000000
#define RIGHT 30000200

int main() {
  for (int i = LEFT; i <= RIGHT; ++i) {
    pid_t pid = fork();
    if (-1 == pid) {
      perror("fork() error");
      exit(EXIT_FAILURE);
    }

    bool is_primer = true;
    if (0 == pid) {
      for (int j = 2; j <= sqrt(i); ++j) {
        if (0 == i % j) {
          is_primer = false;
          break;
        }
      }

      if (is_primer)
        printf("%d is primer\n", i);

      exit(EXIT_SUCCESS);
    }
  }

  pid_t child_pid;
  for (int i = LEFT; i < RIGHT; ++i) {
    child_pid = wait(NULL);
    printf("Child process with pid: %d\n", child_pid);
  }
  // sleep(1000);
  return 0;
}
