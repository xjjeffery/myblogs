#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <getopt.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

#define OPSTRING "ailn"
#define BUFFERSIZE 4096

static int opt_buf[5] = {0};
struct options {
  int show_all;
  int show_inode;
  int long_format;
  int numeric;
};

static void list_files();
static void myls(int argc, const char **argv);
static void list_directory(const char *pathname, struct options *op);
static void display_file_info(const char *pathname, struct stat *st, struct options *op);
static char *file_type(const struct stat *st);

int main(int argc, char *argv[]) {
  if (1 == argc)
    list_files();
  else
    myls(argc, argv);

  return 0;
}

static void list_files() {
  DIR *dir = opendir(".");
  if (NULL == dir) {
    perror("opendir() error");
    exit(EXIT_FAILURE);
  }

  struct dirent *dir_buf;
  while (NULL != (dir_buf = readdir(dir))) {
    if (0 == strcmp(dir_buf->d_name, ".") || 0 == strcmp(dir_buf->d_name, "."))
      continue;

    if (dir_buf->d_name[0] == '.')
      continue;

    puts(dir_buf->d_name);
  }
}

static char *file_type(const struct stat *st) {
  char *file_type = malloc(11);
  memcpy(file_type, "----------", 11);
  if (S_ISDIR(st->st_mode)) {
    file_type[0] = 'd';
  } else if (S_ISLNK(st->st_mode)) {
    file_type[0] = 'l';
  } else if (S_ISREG(st->st_mode)) {
    file_type[0] = '-';
  } else if (S_ISBLK(st->st_mode)) {
    file_type[0] = 'b';
  } else if (S_ISFIFO(st->st_mode)) {
    file_type[0] = 'p';
  } else if (S_ISSOCK(st->st_mode)) {
    file_type[0] = 's';
  } else if (S_ISCHR(st->st_mode)) {
    file_type[0] = 'c';
  }

  if (st->st_mode & S_IRUSR)
    file_type[1] = 'r';
  if (st->st_mode & S_IWUSR)
    file_type[2] = 'w';
  if (st->st_mode & S_IXUSR)
    file_type[3] = 'x';
  else if (st->st_mode & S_ISUID)
    file_type[3] = 's';

  if (st->st_mode & S_IRGRP)
    file_type[4] = 'r';
  if (st->st_mode & S_IWGRP)
    file_type[5] = 'w';
  if (st->st_mode & S_IXGRP)
    file_type[6] = 'x';
  else if (st->st_mode & S_ISGID)
    file_type[6] = 's';

  if (st->st_mode & S_IROTH)
    file_type[7] = 'r';
  if (st->st_mode & S_IWOTH)
    file_type[8] = 'w';
  if (st->st_mode & S_IXOTH)
    file_type[9] = 'x';
  else if (st->st_mode & S_ISVTX)
    file_type[9] = 't';

  return file_type;
}

static void display_file_info(const char *pathname, struct stat *st, struct options *op) {
  if (op->show_inode)
    printf("%lu  ", st->st_ino);

  if (op->long_format) {
    printf("%s  ", file_type(st));

    printf("%lu  ", st->st_nlink);
    if (op->numeric)
      printf("%u  %u  ", st->st_uid, st->st_gid);
    else
      printf("%s  %s  ", getpwuid(st->st_uid)->pw_name, getgrgid(st->st_gid)->gr_name);

    printf("%6lu  ", st->st_size);

    struct tm *ltime = localtime(&st->st_mtim.tv_sec);
    char tm_buf[BUFFERSIZE] = {0};
    int res = strftime(tm_buf, BUFFERSIZE, "%m月  %d日  %T", ltime);
    printf("%-10s ", tm_buf);
  }

  printf("%s\n", pathname);
}

static void list_directory(const char *pathname, struct options *op) {
  struct stat stat_buf;
  if (-1 == lstat(pathname, &stat_buf)) {
    perror("stat() error");
    exit(EXIT_FAILURE);
  }

  if (!S_ISDIR(stat_buf.st_mode)) {
    display_file_info(pathname, &stat_buf, op);
  } else {
    printf("%s:\n", pathname);
    DIR *dir = opendir(pathname);
    if (NULL == dir) {
      perror("opendir() error");
      exit(EXIT_FAILURE);
    }

    struct dirent *dir_buf = NULL;
    while (NULL != (dir_buf = readdir(dir))) {
      if (!op->show_all && dir_buf->d_name[0] == '.')
        continue;

      char path[BUFFERSIZE] = {0};
      strncat(path, pathname, strlen(pathname));
      strcat(path, "/");
      strncat(path, dir_buf->d_name, strlen(dir_buf->d_name));
      struct stat file_stat;
      if (-1 == lstat(path, &file_stat)) {
        perror("lstat() error");
        exit(EXIT_FAILURE);
      }
      display_file_info(dir_buf->d_name, &file_stat, op);
    }
  }
}

static void myls(int argc, const char **argv) {
  struct options op = {0, 0, 0, 0};
  int res = 0;
  while (-1 != (res = getopt(argc, argv, OPSTRING))) {
    switch (res) {
      case 'a':
      op.show_all = 1;
      break;
      case 'i':
      op.show_inode = 1;
      break;
      case 'l':
      op.long_format = 1;
      break;
      case 'n':
      op.numeric = 1;
      break;
      default:
        fprintf(stderr, "Usage: %s [-ailn] [pathanme]\n", argv[0]);
      exit(EXIT_FAILURE);
    }
  }

  if (optind > argc)
    list_directory(".", &op);

  // 非选项参数都会置于选项参数末
  while (optind < argc){
    list_directory(argv[optind++], &op);
    printf("\n");
  }
}

