#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define LEFT  30000000
#define RIGHT 30000200
#define THREADNUM 4

sem_t sem;
pthread_mutex_t mutex;

static int num = 0;

static void *thr_func(void *arg) {
  int val = *(int *)arg;
  int isprimer = 1;

  // 本线程继续进行质数的检测
  for (int i = 2; i * i < val; ++i) {
    if (val % i == 0) {
      isprimer = 0;
      break;
    }
  }

  if (isprimer)
    printf("%d is primer\n", val);

  free(arg);
  // 线程检测完以后，信号量加 1
  sem_post(&sem);

  pthread_exit(arg);
}

int main() {
  int err;
  int *ptr;
  pthread_t tids[THREADNUM];
  if (-1 == sem_init(&sem, 0, THREADNUM)) {
    perror("sem_init error");
    exit(EXIT_FAILURE);
  }

  for (int i = LEFT; i <= RIGHT; ++i) {
    sem_wait(&sem); // 创建一个线程，信号量减 1
    ptr = malloc(sizeof(int));
    *ptr = i;
    err = pthread_create(&tids[i % THREADNUM], NULL, thr_func, ptr);
    if (0 != err) {
      free(ptr);
      fprintf(stderr, "pthread_create() error: %s\n", strerror(err));
      break;
    }
  }

  for (int i = 0; i < THREADNUM; ++i)
    pthread_join(tids[i], NULL);

  sem_destroy(&sem);

  return 0;
}
