# include <stdio.h>
#include <stdlib.h>

static void func1(void); 
static void func2(void);

int main() {
  puts("Begin!");

  if (0 != atexit(func1))
    fprintf(stderr, "can't register func1\n");

  if (0 != atexit(func2))
    fprintf(stderr, "can't register func2\n");

  if (0 != atexit(func2))
    fprintf(stderr, "can't register func2\n");

  puts("End!");

  return 0;
}

static void func1(void) {
  puts("funci is working");
}

static void func2(void) {
  puts("func2 is working");
}
