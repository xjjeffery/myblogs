#include <stdio.h>
#include <stdlib.h>

#define MAXSIZE 64

int main() {
  char name[L_tmpnam] = {0};
  char line[MAXSIZE] = {0};
  printf("%s\n", tmpnam(NULL));
  tmpnam(name);
  printf("%s\n", name);

  FILE *fp = NULL;
  if ((fp = tmpfile()) == NULL) {
    fprintf(stderr, "tmpfile error\n");
    exit(EXIT_FAILURE);
  }

  fputs("one line of output\n", fp);
  rewind(fp);
  if (fgets(line, sizeof(line), fp) == NULL) {
    fprintf(stderr, "fgets error\n");
    exit(EXIT_FAILURE);
  }

  fputs(line, stdout);

  return 0;
}
