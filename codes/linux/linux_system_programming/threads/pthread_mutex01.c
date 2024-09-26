#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREADNUM 20
#define PATHNAME "/tmp/out"
#define BUFFERSIZE 512

// 创建互斥锁
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *thr_func(void *arg) {
  FILE *fp = fopen(PATHNAME, "r+");
  if (NULL == fp) {
    perror("fopen() error");
    exit(EXIT_FAILURE);
  }

  char buf[BUFFERSIZE] = {0};
  // 从临界区开始处进行加锁
  pthread_mutex_lock(&mutex);
  fgets(buf, BUFFERSIZE, fp);
  fseek(fp, 0, SEEK_SET);
  fprintf(fp, "%d\n", atoi(buf)+1);
  fclose(fp);
  // 退出临界区时解锁
  pthread_mutex_unlock(&mutex);

  pthread_exit(NULL);
}

int main() {
  // 使用数组所有线程 ID 的信息，用于后续的回收
  pthread_t tids[THREADNUM];
  int err;
  for (int i = 0; i < THREADNUM; ++i) {
    err = pthread_create(tids+i, NULL, thr_func, NULL);
    if (err) {
      fprintf(stderr, "pthread_create() error: %s\n", strerror(err));
      break;
    }
  }

  for (int i = 0; i < THREADNUM; ++i)
    pthread_join(tids[i], NULL);

  // 释放互斥锁
  pthread_mutex_destroy(&mutex);

  return 0;
}
