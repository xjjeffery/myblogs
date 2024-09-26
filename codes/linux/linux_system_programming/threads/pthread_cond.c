#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define THREADNUM 4

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static int num = 0;

int next(int val) {
  return (val + 1) == THREADNUM ? 0 : (val + 1);
}

void *thr_func(void *arg) {
  int val = *(int *)arg;
  int ch = 'a' + val;
  while(1) {
    // 拿到锁以后，加锁
    pthread_mutex_lock(mutexs+val);
    while (num != val) // 如果不是自己的编号则阻塞等待
      pthread_cond_wait(&cond, &wait);
    write(1, &ch, STDOUT_FILENO);
    num = next(val);
    pthread_cond_signal(&cond); // 唤醒下一个线程
    // 将下一个线程的互斥量解锁，使下一个线程开始运行，本线程阻塞
    pthread_mutex_unlock(mutexs+next(val));
  }

  pthread_exit(NULL);
}

int main() {
  pthread_t tids[THREADNUM];
  int err;
  int *ptr;
  for (int i = 0; i < THREADNUM; ++i) {
    ptr = malloc(sizeof(int));
    *ptr = i;
    err = pthread_create(tids+i, NULL, thr_func, ptr);
    if (err) {
      fprintf(stderr, "pthread_create() error: %s\n", strerror(err));
      break;
    }
  }

  alarm(1);
  for (int i = 0; i < THREADNUM; ++i)
    pthread_join(tids[i], NULL);

  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cond);

  return 0;
}
