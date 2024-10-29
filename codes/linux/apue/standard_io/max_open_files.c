#include <stdio.h>

int main() {
  FILE *fp = NULL;
  int count = 0;
  while (1) {
    fp = fopen("/tmp/out", "w");
    if (NULL == fp) {
      perror("fopen() error");
      break;
    }
    count++;
  }

  printf("max file opens is  %d\n", count);

  return 0;
}
