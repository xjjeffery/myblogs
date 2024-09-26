#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

void *thr_func(void *arg) {
  puts("New thread is working");
  sleep(10);
  // pthread_exit(NULL);
}

int main() {
  puts("Begin");
  pthread_t tid;
  void *retval;
  int err = pthread_create(&tid, NULL, thr_func, NULL);
  if (0 != err) {
    fprintf(stderr, "pthread_create() error: %s\n", strerror(err));
    exit(EXIT_FAILURE);
  }

  err = pthread_cancel(tid);
  if (0 != err) {
    fprintf(stderr, "pthread_cancel() error: %s\n", strerror(err));
    exit(EXIT_FAILURE);
  }

  err = pthread_join(tid, &retval);
  if (0 != err) {
    fprintf(stderr, "pthread_join() error: %s\n", strerror(err));
    exit(EXIT_FAILURE);
  }

  if (PTHREAD_CANCELED == retval)
    puts("New thread 被强行终止");
  else
    puts("error");

  puts("End");
  return 0;
}
