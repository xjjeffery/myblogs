#ifndef __PROTO_H__
#define __PROTO_H__

#include <stdint.h>

#define RCVPORT "1989"
#define NAMESIZE 13     // 为了测试数据对齐的问题，这里选择一个一定不对齐的数字

struct msg_st {
  uint8_t name[NAMESIZE];
  uint32_t math;
  uint32_t chinese;
} __attribute__((packed));  // 告诉 gcc 编译器,不要对齐

#endif