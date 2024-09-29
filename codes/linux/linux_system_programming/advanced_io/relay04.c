#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/epoll.h>

#define TTY1 "/dev/tty10"
#define TTY2 "/dev/tty11"

#define BUFFERSIZE 4096

enum {
  STATE_R = 1,
  STATE_W,
  STATE_A,
  STATE_E,
  STATE_T
};

struct fsm {
  int sfd;
  int dfd;
  int state;
  char buffer[BUFFERSIZE];
  int rlen;
  int index;
  char *strerr;
};

void relay(int fd1, int fd2);
void fsm_driver(struct fsm *fsm);
void fsm_read(struct fsm *fsm);
void fsm_write(struct fsm *fsm);
int max(int fd1, int fd2);

int main(int agrc, char *argv[]) {
  // 1. 分别打开两个文件描述符
  int fd1, fd2;
  fd1 = open(TTY1, O_RDWR); // 以阻塞的方式打开
  if (-1 == fd1) {
    perror("open() error");
    exit(EXIT_FAILURE);
  }

  write(fd1, "TTY1\n", 5);

  fd2 = open(TTY2, O_RDWR | O_NONBLOCK); // 以非阻塞的方式打开
  if (-1 == fd2) {
    perror("open() error");
    close(fd1);
    exit(EXIT_FAILURE);
  }

  write(fd2, "TTY2\n", 5);

  relay(fd1, fd2);

  close(fd1);
  close(fd2);

  return 0;
}

int max(int fd1, int fd2) {
  return fd1 > fd2 ? fd1 : fd2;
}

void fsm_write(struct fsm *fsm) {
  // 开始写入数据
  int ret = write(fsm->dfd, fsm->buffer+fsm->index, fsm->rlen);
  if (ret < 0) {
    if (EAGAIN == ret)
      fsm->state = STATE_W; // 假错继续写
    else
      fsm->state = STATE_E; // 真错更换状态

    fsm->strerr = "write() erro";
  } else {
    if (ret < fsm->rlen) {  // 没有写完继续写
      fsm->state = STATE_W;
      fsm->index += ret;
      fsm->rlen -= ret;
    } else {
      fsm->state = STATE_R; // 写完改成读
    }
  }
}

void fsm_read(struct fsm *fsm) {
  // 读取数据存放在状态机的 buffer 中
  fsm->rlen = read(fsm->sfd, fsm->buffer, BUFFERSIZE);
  fsm->index = 0;
  if (fsm->rlen < 0) {
    if (EAGAIN == errno) {
      fsm->state = STATE_R; // 假错继续读
    } else {
      fsm->state = STATE_E; // 真错
      fsm->strerr = "read() error";
    }
  } else {
    fsm->index = 0;
    fsm->state = STATE_W; // 没有错误则开始写
  }
}

void fsm_driver(struct fsm *fsm) {
  // 根据当前状态机的状态分别处理
  switch (fsm->state) {
    case STATE_R:
    fsm_read(fsm);
    break;
    case STATE_W:
    fsm_write(fsm);
    break;
    case STATE_E:
    perror(fsm->strerr);
    fsm->state = STATE_T;
    break;
    case STATE_T:
    // do sth
    break;
    default:
    abort();
    break;
  }
}

void relay(int fd1, int fd2) {
  // 2. 设置所有的文件描述符为非阻塞
  int fd1_save, fd2_save;
  // 保存原先的文件描述符状态
  fd1_save = fcntl(fd1, F_GETFL);
  fd2_save = fcntl(fd2, F_GETFL);

  // 设置为非阻塞模式
  fcntl(fd1, F_SETFL, fd1_save | O_NONBLOCK);
  fcntl(fd2, F_SETFL, fd2_save | O_NONBLOCK);

  // 初始化状态机，一个状态机的初始状态都是读
  struct fsm fsm1;
  fsm1.sfd = fd1;
  fsm1.dfd = fd2;
  fsm1.state = STATE_R;

  struct fsm fsm2;
  fsm2.sfd = fd2;
  fsm2.dfd = fd1;
  fsm2.state = STATE_R;

  // 创建一棵事件树
  int epfd = epoll_create(10);
  if (-1 == epfd) {
    perror("epoll_create() error");
    fcntl(fd1, F_SETFL, fd1_save);
    fcntl(fd2, F_SETFL, fd2_save);
    return;
  }

  // 接事件添加到事件树中
  struct epoll_event ev;
  ev.events = 0;
  ev.data.fd = fd1;
  epoll_ctl(epfd, EPOLL_CTL_ADD, fd1, &ev); 

  ev.events = 0;
  ev.data.fd = fd2;
  epoll_ctl(epfd, EPOLL_CTL_ADD, fd2, &ev);

  int nready = 0;
  while (fsm1.state != STATE_T || fsm2.state != STATE_T) {
    // 判断状态机的状态，在事件树种修改指定的事件节点
    ev.data.fd = fd1;
    ev.events = 0;
    if (fsm1.state == STATE_R)
      ev.events |= EPOLLIN;
    if (fsm2.state == STATE_W)
      ev.events |= EPOLLOUT;
    epoll_ctl(epfd, EPOLL_CTL_MOD, fd1, &ev); 

    ev.data.fd = fd2;
    ev.events = 0;
    if (fsm2.state == STATE_R)
      ev.events |= EPOLLIN;
    if (fsm1.state == STATE_W)
      ev.events |= EPOLLOUT;
    epoll_ctl(epfd, EPOLL_CTL_MOD, fd2, &ev); 
    // 只有在读和写的状态才进行监视
    struct epoll_event events[2] = {0};
    if (fsm1.state < STATE_A || fsm2.state < STATE_A) {
      nready = epoll_wait(epfd, events, 2, -1);
      if (-1 == nready) {
        if (EINTR == errno)
          continue;
        
        perror("poll()");
        exit(EXIT_FAILURE);
      }
    }

    // 有状态变化或处以错误或退出状态均需要去驱动状态机
    for (int i = 0; i < nready; ++i) {
      int fd = events[i].data.fd;
      if (fd == fd1 && (events[i].events & EPOLLIN) ||
          fd == fd2 && (events[i].events & EPOLLOUT) ||
          fsm1.state > STATE_A)
        fsm_driver(&fsm1);

      if (fd == fd2 && (events[i].events & EPOLLIN) ||
          fd == fd1 && (events[i].events & EPOLLOUT) ||
          fsm2.state > STATE_A)
        fsm_driver(&fsm2);
    }
  }

  // 退出之前恢复文件描述符的状态
  fcntl(fd1, F_SETFL, fd1_save);
  fcntl(fd2, F_SETFL, fd2_save);
  close(epfd);
}