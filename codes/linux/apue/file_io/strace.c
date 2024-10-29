#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
  putchar('a');
  write(STDOUT_FILENO, "b", 1);

  putchar('a');
  write(STDOUT_FILENO, "b", 1);

  putchar('a');
  write(STDOUT_FILENO, "b", 1);

  // 最后的输出结果是 bbbaaa

  return 0;
}
