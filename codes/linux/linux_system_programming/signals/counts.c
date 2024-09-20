#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#if 0
int main() {
  int64_t count = 0;
  time_t end_time = time(NULL) + 5;
  while(time(NULL) <= end_time)
    count++;

  printf("%ld\n", count);

  return 0;
}
#else
static volatile int loop = 1;

void alarm_handler(int sig) {
  loop = 0;
}

int main() {
  int64_t count = 0;
  signal(SIGALRM, alarm_handler);
  alarm(5);
  while(loop)
    count++;

  printf("%ld\n", count);

  return 0;
}

#endif