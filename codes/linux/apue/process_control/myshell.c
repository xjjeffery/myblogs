#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wait.h>
#include <glob.h>

#define DELIM " \t\n"

void parse(char *line_buf, glob_t *glob_res);

int main(int argc, char *argv[]) {
  char *line_buf = NULL;
  size_t line_size = 0;
  glob_t glob_res;
  while (1) {
    // 命令提示符的输出
    printf("myshell-0.1 $ ");
    // 获取命令的输入
    int res = getline(&line_buf, &line_size, stdin);
    if (-1 == res) {
      perror("getline() error");
      exit(EXIT_FAILURE);
    }

    // 命令解析
    parse(line_buf, &glob_res);

    // 创建子进程
    pid_t pid = fork();
    if (-1 == pid) {
      perror("fork() error");
      exit(EXIT_FAILURE);
    }

    // 使用 exec 执行命令
    if (0 == pid) {
      execvp(glob_res.gl_pathv[0], glob_res.gl_pathv);
      perror("execvp() error");
      exit(EXIT_FAILURE);
    } else {
      wait(NULL);
    }
  }
  globfree(&glob_res);

  return 0;
}

void parse(char *line_buf, glob_t *glob_res) {
  char *tok;
  int i = 0;
  while (1)  {
    tok = strsep(&line_buf, DELIM);
    if (NULL == tok)
      break;

    // 清除空串
    if ('\0' == tok[0])
      continue;

    glob(tok, GLOB_NOCHECK | GLOB_APPEND * i, NULL, glob_res);
    i = 1;
  }
}
