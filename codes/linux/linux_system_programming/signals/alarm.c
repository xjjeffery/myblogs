#include <unistd.h>

int main() {
  alarm(5);
  while(1)
    pause();

  return 0;
}