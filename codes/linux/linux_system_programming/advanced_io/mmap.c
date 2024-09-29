#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <wait.h>

#define PATHNAME "/tmp/out"
#define BUFFERSIZE 1024

int main() {
  int fd = open(PATHNAME, O_RDWR | O_CREAT, 0600);
  if (-1 == fd) {
    perror("open() error");
    exit(EXIT_FAILURE);
  }

  off_t len = lseek(fd, 0, SEEK_END);
  void *addr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, fd, 0);
  if (MAP_FAILED == addr) {
    perror("mmap() error");
    exit(EXIT_FAILURE);
  }
  close(fd);

  pid_t pid = fork();
  if (-1 == pid) {
    munmap(addr, len);
    perror("fork() error");
    exit(EXIT_FAILURE);
  }

  if (0 == pid) {
    memcpy(addr, "hello world", strlen("hello world"));
  } else {
    printf("read message: %s\n", (char *)addr);
    wait(NULL);
  }

  munmap(addr, len);
  return 0;
}