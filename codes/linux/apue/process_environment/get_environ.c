#include <stdio.h>

extern char **environ;

int main(int argc, char *argv[]) {
  for (int i = 0; NULL != environ[i]; ++i)
    puts(environ[i]);

  return 0;
}
