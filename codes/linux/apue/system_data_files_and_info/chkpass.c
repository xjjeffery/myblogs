#include <crypt.h>
#include <shadow.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  if (2 != argc) {
    fprintf(stderr, "Usage: %s <username>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  struct spwd *shadow_line = NULL;
  shadow_line = getspnam(argv[1]);

  char *input_pass = getpass("PassWord: ");
  char *crypt_pass = crypt(input_pass, shadow_line->sp_pwdp);
  if (0 == strcmp(shadow_line->sp_pwdp, crypt_pass))
    puts("ok!");
  else
    puts("failed");

  return 0;
}
