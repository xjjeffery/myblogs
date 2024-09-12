#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FILENAME "/tmp/out"
#define FIVEG 5LL*1024LL*1024LL*1024LL

int main() {
  int fd = open(FILENAME, O_WRONLY | O_CREAT | O_TRUNC, 0666);
  if (-1 == fd) {
    perror("open() error");
    exit(EXIT_FAILURE);
  }

  lseek(fd, FIVEG-1, SEEK_SET);
  write(fd, "", 1);
  close(fd);

  return 0;
}
