#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define THREADNUM 4

pthread_mutex_t mutexs[THREADNUM];

int next(int val) {
  return (val + 1) == THREADNUM ? 0 : (val + 1);
}

void *thr_func(void *arg) {
  int val = *(int *)arg;
  int ch = 'a' + val;
  while(1) {
    // 拿到锁以后，加锁
    pthread_mutex_lock(mutexs+val);
    write(1, &ch, STDOUT_FILENO);
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
    pthread_mutex_init(mutexs+i, NULL); // 使用默认属性初始化互斥量
    pthread_mutex_lock(mutexs+i); // 在线程启动之前就将每个对应的互斥量锁住，所有线程都会阻塞
    ptr = malloc(sizeof(int));
    *ptr = i;
    err = pthread_create(tids+i, NULL, thr_func, ptr);
    if (err) {
      fprintf(stderr, "pthread_create() error: %s\n", strerror(err));
      break;
    }
  }

  // 4 个线程创建完成以后首先释放 0 号线程的互斥量，0 号线程开始运行
  pthread_mutex_unlock(mutexs+0);

  alarm(1);
  for (int i = 0; i < THREADNUM; ++i) {
    pthread_join(tids[i], NULL);
    pthread_mutex_destroy(mutexs+i);
  }

  return 0;
}
