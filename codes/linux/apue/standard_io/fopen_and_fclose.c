#include <stdio.h>
#include <stdlib.h>

int main() {
  FILE *fp = fopen("tmp", "r");
  if (NULL == fp) {
    perror("fopen() error");
    exit(EXIT_FAILURE);
  }

  puts("OK!");
  fclose(fp);

  return 0;
}
