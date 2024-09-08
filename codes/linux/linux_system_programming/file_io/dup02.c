#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#define FILENAME "/tmp/out"

int main() {
  // 首先打开一个文件，此时的文件描述符表中最小的可用就是 3
  int fd = open(FILENAME, O_CREAT | O_TRUNC | O_WRONLY, 0666);
  if (fd == -1) {
    perror("open file failed");
    exit(EXIT_FAILURE);
  }

  // 关闭标准输出
  if (STDOUT_FILENO != fd) {
    close(STDOUT_FILENO);
    // 使用 dup 拷贝，此时的文件描述符就是 1
    dup(fd);
  }
  // 保持下面的内容不动，将下面的内容输出的到文件中
  puts("Hello World!");

  return 0;
}
