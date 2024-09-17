#include <stdio.h>
#include <unistd.h>

int main() {
  printf("current process id = %u, parent process id = %u\n", getpid(), getppid());
  getchar();

  return 0;
}
