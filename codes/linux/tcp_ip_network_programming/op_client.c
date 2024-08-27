#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
  if (3 != argc) {
    fprintf(stderr, "Usage: %s <ip> <port>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int clntfd = socket(AF_INET, SOCK_STREAM, 0);
  if (-1 == clntfd) {
    perror("socket() error");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in clnt_addr;
  memset(&clnt_addr, 0, sizeof(clnt_addr));
  clnt_addr.sin_family = AF_INET;
  clnt_addr.sin_addr.s_addr = inet_addr(argv[1]);
  clnt_addr.sin_port = htons(atoi(argv[2]));
  if (-1 == connect(clntfd, (struct sockaddr *)&clnt_addr, sizeof(clnt_addr))) {
    perror("connect() error");
    exit(EXIT_FAILURE);
  } else {
    puts("Connected ......");
  }

  char message[1024] = {0};
  while (1) {
    printf("input nums: ");
    int8_t nums;
    scanf("%hhd", &nums);
    message[0] = (char)nums;

    for (int i = 0; i < nums; ++i) {
      printf("Operand %d: ", i+1);
      scanf("%d", (int *)&message[4 * i + 1]);
    }

    while ('\n' != getchar())
      continue;
    printf("Operator is: ");
    scanf("%c", &message[4 * nums + 1]);

    write(clntfd, message, sizeof(message));

    int result = 0;
    read(clntfd, &result, sizeof(result));
    printf("result is %d\n", result);
  }

  close(clntfd);

  return 0;
}
