#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#define BUFFERSIZE 4096
#define CURPATH "."

static blkcnt_t mydu(const char *pathname);
static int is_directory(const char *pathname);

int main(int argc, const char *argv[]) {
  if (1 == argc) {
    printf("%-6ld %s\n", mydu(CURPATH)/2, CURPATH);
  } else {
    for (int i = 1; i < argc; ++i) {
      if (!is_directory(argv[i])) {
        mydu(argv[i]);
      } else {
        printf("%-6ld %s\n", mydu(argv[i])/2, argv[i]);
      }
    }
  }

  return 0;
}

static int is_directory(const char *pathname) {
  struct stat stat_buf;
  if (-1 == stat(pathname, &stat_buf)) {
    perror("stat() error");
    return -1;
  }

  if (S_ISDIR(stat_buf.st_mode))
    return 0;
  else
    return -1;
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

  // 如果是目录则递归读取目录
  DIR *dir = opendir(pathname);
  if (NULL == dir) {
    perror("opendir error");
    exit(EXIT_FAILURE);
  }

  // 首先记录此目录的块大小
  blkcnt_t total_blocks = stat_buf.st_blocks;
  char path[BUFFERSIZE] = {0};
  // 递归计算目录中每个文件和子目录的大小
  struct dirent *dir_buf = NULL;
  while (NULL != (dir_buf = readdir(dir))) {
    memset(path, 0, BUFFERSIZE);
    if (0 == strcmp(dir_buf->d_name, ".") || 0 == strcmp(dir_buf->d_name, ".."))
      continue;

    // 路径拼接，开始递归
    strncpy(path, pathname, BUFFERSIZE);
    strcat(path, "/");
    strncat(path, dir_buf->d_name, strlen(dir_buf->d_name));
    total_blocks += mydu(path);
  }

  closedir(dir);
  printf("%-6ld %s\n", total_blocks/2, pathname);
  return total_blocks;
}