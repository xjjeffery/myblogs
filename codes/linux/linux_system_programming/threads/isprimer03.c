#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define LEFT  30000000
#define RIGHT 30000200
#define THREADNUM 4

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static int num = 0;

static void *thr_func(void *arg) {
  int val;
  int isprimer = 1;
  while(1) { // 死循环领取任务
    pthread_mutex_lock(&mutex); // 锁上互斥量，使其他线程阻塞
    while (0 == num) // 还没有下发任务，等待任务的发送
      pthread_cond_wait(&cond, &mutex);

    // 拿到任务，判断任务是否为 -1，若为 -1 则任务结束
    if (-1 == num) {
      // 在结束前释放锁，防止线程死锁
      pthread_mutex_unlock(&mutex);
      break;
    }

    // 拿到任务开始处理任务
    val = num;
    num = 0;
    pthread_cond_broadcast(&cond);  // 通知所有线程任务已取走
    pthread_mutex_unlock(&mutex); // 拿到任务后，解锁互斥量，继续让main线程下发任务
    // 本线程继续进行质数的检测
    isprimer = 1;
    for (int i = 2; i < val/2; ++i) {
      if (val % i == 0) {
        isprimer = 0;
        break;
      }
    }

    if (isprimer)
      printf("[thread-%d] %d is primer\n", *(int *)arg, val);
  }

  pthread_exit(arg);
}

int main() {
  int err;
  int *ptr;
  pthread_t tids[THREADNUM];
  for (int i = 0; i <= THREADNUM; ++i) {
    ptr = malloc(sizeof(int));
    *ptr = i;
    err = pthread_create(tids+i, NULL, thr_func, ptr);
    if (0 != err) {
      fprintf(stderr, "pthread_create() error: %s\n", strerror(err));
      exit(EXIT_FAILURE);
    }
  }

  for (int i = LEFT; i <= RIGHT; ++i) {
    pthread_mutex_lock(&mutex); // 主线程锁上互斥量，下发任务
    while (0 != num) // 任务没有被拿走，等待它们拿走
      pthread_cond_wait(&cond, &mutex);

    num = i;  // 如果没有任务，下发任务
    pthread_cond_signal(&cond); // 唤醒某个线程拿走任务
    pthread_mutex_unlock(&mutex); // 解锁互斥量，让其他线程开始抢占任务
  }

  // 所有的任务下发完成后，检测任务是否被拿走，等待任务被拿走
  pthread_mutex_lock(&mutex);
  while (0 != num)  // 如果最后一个任务没有被拿走，等待它们拿走
    pthread_cond_wait(&cond, &mutex);

  num = -1; // 任务完成
  pthread_cond_broadcast(&cond); // 通知所有线程任务完成
  pthread_mutex_unlock(&mutex);

  void *retval;
  for (int i = 0; i < THREADNUM; ++i) {
    pthread_join(tids[i], &retval);
    free(retval);
  }

  pthread_cond_destroy(&cond);
  pthread_mutex_destroy(&mutex);

  return 0;
}
