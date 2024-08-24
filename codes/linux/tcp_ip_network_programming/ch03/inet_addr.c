#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
  char *addr1 = "1.2.3.4";
  char *addr2 = "1.2.3.256";

  unsigned long conv_addr = inet_addr(addr1);
  if (INADDR_NONE == conv_addr)
    printf("Error occured!\n");
  else
    printf("Network ordered integer addr: %#lx \n", conv_addr);


  conv_addr = inet_addr(addr2);
  if (INADDR_NONE == conv_addr)
    printf("Error occured!\n");
  else
    printf("Network ordered integer addr: %#lx \n", conv_addr);

  return 0;
}
