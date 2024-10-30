#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <time.h>

#define OPTSTRING "y:mdH:MS"
#define TIMEFORMAT "%Y年 %m月 %d日 %A %T %Z"
#define BUFFERSIZE  4096

int main(int argc, char *argv[]) {
  // 获取当前的整型时间
  time_t time_now = time(NULL);
  // 将时间分解
  struct tm *ltime_now = localtime(&time_now);
  // 创建保存格式化匹配模式的内存
  char format[BUFFERSIZE] = {0};

  if (1 == argc) {
    strncpy(format, TIMEFORMAT, strlen(TIMEFORMAT));
  } else {
    int res = 0;
    while (-1 != (res = getopt(argc, argv, OPTSTRING))) {
      switch (res) {
      case 'y':
        {
          int val = 4;
          if (NULL == optarg)
            val = 4;
          else
            val = atoi(optarg);

          if (2 == val)
            strcat(format, "%y ");
          else if (4 == val)
            strcat(format, "%Y ");
        }
        break;
      case 'm':
        strcat(format, "%m ");
        break;
      case 'd':
        strcat(format, "%d ");
        break;
      case 'H':
        {
          int val = 24;
          if (NULL == optarg)
            val = 24;
          else
            val = atoi(optarg);

          if (24 == val)
            strcat(format, "%H ");
          else if (12 == val)
            strcat(format, "%I ");
        }
        break;
      case 'M':
        strcat(format, "%M ");
        break;
      case 'S':
        strcat(format, "%S ");
        break;
      default:
        fprintf(stderr, "Usage: %s [-Y year] [-M] [-D] [-h hour] [-m] [-s]\n", argv[0]);
        break;
      }
    }
  }

  char date_buf[BUFFERSIZE] = {0};
  int res = strftime(date_buf, BUFFERSIZE, format, ltime_now);
  puts(date_buf);

  return 0;
}
