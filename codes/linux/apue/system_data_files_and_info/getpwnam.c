#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>

int main(int argc, char *argv[]) {
  if (1 == argc) {
    fprintf(stderr, "Usage: %s filename ...\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  struct passwd *passwd_line = NULL;
  for (int i = 0; i < argc; ++i) {
    struct stat stat_buf;
    if (-1 == stat(argv[i], &stat_buf)) {
      perror("stat() error");
      exit(EXIT_FAILURE);
    }

    passwd_line = getpwuid(stat_buf.st_uid);
    printf("%s name is %s\n", argv[i], passwd_line->pw_name);
  }

  return 0;
}
