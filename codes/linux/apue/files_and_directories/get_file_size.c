#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

static off_t get_file_size(const char *pathname);

int main(int argc, char *argv[]) {
  if (2 != argc) {
    fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  off_t size = get_file_size(argv[1]);
  printf("%s size is %lu\n", argv[1], size);

  return 0;
}

static off_t get_file_size(const char *pathname) {
  struct stat stat_buf;
  if (-1 == stat(pathname, &stat_buf)) {
    perror("stat() error");
    return -1;
  }

  return stat_buf.st_size;
}
