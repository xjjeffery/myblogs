#include <fcntl.h>
#include <glob.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFERSIZE 4096
#define CURPATH "."
#define APPENDPATH "/*"
#define APPENDHIDEPATH "/.*"

static blkcnt_t mydu(const char *pathname);
static int is_directory(const char *pathname);
bool is_hidedir(const char *pathname);

int main(int argc, const char *argv[]) {
  if (1 == argc) {
    printf("%-6ld %s\n", mydu(CURPATH)/2, CURPATH);
  } else {
    for (int i = 1; i < argc; ++i)
      printf("%-6ld %s\n", mydu(argv[i])/2, argv[i]);
  }

  return 0;
}

static blkcnt_t mydu(const char *pathname) {
  // 首先判断此文件的类型，如果是文件则直接显示，如果是目录则递归分析
  struct stat stat_buf;
  if (-1 == lstat(pathname, &stat_buf)) {
    perror("stat() error");
    return -1;
  }

  // 如果不是目录则返回其块大小
  if (!S_ISDIR(stat_buf.st_mode))
    return stat_buf.st_blocks;

  // 如果是目录，则在文件名后追加 /* 通配符
  char pattern[BUFFERSIZE] = {0};
  strncpy(pattern, pathname, BUFFERSIZE);
  strncat(pattern, APPENDPATH, sizeof(APPENDPATH));
  glob_t glob_buf;
  if (0 != glob(pattern, 0, NULL, &glob_buf)) {
    perror("glob() error");
    exit(EXIT_FAILURE);
  }

  // 递归所有的隐藏文件
  char hide_pattern[BUFFERSIZE];
  strncpy(hide_pattern, pathname, BUFFERSIZE);
  strncat(hide_pattern, APPENDHIDEPATH, sizeof(APPENDHIDEPATH));
  if (glob(hide_pattern, GLOB_APPEND, NULL, &glob_buf) != 0) {
    perror("glob() error");
    exit(EXIT_FAILURE);
  }

  // 首先记录此目录的块大小
  blkcnt_t total_blocks = stat_buf.st_blocks;
  // 递归计算目录种每个文件和子目录的大小
  for (int i = 0; i < glob_buf.gl_pathc; ++i) {
    if (!is_hidedir(glob_buf.gl_pathv[i]))
      total_blocks += mydu(glob_buf.gl_pathv[i]);
  }

  globfree(&glob_buf);
  printf("%-6ld %s\n", total_blocks/2, pathname);
  return total_blocks;
}

bool is_hidedir(const char *pathname) {
  // 取出最后一个 / 后的字符
  char *str = strrchr(pathname, '/');
  if (NULL == str)
    exit(EXIT_FAILURE);

  // 判断文件是否为 . 或 ..
  if (strcmp(str+1, ".") == 0 || strcmp(str+1, "..") == 0)
    return true;
  else
    return false;
}
