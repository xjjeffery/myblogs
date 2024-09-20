#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

#define BUFFERSIZE 1024
#define CSP 10
#define MAX_BURST 100

// static volatile int token = 1; // 让编译优化后读取的数据也从实际内存中获取
static volatile sig_atomic_t token = 1;

void alarm_handler(int sig) {
  token++;
  if (MAX_BURST <= token)
    token = MAX_BURST;
  alarm(1);
}

int main(int argc, char *argv[]) {
  if (2 != argc) {
    fprintf(stderr, "Usage: %s <dest>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  int fd = 0;
  do {
    fd = open(argv[1], O_RDONLY);
    if (-1 == fd) {
      if (EINTR != errno) { // 信号中断处理
        perror("open() error");
        exit(EXIT_FAILURE);
      }
    }
  } while(-1 == fd);

  // 信号注册并启动 alarm
  signal(SIGALRM, alarm_handler);
  alarm(1);

  while (1) {
    while (0 >= token)  // 在没有 token 的时候等待信号响应累计 token
      pause();
    token--;  // 递减操作的机器指令不是一条指令，不原子，需将其声明为原子类型

    int rlen = 0, wlen = 0;
    char buf[BUFFERSIZE] = {0};
    int index = 0;

    do {
      rlen = read(fd, buf+index, CSP);
      if (0 > rlen) {
        if (EINTR != errno) { // 阻塞的系统调用要进行信号中断处理
          perror("read() error");
          close(fd);
          break;
        } else {
          continue;
        }
      }
    } while (rlen < 0);

    if (0 == rlen) {
      printf("No Data\n");
      break;
    }

    while ((wlen = write(STDOUT_FILENO, buf+index, rlen)) < rlen) {
      if (0 > wlen) {
        if (EINTR == errno) {
          continue;
        } else {
          perror("write() error");
          close(fd);
          exit(EXIT_FAILURE);
        }
      }
      index += wlen;
      rlen -= wlen;
    }
  }

  close(fd);

  return 0;
}
