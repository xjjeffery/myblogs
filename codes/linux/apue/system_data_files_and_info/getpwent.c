#include <pwd.h>
#include <stddef.h>
#include <string.h>

struct passwd* getpwnam(const char *name) {
  struct passwd *ptr;
  setpwent(); // 打开口令文件
  while ((ptr = getpwent()) != NULL)  // 逐一获取口令文件中的每一行并比较
    if (strcmp(name, ptr->pw_name) == 0)
      break;

  endpwent();   // 关闭口令文件
  return(ptr);  // 返回用户名
}
