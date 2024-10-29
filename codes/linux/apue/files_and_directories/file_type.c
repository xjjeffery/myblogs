#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

static char file_type(const char *pathname);

int main(int argc, char *argv[]) {
  if (2 != argc) {
    fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  char ch = file_type(argv[1]);
  switch (ch) {
  case '-':
    printf("%s is regular file\n", argv[1]);
    break;
  case 'l':
    printf("%s is symbolic link\n", argv[1]);
    break;
  case 'd':
    printf("%s is directory\n", argv[1]);
    break;
  case 'c':
    printf("%s is character device\n", argv[1]);
    break;
  case 'b':
    printf("%s is block device\n", argv[1]);
    break;
  case 'p':
    printf("%s is FIFO\n", argv[1]);
    break;
  case 's':
    printf("%s is socket\n", argv[1]);
    break;
  }

  return 0;
}

static char file_type(const char *pathname) {
  struct stat stat_buf;
  if (-1 == stat(pathname, &stat_buf)) {
    perror("stat() error");
    exit(EXIT_FAILURE);
  }

  if (S_ISREG(stat_buf.st_mode))
    return '-';
  else if (S_ISDIR(stat_buf.st_mode))
    return 'd';
  else if (S_ISCHR(stat_buf.st_mode))
    return 'c';
  else if (S_ISBLK(stat_buf.st_mode))
    return 'b';
  else if (S_ISLNK(stat_buf.st_mode))
    return 'l';
  else if (S_ISFIFO(stat_buf.st_mode))
    return 'p';
  else if (S_ISSOCK(stat_buf.st_mode))
    return 's';
}
