#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

int calculate(int *arr, int nums, char op) {
  int result = arr[0];
  // printf("result = %d\n", result);
  switch (op) {
    case '+':
      for(int i = 1; i < nums; ++i) result += arr[i];
    break;
    case '-':
      for(int i = 1; i < nums; ++i) result -= arr[i];
    break;
    case '*':
      for(int i = 1; i < nums; ++i) result *= arr[i];
    break;
    case '/':
      for(int i = 1; i < nums; ++i) result /= arr[i];
    break;
    default:
      printf("Invalide operator\n");
    exit(EXIT_FAILURE);
  }
  return result;
}

int main(int argc, char *argv[]) {
  if (2 != argc) {
    fprintf(stderr, "Usage: %s <port>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (-1 == sockfd) {
    perror("socket() error");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(atoi(argv[1]));
  if (-1 == bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) {
    perror("bind() error");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  if (-1 == listen(sockfd, 5)) {
    perror("listen() error");
    close(sockfd);
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in clnt_addr;
  int addr_len = sizeof(clnt_addr);
  for (int i = 0; i < 5; ++i) {
    int clntfd = accept(sockfd, (struct sockaddr *)&clnt_addr, &addr_len);
    if (-1 == clntfd) {
      perror("accept() error");
      close(sockfd);
      exit(EXIT_FAILURE);
    }

    int8_t nums = 0;
    read(clntfd, &nums, sizeof(nums));
    // printf("nums = %hhd\n", nums);
    char message[1024] = {0};
    for (int i = 0; i < nums; ++i) {
      read(clntfd, &message[i * 4], sizeof(int));
      // printf("%d\n", message[i * 4]);
    }

    char op;
    read(clntfd, &op, sizeof(op));
    printf("op = %c\n", op);

    int result = calculate((int *)message, nums, op);
    write(clntfd, &result, sizeof(result));

    close(clntfd);
  }

  close(sockfd);

  return 0;
}
