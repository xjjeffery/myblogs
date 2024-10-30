#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  puts("Begin");
  execl("/usr/bin/date", "date", "+%Y", NULL);
  puts("End");

  return 0;
}
