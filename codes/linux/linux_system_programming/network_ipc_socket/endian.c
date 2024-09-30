#include <stdio.h>

// 使用联合体保存数据，使用同样大小的 char 数据按字节读取
union {
  short sval;
  char sendian[sizeof(short)];
} sed;

union {
  int ival;
  char iendian[sizeof(int)];
} ied;

int main() {
  sed.sval = 0x1122;
  if (sed.sendian[0] == 0x11) {
    puts("host is big endian");
  } else if (sed.sendian[0] == 0x22) {
    puts("host is little endian");
  }

  printf("sval = %#x, sed.sendian[0] = %#x, sed.sendian[1] = %#x\n", sed.sval, sed.sendian[0], sed.sendian[1]);
  
  ied.ival = 0x11223344;
  printf("ival = %#x, ied.iendian[0] = %#x, ied.iendian[1] = %#x, ied.iendian[2] = %#x, ied.iendian[3] = %#x\n", 
          ied.ival, ied.iendian[0], ied.iendian[1], ied.iendian[2], ied.iendian[3]);
  return 0;
}