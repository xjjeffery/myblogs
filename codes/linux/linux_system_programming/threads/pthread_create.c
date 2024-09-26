#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

void printid(const char *s) {
  pid_t pid;
  pthread_t tid;
  pid = getpid();
  tid = pthread_self();
  printf("%s pid %u tid %lu (0x%lx)\n", s, pid, tid, tid);
}

void *thr_func(void *arg) {
  printid("new thread:");
  // return NULL;
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  puts("Begin!");

  pthread_t ntid;
  // 调用失败会返回一个错误码
  int err = pthread_create(&ntid, NULL, thr_func, NULL);
  if (0 != err) {
    fprintf(stderr, "pthread_creat() error: %s", strerror(err));
    exit(EXIT_FAILURE);
  }

  printid("main thread:");
  // sleep(1); // 使主线程挂起，确保新线程有机会运行
  pthread_join(ntid, NULL);
  puts("End!");

  return 0;
}
