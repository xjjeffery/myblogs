#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#define LEFT  30000000
#define RIGHT 30000200

int main() {
  for (int i = LEFT; i <= RIGHT; ++i) {
    bool is_primer = true;
    for (int j = 2; j <= sqrt(i); ++j) {
      if (0 == i % j) {
        is_primer = false;
        break;
      }
    }

    if (is_primer)
      printf("%d is primer\n", i);
  }

  return 0;
}
