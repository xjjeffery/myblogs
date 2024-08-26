#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FILEPATH "/tmp/out"

int main() {
  int fd = open(FILEPATH, O_CREAT | O_WRONLY, 0600);
  if (-1 == fd) {
    perror("open() error");
    exit(EXIT_FAILURE);
  }

  char message[] = "Let's Go";
  write(fd, message, sizeof(message));

  close(fd);

  return 0;
}

