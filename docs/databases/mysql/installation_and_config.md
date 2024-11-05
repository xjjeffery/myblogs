# MySQL 安装与配置

## MySQL 安装

MySQL 一般分为社区版和商业版，我们使用的是社区版（因为免费）。MySQL 安装的教程在网上有很多，此处就不再进行进行赘述，这里推荐两篇文章：[如何在 Windows11 中安装 MySQL 8.0](https://medium.com/@gayankurukulasooriya/how-to-install-mysql-8-0-33-server-on-windows-11-e5ddb2e9cc6e) 和 [Ubuntu20.04 安装 MySQL 并配置远程访问](https://blog.csdn.net/qq_43686863/article/details/130558349)，大家可以按照文章中的步骤安装。

### 启动停止和连接

一般在 MySQL 安装以后，在系统启动时，会自动启动 MySQL 服务，我们无需手动启动。我们也可以通过指令启动和停止 MySQL 服务：

- 在 Linux 中，输入 `sudo systemctl stop mysql.service`/`sudo systemctl start mysql.service` 来停止/启动 MySQL 服务
- 在 Windows 中输入 `net stop mysql80`/`net start mysql80` 来停止/启动 MySQL 服务（要使用管理员权限打开终端操作）

用户连接到 MySQL 服务器的方式有两种：

1. 使用具有可视化界面的工具连接，如 MySQL 官网提供的 Workbench
2. 使用命令连接，基本语法为 `mysql [-h IP地址] [-P 端口号] -u <用户名> -p`
    - `-h`：MySQL 服务所在的主机 IP
    - `-P`：MySQL 服务端口号，默认 3306
    - `-u`：MySQL 数据库用户名
    - `-p`：MySQL 数据库用户名对应的密码

> `[]` 内为可选参数，如果需要连接远程的 MySQL，需要加上这两个参数来指定远程主机 IP、端口，如果
连接本地的 MySQL，则无需指定这两个参数。

![在这里插入图片描述](https://i-blog.csdnimg.cn/direct/f1de6c2752cf49e0a00e25d3b840413f.png)

## 基本问题

Ubuntu20.04 安装完 MySQL 以后，在没有管理员权限或超级用户权限下，是无法使用 MySQL 的，因为此时本地数据库服务器中的 `root` 用户的密码是空的，我们无法通过此用户进行登录，因此安装后的第一步是为 `root` 用户修改密码：

```bash
# 切换超级用户权限
root@ubuntu:~$ su -
# 打开数据库
root@ubuntu:~# mysql
Welcome to the MySQL monitor.  Commands end with ; or \g.
Your MySQL connection id is 17
Server version: 8.0.39-0ubuntu0.20.04.1 (Ubuntu)

Copyright (c) 2000, 2024, Oracle and/or its affiliates.

Oracle is a registered trademark of Oracle Corporation and/or its
affiliates. Other names may be trademarks of their respective
owners.

Type 'help;' or '\h' for help. Type '\c' to clear the current input statement.
# 进入到数据库操作，修改 root 密码
mysql> ALTER USER 'root'@'localhost' identified with mysql_native_password by '<密码>';
Query OK, 0 rows affected (0.00 sec)
```

密码修改完成以后，就可以直接以普通用户身份打开数据库，基本命令格式为 `mysql -u <数据库用户名> -p`，如下所示：

```bash
root@ubuntu:~$ mysql -u root -p
Enter password: 
Welcome to the MySQL monitor.  Commands end with ; or \g.
Your MySQL connection id is 18
Server version: 8.0.39-0ubuntu0.20.04.1 (Ubuntu)

Copyright (c) 2000, 2024, Oracle and/or its affiliates.

Oracle is a registered trademark of Oracle Corporation and/or its
affiliates. Other names may be trademarks of their respective
owners.

Type 'help;' or '\h' for help. Type '\c' to clear the current input statement.
```

在 Windows 中操作基本相同，如果是按上面推荐教程逐步安装，在安装的过程中会要求我们输入 `root` 的密码，则不需要进行后续的修改。

## 远程访问

一般开发中 MySQL 数据库服务都是部署在 Docker 或 Linux 中，在 Windows 中使用数据库都是通过远程访问。现以 Windows 远程连接虚拟机 Ubuntu20.04 中的数据库为例，在 Windows 中打开 Workbench8.0（上述推荐 Windows 教程中会安装），在虚拟机中启动 mysql.server 服务。

![在这里插入图片描述](https://i-blog.csdnimg.cn/direct/2f42be4ec2c54e2f85c735dfea23027f.png)

如上图所示，按顺序在 Windows 中向虚拟机的数据库服务器发起连接请求，在第 4 步的时候点击测试连接，会出现如下的错误：

![在这里插入图片描述](https://i-blog.csdnimg.cn/direct/90678d61f0274a75ad98c5ef8be7cf1f.png)

这是因为 root 用户默认的绑定地址是 localhost，无法通过网络进行连接，需要在 `/etc/mysql/my.conf` 文件中增加或修改 `bind-adress = 0.0.0.0` 用来监听所有网段的连接。

```ini
[mysqld]  # 必须位于此选项下，否则会出问题
    bind-address = 0.0.0.0
```

保存并退出，重启 mysql.service。

```bash
$ sudo systemctl restart mysql.service
```

此时再次通过 Workbench 进行连接，会出现一个新的问题：

![在这里插入图片描述](https://i-blog.csdnimg.cn/direct/29f657e14656431985a8df8bde9f2e22.png)

这是因为数据库限制了 `root` 的远程登录，最好的办法是创建一个新的用户。

```bash
# 创建用户之前先通过 root 用户进入数据库
$ mysql -u root -p
Enter password: 
Welcome to the MySQL monitor.  Commands end with ; or \g.
Your MySQL connection id is 10
Server version: 8.0.39-0ubuntu0.20.04.1 (Ubuntu)

Copyright (c) 2000, 2024, Oracle and/or its affiliates.

Oracle is a registered trademark of Oracle Corporation and/or its
affiliates. Other names may be trademarks of their respective
owners.

Type 'help;' or '\h' for help. Type '\c' to clear the current input statement.

# CREATE USER '<用户名>'@'%' IDENTIFIED BY '<密码>'; # % 表示可以监听所有网段
mysql> CREATE USER 'jeffery'@'%' IDENTIFIED BY '123456';
```

创建完成后再次通过 Workbench 对新建用户建立连接，成功建立连接。

![在这里插入图片描述](https://i-blog.csdnimg.cn/direct/3fbce78b3842475285072c66df09f4a0.png)

新建的用户权限有限，即使是在本地操作，也仅仅只能使用几个简单的语句，如 `SHOW`。需要使用 `GRANT` 增加新建用户的权限，这样我们就可以直接通过新建用户来操作数据库，而不是通过 `root` 用户（在后续的基础篇会有介绍，此处了解即可）。

```bash
# 授权权限必须是 root 用户进行
$ mysql -u root -p
Enter password: 
Welcome to the MySQL monitor.  Commands end with ; or \g.
Your MySQL connection id is 10
Server version: 8.0.39-0ubuntu0.20.04.1 (Ubuntu)

Copyright (c) 2000, 2024, Oracle and/or its affiliates.

Oracle is a registered trademark of Oracle Corporation and/or its
affiliates. Other names may be trademarks of their respective
owners.

Type 'help;' or '\h' for help. Type '\c' to clear the current input statement.

# GRANT <权限类别> ON <指定的库/表> TO '<用户名>'@'<主机>';
mysql> GRANT ALL ON learn_mysql TO 'jeffery'@'%';

# 权限授权完成后，需要刷新权限
mysql> FLUSH PRIVILEGES;
```

## MySQL 配置文件

在使用 MySQL 时需要配置各种参数和选项，这个可以通过 MySQL 的配置文件来设置，在 Windows 中一般会在 MySQL server 的安装目录下添加一个 `my.ini` 文件进行设置，而在 Ubuntu 则在 `/etc/mysql/my.cnf` 文件中进行设置，设置的内容示例如下:

- **Ububtu**：

```ini
[mysqld]
datadir=/var/lib/mysql
socket=/var/lib/mysql/mysql.sock

[mysql.server]
user=mysql
basedir=/var/lib

[safe_mysqld]
err-log=/var/log/mysqld.log
pid-file=/var/run/mysqld/mysqld.pid
```

- **Windows**：

```ini
[mysqld]
# 设置 3306 端口
port=3306
# 设置 mysql 的安装目录 ---这里输入你安装的文件路径----
basedir=C:\Program Files\MySQL
# 设置 mysql 数据库的数据的存放目录
datadir=C:\Program Files\MySQL\data
# 允许最大连接数
max_connections=200
# 允许连接失败的次数
max_connect_errors=10
# 服务端使用的字符集默认为 utf8
character-set-server=utf8
# 创建新表时将使用的默认存储引擎
default-storage-engine=INNODB
# 默认使用 “mysql_native_password” 插件认证
# mysql_native_password
default_authentication_plugin=mysql_native_password
[mysql]
# 设置 mysql 客户端默认字符集
default-character-set=utf8
[client]
# 设置 mysql 客户端连接服务端时默认使用的端口
port=3306
default-character-set=utf8
```

配置文件一般包含以下几部分：

- 基本设置
    - `basedir`: MySQL 服务器的基本安装目录
    - `datadir`: 存储 MySQL 数据文件的位置
    - `socket`: MySQL 服务器的 Unix 套接字文件路径
    - `pid-file`: 存储当前运行的 MySQL 服务器进程 ID 的文件路径
    - `port`: MySQL 服务器监听的端口号，默认是 3306
- 服务器选项
    - `bind-address`: 指定 MySQL 服务器监听的 IP 地址，可以是 IP 地址或主机名
    - `server-id`: 在复制配置中，为每个 MySQL 服务器设置一个唯一的标识符
    - `default-storage-engine`: 默认的存储引擎，例如 InnoDB 或 MyISAM
    - `max_connections`: 服务器可以同时维持的最大连接数
    - `thread_cache_size`: 线程缓存的大小，用于提高新连接的启动速度
    - `query_cache_size`: 查询缓存的大小，用于提高相同查询的效率
    - `default-character-set`: 默认的字符集
    - `collation-server`: 服务器的默认排序规则
- 性能调优
    - `innodb_buffer_pool_size`: InnoDB 存储引擎的缓冲池大小，这是 InnoDB 性能调优中最重要的参数之一
    - `key_buffer_size`: MyISAM 存储引擎的键缓冲区大小
    - `table_open_cache`: 可以同时打开的表的缓存数量
    - `thread_concurrency`: 允许同时运行的线程数
- 安全设置
    - `skip-networking`: 禁止 MySQL 服务器监听网络连接，仅允许本地连接
    - `skip-grant-tables`: 以无需密码的方式启动 MySQL 服务器，通常用于恢复忘记的 root 密码，但这是一个安全风险
    - `auth_native_password=1`: 启用 MySQL 5.7 及以上版本的原生密码认证
- 日志设置
    - `log_error`: 错误日志文件的路径
    - `general_log`: 记录所有客户端连接和查询的日志
    - `slow_query_log`: 记录执行时间超过特定阈值的慢查询
    - `log_queries_not_using_indexes`: 记录未使用索引的查询
- 复制设置
    - `master_host` 和 `master_user`: 主服务器的地址和复制用户
    - `master_password`: 复制用户的密码
    - `master_log_file` 和 `master_log_pos`: 用于复制的日志文件和位置

## 概述

### 数据库相关概念

在学习 MySQL 数据库之前，先区分三个概念：数据库、数据库管理系统、SQL。

| 名称 | 全称 | 简称 |
| --- | --- | --- |
| 数据库 | 存储数据库的仓库，数据是有组织的进行存储 | DataBase（DB） |
| 数据库管理系统 | 操纵和管理数据库的大型软件 | DataBase Management System（DBMS） |
| SQL | 操作关系型数据库的编程语言，定义了一套操作关系型数据库统一标准 | Structured Query Language（SQ） |

用户使用 SQL 语言可以操作数据库管理系统，数据库管理系统解析 SQL 语言，对数据库进行操作，如下图所示：

![在这里插入图片描述](https://i-blog.csdnimg.cn/direct/3caef34713c14a7695fdaa5c32af567f.png)

### 数据模型

关系数据库是指建立在关系模型基础上，由多张相互连接的二维表组成的数据库。而所谓二维表，指的是由行和列组成的表，如下图（就类似于 Excel 表格数据，有表头、有列、有行，还可以通过一列关联另外一个表格中的某一列数据）。如 MySQL、Oracle、DB2、SQLServer 这些都是属于关系型数据库，里面都是基于二维表存储数据的。简单说，基于二维表存储数据的数据库就成为关系型数据库，不是基于二维表存储数据的数据库，就是非关系型数据库。

![在这里插入图片描述](https://i-blog.csdnimg.cn/direct/5c94910a5fb948eaa9c7e4307f53febe.png)

**特点**：

- 使用表存储数据，格式统一，便于维护
- 使用 SQL 语言操作，标准统一，使用方便

MySQL 是关系型数据库，是基于二维表进行数据存储的，具体的结构图下:

![在这里插入图片描述](https://i-blog.csdnimg.cn/direct/3b5ddc9f99ca4e7189f10c1bc1652b1d.png)

- 我们可以通过客户端连接数据库管理系统 DBMS，然后通过 DBMS 操作数据库。
- 可以使用 SQL 语句，通过数据库管理系统操作数据库，以及操作数据库中的表结构及数据。
- 一个数据库服务器中可以创建多个数据库，一个数据库中也可以包含多张表，而一张表中又可以包
含多行记录。