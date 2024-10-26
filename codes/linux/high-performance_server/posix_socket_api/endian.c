#include <stdio.h>

union {
  short ival;
  char cval[2];
} endian;

int main() {
  endian.ival = 0x1234;
  if (endian.cval[0] == 0x12 && endian.cval[1] == 0x34)
    printf("your compute is big endian\n");
  else
    printf("your compute is little endian\n");

  return 0;
}