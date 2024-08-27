#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>

int main(int argc, char *argv[]) {
  if (2 != argc) {
    fprintf(stderr, "Usage: %s <hostname>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  struct hostent *host = gethostbyname(argv[1]);
  if (!host) {
    fprintf(stderr, "gethostbyname() error\n");
    exit(EXIT_FAILURE);
  }

  printf("Official name: %ss \n", host->name);
  for (int i = 0; host->h_aliases[i]; ++i)
    printf("Aliasers %d: %s\n", i+1, host->h_aliases[i]);

  printf("Addresss type: %s\n", (host->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET6");
  for (int i = 0; host->h_addr_list[i]; ++i)
    printf("IP addr %d: %s\n", i+1, inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));

  return 0;
}
