#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <grp.h>

int main(int argc, char *argv[]) {
  if (1 == argc) {
    fprintf(stderr, "Usage: %s filename ...\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  struct group *group_line = NULL;
  for (int i = 1; i < argc; ++i) {
    struct stat stat_buf;
    if (-1 == stat(argv[i], &stat_buf)) {
      perror("stat() error");
      exit(EXIT_FAILURE);
    }

    group_line = getgrgid(stat_buf.st_gid);
    printf("%s belong to %s\n", argv[i], group_line->gr_name);
  }

  return 0;
}
