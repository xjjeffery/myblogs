#include <time.h>
#include <stdio.h>

#define BUFFERSIZE 4096

int main() {
  // 获取整型的时间值
  time_t time_now = time(NULL);
  // 对整型值进行分解
  struct tm *ltime_now = localtime(&time_now);
  // 格式化转换时间
  char buf[BUFFERSIZE] = {0};
  int res = strftime(buf, BUFFERSIZE, "%G %m %d %A %T %Z", ltime_now);
  if (0 == res)
    puts("failed");
  else
    puts(buf);

  return 0;
}
