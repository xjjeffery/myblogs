#include <stdio.h>
#include <time.h>

int main(int argc, const char *argv[]) {
  time_t time_now = time(NULL);
  struct tm *ltime_now = localtime(&time_now);
  printf("------localtime------\n");
  printf("%d 年 %d 月 %d 日 %d 时 %d 分 %d 秒\n", ltime_now->tm_year + 1900, ltime_now->tm_mon + 1, ltime_now->tm_mday,
        ltime_now->tm_hour, ltime_now->tm_min, ltime_now->tm_sec);
  printf("%d 天 %d 日 %d \n", ltime_now->tm_yday, ltime_now->tm_wday, ltime_now->tm_isdst);

  struct tm *gtime_now = gmtime(&time_now);
  printf("------gtime------\n");
  printf("%d 年 %d 月 %d 日 %d 时 %d 分 %d 秒\n", gtime_now->tm_year + 1900, gtime_now->tm_mon + 1, gtime_now->tm_mday,
        gtime_now->tm_hour, gtime_now->tm_min, gtime_now->tm_sec);
  printf("%d 天 %d 日 %d \n", gtime_now->tm_yday, gtime_now->tm_wday, gtime_now->tm_isdst);

  return 0;
}
