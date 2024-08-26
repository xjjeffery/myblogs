#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FILEPATH "/tmp/out"

int main() {
  int fd = open(FILEPATH, O_RDONLY);
  if (-1 == fd) {
    perror("open() error");
    exit(EXIT_FAILURE);
  }

  char message[1024] = {0};
  int len = read(fd, message, 1024);
  if (-1 == len) {
    perror("read() error");
    close(fd);
    exit(EXIT_FAILURE);
  }

  printf("message = %s", message);
  close(fd);

  return 0;
}
