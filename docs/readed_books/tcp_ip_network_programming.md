# TCP/IP 网络编程

**阅读需要具备的基础**：熟悉 C 语言编程、熟悉 C 语言程序在 Linux 或 Windows 下的编写和编译。

## 理解网络编程和套接字

网络编程就是编写程序让两个计算机通过网络进行数据交互，我们不需要关注计算机之间是用什么传输数据的，也不需要关注传输数据的软件是什么，关注的是如何将让两个计算机建立连接以及怎么传输数据。

网络服务一般都会有服务端和客户端，服务端用来接收客户端的连接请求、接收和转发客户端的数据，客户端则是进行多个客户端的数据交互，如微信中好友之间的聊天等。下面先来了解 TCP 服务。

### 服务端

tcp 服务端的建立在 Linux 中一般有四步，在 Windows 中要多几步，多的这几步仅限对库的使用，先看两个都有的四步，类比家里安装固定电话机：

1. 创建套接字 ——> 相当于购买一个电话机硬件，通信的前提条件

=== "Linux"

    ```c
    #include <sys/types.h>
    #include <sys/socket.h>

    // 成功返回文件描述符，失败返回 -1
    int socket(int domain, int type, int protocol);
    ```

=== "Windows"

    ```c
    #include <winsock2.h>

    // 成功返回套接字，失败返回 INVALID_SOCKET
    SOCKET socket(int af, int type, int protocol);
    ```

2. 给套接字绑定地址信息 ——> 选取电话号码，确保别人知道你的电话号码，可以通过这个号码打过来

=== "Linux"

    ```c
    #include <sys/types.h>
    #include <sys/socket.h>

    // 成功返回 0，失败返回 -1
    int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
    ```

=== "Windows"

    ```c
    #include <winsock2.h>

    // 成功返回 0，失败返回 SOCKET_ERROR
    int bind(SOCKET s, const struct sockaddr *addr, socklen_t addrlen);
    ```

3. 将套接字设置为接收连接的状态 ——> 接上电信局的电话线，电话就处于一个可接听的状态

=== "Linux"

    ```c
    #include <sys/types.h>
    #include <sys/socket.h>

    // 成功返回 0，失败返回 -1
    int listen(int sockfd, int backlog);
    ```

=== "Windows"

    ```c
    #include <winsock2.h>

    // 成功返回 0，失败返回 SOCKET_ERROR
    int listen(SOCKET s, int backlog);
    ```

4. 接收连接请求 ——> 拿起电话进行接听

=== "Linux"

    ```c
    #include <sys/types.h>
    #include <sys/socket.h>

    // 成功返回非负整数，这个整数是客户端的文件描述符，失败返回 -1
    int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
    ```

=== "Windows"

    ```c
    #include <winsock2.h>

    // 成功返回非负整数，这个整数是客户端的套接字，失败返回 SOCKET_ERROR
    SOCKET accept(SOCKET s, struct sockaddr *addr, socklen_t *addrlen);
    ```

如果使用 Windows 编写网络相关的程序，必须使用 `winsock2.h` 库，并且在编译的时候需要链接 `ws2_32`。在代码中还需初始化此库，并且在结束的时候还需要注销此库，函数如下

```c
#include <winsock2.h>

// 成功返回 0，失败返回非 0 的错误代码值
int WSAStartup(WORD wVersionRequested, LPWSDATA lpWSAData);

// WORD 表示 winsock 的版本类型，直接传递则需要使用十六进制表示，高 8 位为副版本号，低八位为主版本号
// 为了方便可以使用 MAKEWORD 函数，只需要两个参数，主版本号和副版本号，如 MAKEWORD(2, 1);
// 第二个参数就是一个 WSADATA 结构体变量的地址，将其传入即可

int WSACleanup(void);
```

初次之外，关闭套接字也是使用独有的函数

```c
#include <winsock2.h>

// 成功返回 0，失败返回 SOCKET_ERROR
int closesocket(SOCKET s);
```

简单 tcp 服务端程序如下：

=== "Linux"

    ```c
    #include <arpa/inet.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <unistd.h>

    #define BUFFERSIZE 1024

    int main(int argc, char *argv[]) {
      if (2 != argc) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
      }

      // 1. 创建套接字
      int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
      if (-1 == sock_fd) {
        perror("socket error");
        exit(EXIT_FAILURE);
      }

      // 2. 绑定地址信息
      struct sockaddr_in serv_addr;
      serv_addr.sin_family = AF_INET;
      serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
      serv_addr.sin_port = htons(atoi(argv[1]));
      if (-1 == bind(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) {
        perror("bind error");
        exit(EXIT_FAILURE);
      }

      // 3. 建立接收连接通道
      if (-1 == listen(sock_fd, 5)) {
        perror("listen error");
        exit(EXIT_FAILURE);
      }

      // 4. 接收连接请求
      struct sockaddr_in clnt_addr;
      socklen_t len = sizeof(clnt_addr);
      int clnt_fd = accept(sock_fd, (struct sockaddr *)&clnt_addr, &len);
      if (-1 == clnt_fd) {
        perror("accept error");
        exit(EXIT_FAILURE);
      }

      char messages[] = "hello world!";
      write(clnt_fd, messages, sizeof(messages));

      close(clnt_fd);
      close(sock_fd);

      return 0;
    }
    ```

=== "Windows"

    ```c
    #include <stdio.h>
    #include <stdlib.h>
    #include <WinSock2.h>

    void error_handling(const char *msg);

    int main(int argc, char *argv[]) {
      if (2 != argc) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
      }

      // 初始化 winsock 库
      WSADATA wsa_data;
      WSAStartup(MAKEWORD(2, 2), &wsa_data);

      // 1. 创建套接字
      SOCKET serv_sock = socket(AF_INET, SOCK_STREAM, 0);
      if (-1 == serv_sock)
        error_handling("socket error");

      // 2. 绑定本地信息
      struct sockaddr_in serv_addr;
      serv_addr.sin_family = AF_INET;
      serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
      serv_addr.sin_port = htons(atoi(argv[1]));
      if (-1 == bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)))
        error_handling("bind error");

      // 3. 打开可连接状态
      if (-1 == listen(serv_sock, 5))
        error_handling("listen error");

      // 4. 接收客户端的连接
      struct sockaddr_in clnt_addr;
      int addrlen = sizeof(clnt_addr);
      SOCKET clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &addrlen);
      if (-1 == clnt_sock)
        error_handling("accept error");

      char message[] = "hello world!";
      int size = send(clnt_sock, message, sizeof(message), 0);
      if (-1 == size)
        error_handling("send error");

      closesocket(clnt_sock);
      closesocket(serv_sock);
      // 注销 winsock 库
      WSACleanup();
      return 0;
    }

    void error_handling(const char *msg) {
      fputs(msg, stderr);
      fputc('\n', stderr);
      exit(EXIT_FAILURE);
    }
    ```

### 客户端

tcp 客户端的搭建只需要两个步骤：

1. 创建套接字 ——> 购买一个电话机
2. 请求连接 ——> 拨打电话

=== "Liunx"

    ```c
    #include <sys/types.h>
    #include <sys/socket.h>

    // 成功返回 0，失败返回 -1
    int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
    ```

=== "Windows"

    ```c
    #include <winsock2.h>

    // 成功返回 0，失败返回 SOCKET_ERROR
    int connect(SOCKET s, const struct sockaddr *addr, socklen_t addrlen);
    ```

简单 tcp 客户端程序如下：

=== "Linux"

    ```c
    #include <arpa/inet.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <sys/socket.h>
    #include <sys/types.h>
    #include <unistd.h>

    #define BUFFERSIZE 1024

    int main(int argc, char *argv[]) {
      if (3 != argc) {
        fprintf(stderr, "Usage: %s <ip> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
      }

      // 1. 创建套接字
      int clnt_fd = socket(AF_INET, SOCK_STREAM, 0);
      if (-1 == clnt_fd) {
        perror("socket error");
        exit(EXIT_FAILURE);
      }

      // 2. 向服务器发送连接请求
      struct sockaddr_in clnt_addr;
      clnt_addr.sin_family = AF_INET;
      clnt_addr.sin_addr.s_addr = htonl(argv[1]);
      clnt_addr.sin_port = htons(atoi(argv[2]));
      if (-1 == connect(clnt_fd, (struct sockaddr *)&clnt_addr, sizeof(clnt_addr))) {
        perror("connect error");
        exit(EXIT_FAILURE);
      }

      char buffer[BUFFERSIZE] = {0};
      ssize_t len = read(clnt_fd, buffer, BUFFERSIZE);
      if (-1 == len) {
        perror("read error");
        exit(EXIT_FAILURE);
      }

      printf("read from server: %s\n", buffer);
      close(clnt_fd);

      return 0;
    }
    ```

=== "Windows"

    ```c
    #include <stdio.h>
    #include <stdlib.h>
    #include <WinSock2.h>

    #define BUFFERSIZE 1024

    void error_handling(const char *msg);

    int main(int argc, char *argv[]) {
      if (3 != argc) {
        fprintf(stderr, "Usage: %s <ip> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
      }

      // 初始化 winsock 库
      WSADATA wsa_data;
      WSAStartup(MAKEWORD(2, 2), &wsa_data);

      // 1. 创建套接字
      SOCKET clnt_sock = socket(AF_INET, SOCK_STREAM, 0);
      if (-1 == clnt_sock)
        error_handling("socket error");

      // 2. 向服务器发送连接请求
      struct sockaddr_in clnt_addr;
      clnt_addr.sin_family = AF_INET;
      clnt_addr.sin_addr.s_addr = inet_addr(argv[1]);
      clnt_addr.sin_port = htons(atoi(argv[2]));
      if (-1 == connect(clnt_sock, (struct sockaddr *)&clnt_addr, sizeof(clnt_addr)))
        error_handling("connect error");

      char buffer[BUFFERSIZE] = {0};
      int len = recv(clnt_sock, buffer, BUFFERSIZE, 0);
      if (-1 == len)
        error_handling("recv error");
      printf("buffer from server: %s\n", buffer);

      closesocket(clnt_sock);
      // 注销 winsock 库
      WSACleanup();
      return 0;
    }

    void error_handling(const char *msg) {
      fputs(msg, stderr);
      fputc('\n', stderr);
      exit(EXIT_FAILURE);
    }
    ```

编译运行上述两个程序，先启动服务端的程序，再启动客户端的程序。此时客户端会收到服务端发来的数据，并且两个程序都会立即退出。

上述两个程序使用 `read` 和 `write` 函数，是因为在 Linux 中，一切都是文件，socket 也是文件，因此可以使用文件相关的读写操作。而在 Windows 中，网络套接字和文件是有区别的，需要使用网络读写专用的函数 `recv` 和 `send` 来进行操作，这两个函数在 Linux 中也适用。
