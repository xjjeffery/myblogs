#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

struct nums {
  int a, b, c, d;
};

void printnums(const char *str, const struct nums *ptr) {
  printf("%s", str);
  printf("  structure at 0x%lx\n", (unsigned long)ptr);
  printf("  ptr.a = %d\n", ptr->a);
  printf("  ptr.b = %d\n", ptr->b);
  printf("  ptr.c = %d\n", ptr->c);
  printf("  ptr.d = %d\n", ptr->d);
}

void *thr_func1(void *arg) {
  struct nums test = {1, 2, 3, 4};
  printnums("thread 1: \n", &test);
  pthread_exit(&test);
}

void *thr_func2(void *arg) {
  printf("thread 2: ID is %lu\n", pthread_self());
  pthread_exit(NULL);
}

int main() {
  pthread_t tid1, tid2;
  struct nums *sptr;

  int err = pthread_create(&tid1, NULL, thr_func1, NULL);
  if (0 != err) {
    fprintf(stderr, "pthread_create() error: %s\n", strerror(err));
    exit(EXIT_FAILURE);
  }

  err = pthread_join(tid1, (void *)&sptr);
  if (0 != err) {
    fprintf(stderr, "pthread_create() error: %s\n", strerror(err));
    exit(EXIT_FAILURE);
  }

  sleep(1);
  printf("parent starting second thread\n");

  err = pthread_create(&tid2, NULL, thr_func2, NULL);
  if (0 != err) {
    fprintf(stderr, "pthread_create() error: %s\n", strerror(err));
    exit(EXIT_FAILURE);
  }

  err = pthread_join(tid2, NULL);
  if (0 != err) {
    fprintf(stderr, "pthread_create() error: %s\n", strerror(err));
    exit(EXIT_FAILURE);
  }

  // sleep(1);
  printnums("parent: \n", sptr);

  return 0;
}
