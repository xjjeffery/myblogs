#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define THREADNUM 4

static int num = 0;
sem_t sem[THREADNUM];

int next(int val) {
  return (val + 1) == THREADNUM ? 0 : (val + 1);
}

void *thr_func(void *arg) {
  int val = *(int *)arg;
  int ch = 'a' + val;
  while(1) {
    // 拿到锁以后，加锁
    sem_wait(sem+val);
    write(1, &ch, STDOUT_FILENO);
    num = next(val);
    sem_post(sem+num);
  }

  pthread_exit(NULL);
}

int main() {
  for (int i = 0; i < THREADNUM; ++i) {
    if (-1 == sem_init(sem+i, 0, 1)) {
      perror("sem_init error");
      break;
    }
  }

  pthread_t tids[THREADNUM];
  int err;
  int *ptr;
  for (int i = 0; i < THREADNUM; ++i) {
    sem_wait(sem+i);
    ptr = malloc(sizeof(int));
    *ptr = i;
    err = pthread_create(tids+i, NULL, thr_func, ptr);
    if (err) {
      fprintf(stderr, "pthread_create() error: %s\n", strerror(err));
      break;
    }
  }

  sem_post(sem+0);
  alarm(1);
  for (int i = 0; i < THREADNUM; ++i) {
    pthread_join(tids[i], NULL);
    sem_destroy(sem+i);
  }

  return 0;
}
