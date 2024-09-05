#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (2 != argc) {
    fprintf(stderr, "Usage: %s <file>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  FILE *fp = fopen(argv[1], "r");
  if (NULL == fp) {
    perror("fopen() error");
    exit(EXIT_FAILURE);
  }

  fseek(fp, 0l, SEEK_END);
  long count = ftell(fp);

  printf("file size is %ld\n", count);
  fclose(fp);

  return 0;
}
