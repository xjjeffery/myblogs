#include <stdio.h>
#include <stdlib.h>

#define DEFAULTSIZE 120

ssize_t my_getline(char **lineptr, size_t *n, FILE *stream);

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
  while (-1 != (rlen = my_getline(&buffer, &size, sfp)))
    fputs(buffer, dfp);
  // 有打开就要有关闭
  fclose(dfp);
  fclose(sfp);

  return 0;
}

ssize_t my_getline(char **lineptr, size_t *n, FILE *stream) {
  if (NULL == lineptr || NULL == n || NULL == stream)
    return -1;

  size_t default_size = DEFAULTSIZE;
  if (NULL == *lineptr) {
    *lineptr = malloc(default_size);
    if (NULL == *lineptr) {
      perror("malloc() error");
      return -1;
    }
    *n = default_size;
  }

  int count = 0;
  int ch;
  while (EOF != (ch = fgetc(stream))) {
    if (count+1 >= default_size) {
      default_size += DEFAULTSIZE;
      char *temp = realloc(*lineptr, default_size);
      if (NULL == temp) {
        free(*lineptr);
        perror("realloc() error");
        return -1;
      }
      *lineptr = temp;
      *n = default_size;
    }

    (*lineptr)[count++] = ch;

    if ('\n' == ch)
      break;
  }

  if (EOF == ch || 0 == count)
    return -1;

  (*lineptr)[count] = '\0';
  return count;
}
