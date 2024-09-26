#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t mutex_a = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_b = PTHREAD_MUTEX_INITIALIZER;

// 线程函数 A
void *thr_funca(void *arg) {
  printf("thread A waiting get ResourceA \n");
  pthread_mutex_lock(&mutex_a);
  printf("thread A got ResourceA \n");

  sleep(1);
  printf("thread A waiting get ResourceB \n");
  pthread_mutex_lock(&mutex_b);
  printf("thread A got ResourceB \n");

  pthread_mutex_unlock(&mutex_b);
  pthread_mutex_unlock(&mutex_a) ;

  return NULL;
}

// 线程函数 B
void *thr_funcb(void *arg) {
  printf("thread B waiting get ResourceB \n");
  pthread_mutex_lock(&mutex_b);
  printf("thread B got ResourceB \n");

  sleep(1);
  printf("thread B waiting get ResourceA \n");
  pthread_mutex_lock(&mutex_a) ;
  printf("thread B got ResourceA \n");

  pthread_mutex_unlock(&mutex_a);
  pthread_mutex_unlock(&mutex_b) ;

  return NULL;
}

int main() {
  pthread_t tida, tidb;
  int err = pthread_create(&tida, NULL, thr_funca, NULL);
  if (err) {
    fprintf(stderr, "pthread_create error: %s\n", strerror(err));
    exit(EXIT_FAILURE);
  }

  err = pthread_create(&tidb, NULL, thr_funcb, NULL);
  if (err) {
    fprintf(stderr, "pthread_create error: %s\n", strerror(err));
    pthread_join(tida, NULL);
    exit(EXIT_FAILURE);
  }

  pthread_join(tida, NULL);
  pthread_join(tidb, NULL);

  pthread_mutex_destroy(&mutex_a);
  pthread_mutex_destroy(&mutex_b);

  return 0;
}
