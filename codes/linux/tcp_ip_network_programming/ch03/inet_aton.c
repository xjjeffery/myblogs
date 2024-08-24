#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>

void error_handling(const char *msg) {
  fputs(msg, stderr);
  fputc('\n', stderr);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  char *addr = "127.232.124.79";
  struct sockaddr_in addr_inet;

  if (!inet_aton(addr, &addr_inet.sin_addr))
    error_handling("Conversion error");
  else
    printf("Network ordered integer addr: %#x \n", addr_inet.sin_addr.s_addr);

  return 0;
}
