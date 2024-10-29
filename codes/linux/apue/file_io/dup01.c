#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FILEPATH "/tmp/out"

int main(int argc, char *argv[]) {
  // 首先关闭标准输出的文件描述符
  close(STDOUT_FILENO);
  // 打开一个文件，此时的文件描述符表中最小的可用就是 1
  int fd = open(FILEPATH, O_CREAT | O_WRONLY | O_TRUNC, 0666);
  if (-1 == fd) {
    perror("open() error");
    exit(EXIT_FAILURE);
  }

  // 保持下面的内容不动，将下面的内容输出的到文件中
  puts("Hello World!");

  return 0;
}
