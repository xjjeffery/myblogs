#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  if (3 != argc) {
    fprintf(stderr, "Usage: %s <source> <dest>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // 打开两个文件，一个进行读，另一个进行写
  FILE *sfp = fopen(argv[1], "r");
  if (NULL == sfp) {
    perror("source fopen() error");
    exit(EXIT_FAILURE);
  }

  FILE *dfp = fopen(argv[2], "w");
  if (NULL == dfp) {
    perror("dest fopen() error");
    fclose(sfp);
    exit(EXIT_FAILURE);
  }

  char *buffer = NULL;
  size_t size = 0;
  ssize_t rlen = 0;
  while (-1 != (rlen = getline(&buffer, &size, sfp)))
    fputs(buffer, dfp);
  // 有打开就要有关闭
  fclose(dfp);
  fclose(sfp);

  return 0;
}
