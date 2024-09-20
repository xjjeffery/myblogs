#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void signal_handler(int sig) {
  write(1, "!", STDOUT_FILENO);
}

int main() {
  // 只有下面的这些指令，在程序执行过程中，一旦按下 Ctrl+C 程序就终止
  // 如果想要忽略 Ctrl+C 的信号，可以使用 SIG_IGN 忽略或自定义一个信号，打印特殊字符处理
  // 1. 忽略信号
  // signal(SIGINT, SIG_IGN);
  // 2. 打印特殊字符
  signal(SIGINT, signal_handler);

  for (int i = 0; i < 10; ++i) {
    write(1, "*", STDOUT_FILENO);
    sleep(1);
  }

  return 0;
}
