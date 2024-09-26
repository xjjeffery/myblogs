#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define LEFT  30000000
#define RIGHT 30000200
#define THREADNUM (RIGHT - LEFT + 1)

static void *thr_func(void *arg) {
  int val = *(int *)arg;
  int isprimer = 1;
  for (int i = 2; i < val/2; ++i) {
    if (val % i == 0) {
      isprimer = 0;
      break;
    }
  }

  if (isprimer)
    printf("%d is primer\n", val);

  pthread_exit(arg);
}

int main() {
  int err;
  pthread_t tid[THREADNUM];
  int *ptr;
  for (int i = LEFT; i <= RIGHT; ++i) {
    ptr = malloc(sizeof(int));
    *ptr = i;
    err = pthread_create(tid+(i-LEFT), NULL, thr_func, ptr);
    if (0 != err) {
      fprintf(stderr, "pthread_create() error: %s\n", strerror(err));
      break;
    }
  }

  void *ret;
  for (int i = 0; i < THREADNUM; ++i) {
    pthread_join(tid[i], &ret);
    free(ret);
  }

  return 0;
}
