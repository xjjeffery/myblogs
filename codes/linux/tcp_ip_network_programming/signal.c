#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void keycontrol(int sig) {
  if (SIGINT == sig)
    puts("Ctrl+C pressed!");
}

void timeout(int sig) {
  if (SIGALRM == sig)
    puts("Time out!");
  alarm(2);
}

int main(int argc, char *argv[]) {
  signal(SIGINT, keycontrol);
  signal(SIGALRM, timeout);
  alarm(2);

  for (int i = 0; i < 3; ++i) {
    puts("wait ...");
    sleep(20);
  }

  return 0;
}
