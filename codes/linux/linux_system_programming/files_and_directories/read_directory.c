#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#define BUFFERSIZE 4096

static int is_directory(const char *pathname);
static void read_directory(const char *pathname);

int main(int argc, char *argv[]) {
  if (2 != argc) {
    fprintf(stderr, "Usage: %s <dirname>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // 如果不是目录则直接退出
  if (-1 == is_directory(argv[1])) {
    fprintf(stderr, "%s isn't directory\n", argv[1]);
    exit(EXIT_FAILURE);
  }

  // 开始递归遍历目录
  read_directory(argv[1]);

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

static void read_directory(const char *pathname) {
  // 先打开目录
  DIR *dir = opendir(pathname);
  if (NULL == dir) {
    perror("opendir() error");
    exit(EXIT_FAILURE);
  }

  // 开始读取目录
  struct dirent *rbuf = NULL;
  while (NULL != (rbuf = readdir(dir))) {
    // 记录此时的完整路径
    char path[BUFFERSIZE] = {0};
    strncat(path, pathname, strlen(pathname));
    strcat(path, "/");
    strncat(path, rbuf->d_name, strlen(rbuf->d_name));
    // 如果此路径是目录，开始递归
    if (0 == is_directory(path)) {
      //如果是 . 或 .. 则跳过
      if (0 == strcmp(rbuf->d_name, ".") || 0 == strcmp(rbuf->d_name, ".."))
        continue;

      read_directory(path);
    } else {
      // 不是目录则打印路径
      puts(path);
    }
  }

  closedir(dir);
}
