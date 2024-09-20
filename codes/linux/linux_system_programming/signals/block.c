#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void int_handler(int sig) {
  write(1, "!", STDOUT_FILENO);
}

int main() {
  signal(SIGINT, int_handler);
  sigset_t set, oldset, saveset;
  sigemptyset(&set);  // 初始化信号集
  sigaddset(&set, SIGINT);  // 将 SIGINT 添加到信号集中
  sigprocmask(SIG_UNBLOCK, &set, &saveset);  // 将进程原本的信号屏蔽字保存在 saveset 中
  for (int i = 0; i < 10; ++i) {
    sigprocmask(SIG_BLOCK, &set, &oldset);  // 设置信号屏蔽字，将 SIGINT 屏蔽掉，并保存旧的屏蔽字
    for (int j = 0; j < 5; ++j) {
      write(1, "*", STDOUT_FILENO);
      sleep(1);
    }
    write(1, "\n", STDOUT_FILENO);
    // sigprocmask(SIG_UNBLOCK, &set, NULL);  // 设置信号屏蔽字，将 SIGINT 屏蔽解除
    sigprocmask(SIG_SETMASK, &oldset, NULL);  // 使用 SIG_SETMASK 恢复旧的屏蔽字
  }
  sigprocmask(SIG_BLOCK, &saveset, NULL);  // 退出时还原信号屏蔽字

  return 0;
}
