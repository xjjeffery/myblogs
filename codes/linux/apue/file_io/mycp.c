#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFERSIZE 1024

int main(int argc, char *argv[]) {
  if (3 != argc) {
    fprintf(stderr, "Usage: %s <source> <dest>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int sfd = open(argv[1], O_RDONLY);
  if (-1 == sfd) {
    perror("open() error");
    exit(EXIT_FAILURE);
  }

  int dfd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
  if (-1 == dfd) {
    perror("open() error");
    close(sfd);
    exit(EXIT_FAILURE);
  }

  int rlen = 0, wlen = 0;
  char buf[BUFFERSIZE] = {0};
  int index = 0;
  while (rlen = read(sfd, buf+index, BUFFERSIZE)) {
    if (0 > rlen) {
      perror("read() error");
      break;
    }

    if (0 == rlen)
      printf("No Data\n");

    while ((wlen = write(dfd, buf, rlen)) < rlen) {
      index += wlen;
      rlen -= wlen;
    }
  }

  close(dfd);
  close(sfd);

  return 0;
}
