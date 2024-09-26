#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define THREADNUM 8

static int num = 0;

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

void *thr_funcw(void *arg) {
  int val = *(int *)arg;

  while (1) {
    pthread_rwlock_wrlock(&rwlock);
    num++;
    printf("[w-%d]: write %d\n", val, num);
    pthread_rwlock_unlock(&rwlock);
  }

  pthread_exit(NULL);
}

void *thr_funcr(void *arg) {
  int val = *(int *)arg;

  while (1) {
    pthread_rwlock_rdlock(&rwlock);
    printf("[r-%d]: read %d\n", val, num);
    pthread_rwlock_unlock(&rwlock);
    usleep(200);
  }

  pthread_exit(NULL);
}

int main() {
  pthread_t tids[THREADNUM];
  int i = 0;
  int err;
  int arr[THREADNUM] = {0};
  for (; i < 3; ++i) {
    arr[i] = i;
    err = pthread_create(tids+i, NULL, thr_funcw, arr+i);
    if (err) {
      fprintf(stderr, "pthread_create error: %s\n", strerror(err));
      break;
    }
  }

  for (; i < THREADNUM; ++i) {
    arr[i] = i;
    err = pthread_create(tids+i, NULL, thr_funcr, arr+i);
    if (err) {
      fprintf(stderr, "pthread_create error: %s\n", strerror(err));
      break;
    }
  }

  alarm(1);
  for (i = 0; i < THREADNUM; ++i)
    pthread_join(tids[i], NULL);

  pthread_rwlock_destroy(&rwlock);

  return 0;
}
