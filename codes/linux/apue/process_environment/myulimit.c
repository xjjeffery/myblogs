#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <unistd.h>

#define OPTSIZE 1024

static bool is_hard = false;
static void pr_slimit(const char *name, int resource);
static void pr_hlimit(const char *name, int resource);

// 使用 ISO C 的字符串创建宏，以便为每个资源名产生字符串值
#define PRSLIMIT(resource)          \
  do {                              \
    pr_slimit(#resource, resource); \
  } while (0)

#define PRHLIMIT(resource)          \
  do {                              \
    pr_hlimit(#resource, resource); \
  } while (0)

#define PRLIMIT(resource) \
  do {                    \
    if (is_hard) {        \
      PRHLIMIT(resource); \
    } else {              \
      PRSLIMIT(resource); \
    }                     \
  } while (0)

static void show_all_limits();
static void show_limit(int ret);
static void check_options(int argc, char **argv, int ret);

int main(int argc, const char *argv[]) {
  // 有命令行参数选项就要进行参数选项判断
  if (argc < 2) {
    fprintf(stderr, "Usage: %s -[acdefHilmnpqrsStuvx]...\n", argv[0]);
    exit(1);
  }

  int opt_ret = 0;
  // 获取参数选项，如果参数选项获取完成则返回 -1，否则返回参数选项的字符
  // ulimit 最多只能接受两个参数选项，如果有两个参数选项第一个必须是 H 或 S
  // 判断第一个参数选项是什么，如果不是 H 或 S，则不能有更多的参数选项
  // 如果是 H 和 S，后面只能再跟一个参数选项
  if ((opt_ret = getopt(argc, argv, "acdefHilmnpqrsStuvx")) != -1) {
    if (opt_ret == 'H') {
      is_hard = true;
      opt_ret = getopt(argc, argv, "acdefHilmnpqrsStuvx");
    } else if (opt_ret == 'S') {
      is_hard = false;
      opt_ret = getopt(argc, argv, "acdefHilmnpqrsStuvx");
    }

    check_options(argc, argv, opt_ret);
  }

  return 0;
}

static void pr_slimit(const char *name, int resource) {
  int kbytes = 1;
  if (resource == RLIMIT_DATA || resource == RLIMIT_MEMLOCK || resource == RLIMIT_STACK || resource == RLIMIT_AS)
    kbytes = 1024;

  struct rlimit rlim;
  if (getrlimit(resource, &rlim) == 0) {
    if (rlim.rlim_cur != RLIM_INFINITY) {
      printf("%ld\n", rlim.rlim_cur / kbytes);
    } else {
      printf("unlimited\n");
    }
  } else {
    fprintf(stderr, "get %s limit error %s", name, strerror(errno));
  }
}

static void pr_hlimit(const char *name, int resource) {
  int kbytes = 1;
  if (resource == RLIMIT_DATA || resource == RLIMIT_MEMLOCK || resource == RLIMIT_STACK || resource == RLIMIT_AS)
    kbytes = 1024;

  struct rlimit rlim;
  if (getrlimit(resource, &rlim) == 0) {
    if (rlim.rlim_max != RLIM_INFINITY) {
      printf("%ld\n", rlim.rlim_max / kbytes);
    } else {
      printf("unlimited\n");
    }
  } else {
    fprintf(stderr, "get %s limit error %s", name, strerror(errno));
  }
}

static void show_all_limits() {
  printf("%-22s", "core file size ");
  printf("%16s", "(blocks, -c) ");
  PRLIMIT(RLIMIT_CORE);
  printf("%-22s", "data seg size");
  printf("%16s", "(kbytes, -d) ");
  PRLIMIT(RLIMIT_DATA);
  printf("%-22s", "scheduling priority");
  printf("%16s", "(-e) ");
  PRLIMIT(RLIMIT_RTTIME);
  printf("%-22s", "file size ");
  printf("%16s", "(blocks, -f) ");
  PRLIMIT(RLIMIT_FSIZE);
  printf("%-22s", "pending signals ");
  printf("%16s", "(-i) ");
  PRLIMIT(RLIMIT_SIGPENDING);
  printf("%-22s", "max locked memory ");
  printf("%16s", "(kbytes, -l) ");
  PRLIMIT(RLIMIT_MEMLOCK);
  printf("%-22s", "open files ");
  printf("%16s", "(-n) ");
  PRLIMIT(RLIMIT_NOFILE);
  printf("%-22s", "POSIX message queues ");
  printf("%16s", "(bytes, -q) ");
  PRLIMIT(RLIMIT_MSGQUEUE);
  printf("%-22s", "real-time priority ");
  printf("%16s", "(-r) ");
  PRLIMIT(RLIMIT_RTPRIO);
  printf("%-22s", "stack size ");
  printf("%16s", "(kbytes, -s) ");
  PRLIMIT(RLIMIT_STACK);
  printf("%-22s", "cpu time ");
  printf("%16s", "(seconds, -t) ");
  PRLIMIT(RLIMIT_CPU);
  printf("%-22s", "max user processes ");
  printf("%16s", "(-u) ");
  PRLIMIT(RLIMIT_NPROC);
  printf("%-22s", "virtual memory ");
  printf("%16s", "(kbytes, -v) ");
  PRLIMIT(RLIMIT_AS);
  printf("%-22s", "file locks ");
  printf("%16s", "(-x) ");
  PRLIMIT(RLIMIT_LOCKS);
}

static void show_limit(int ret) {
  switch (ret) {
  case 'a':
    show_all_limits();
    break;
  case 'c':
    PRLIMIT(RLIMIT_CORE);
    break;
  case 'd':
    PRLIMIT(RLIMIT_DATA);
    break;
  case 'e':
    PRLIMIT(RLIMIT_RTTIME);
    break;
  case 'f':
    PRLIMIT(RLIMIT_FSIZE);
    break;
  case 'i':
    PRLIMIT(RLIMIT_SIGPENDING);
    break;
  case 'l':
    PRLIMIT(RLIMIT_MEMLOCK);
    break;
  case 'n':
    PRLIMIT(RLIMIT_NOFILE);
    break;
  case 'q':
    PRLIMIT(RLIMIT_MSGQUEUE);
    break;
  case 'r':
    PRLIMIT(RLIMIT_RTPRIO);
    break;
  case 's':
    PRLIMIT(RLIMIT_STACK);
    break;
  case 't':
    PRLIMIT(RLIMIT_CPU);
    break;
  case 'u':
    PRLIMIT(RLIMIT_NPROC);
    break;
  case 'v':
    PRLIMIT(RLIMIT_AS);
    break;
  case 'x':
    PRLIMIT(RLIMIT_LOCKS);
    break;
  default:
    break;
  }
}

static void check_options(int argc, char **argv, int ret) {
  char str[OPTSIZE] = {0};
  int i = 0;
  // 继续读取参数选项，如果存在则保存
  int opt_ret;
  while ((opt_ret = getopt(argc, argv, "acdefHilmnpqrsStuvx")) != -1) {
    str[i++] = opt_ret;
  }

  // 如果还有更多的参数选项直接报错，否则显示需要查看的参数选项
  if (strlen(str) != 0) {
    fprintf(stderr, "ulimit: %s: invalid number\n", str);
  } else {
    show_limit(ret);
  }
}
