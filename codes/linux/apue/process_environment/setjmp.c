#include <setjmp.h>
#include <stdio.h>

static jmp_buf env;

void func4() {
  printf("%s begin\n", __FUNCTION__);
  printf("%s end\n", __FUNCTION__);
}

void func3() {
  printf("%s begin\n", __FUNCTION__);
  printf("%s call func4()\n", __FUNCTION__);
  longjmp(env, 1);
  func4();
  printf("%s is return\n", __FUNCTION__);
  printf("%s end\n", __FUNCTION__);
}

void func2() {
  printf("%s begin\n", __FUNCTION__);
  printf("%s call func3()\n", __FUNCTION__);
  func3();
  printf("%s is return\n", __FUNCTION__);
  printf("%s end\n", __FUNCTION__);
}

void func1() {
  printf("%s begin\n", __FUNCTION__);
  int ret = setjmp(env);
  if (0 == ret) {
    printf("%s call func2()\n", __FUNCTION__);
    func2();
    printf("%s is return\n", __FUNCTION__);
  } else {
    printf("%s(): Jumped back here with code %d\n", __FUNCTION__, ret);
  }
  printf("%s end\n", __FUNCTION__);
}

int main() {
  printf("%s begin\n", __FUNCTION__);
  func1();
  printf("%s end\n", __FUNCTION__);
  return 0;
}
