#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void clean_func(void *arg) {
  puts((char *)arg);
}

void *thr_func(void *arg) {
  puts("New thread is working");
  pthread_cleanup_push(clean_func, "func 1");
  pthread_cleanup_push(clean_func, "func 2");
  pthread_cleanup_push(clean_func, "func 3");

  pthread_cleanup_pop(1);
  pthread_cleanup_pop(1);
  pthread_cleanup_pop(1);

  pthread_exit(NULL);
}

int main() {
  puts("Begin");

  pthread_t tid;
  int err = pthread_create(&tid, NULL, thr_func, NULL);
  if (0 != err) {
    fprintf(stderr, "pthread_create() error: %s", strerror(err));
    exit(EXIT_FAILURE);
  }

  pthread_join(tid, NULL);
  puts("End");

  return 0;
}
