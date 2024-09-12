#include <glob.h>
#include <stdio.h>
#include <stdlib.h>

#define PATTERN "/etc/a*.conf"

int main(int argc, const char *argv[]) {
  glob_t pattern_glob;
  glob(PATTERN, 0, NULL, &pattern_glob);

  for (int i = 0; i < pattern_glob.gl_pathc; ++i) 
    puts(pattern_glob.gl_pathv[i]);

  globfree(&pattern_glob);
  return 0;
}
