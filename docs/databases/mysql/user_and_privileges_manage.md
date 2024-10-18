# 用户和权限管理

## 用户管理

MySQL 支持用户设置，包括用户创建、设置权限、管理用户等操作。

### 创建用户

如果需要创建用户，可以使用以下命令：

```sql
CREATE USER '<username>'@'<host>' IDENTIFIED BY '<password>';
```

- `username`：新建用户的用户名
- `host`：指定哪些网络地址连接到此用户，如 localhost 仅允许本地连接，% 允许从任何网络地址连接
- `password`：此用户的密码

**示例**:

```sql
CREATE USER 'jeffery'@'%' IDENTIFIED BY '123456';
```

### 删除用户

如果需要删除用户，可以使用以下命令：

```sql
DROP USER '<username>'@'<host>';
```

**示例**:

```sql
DROP USER 'jeffery'@'%';
```

### 修改用户密码

要修改用户的密码，可以使用 `ALTER USER` 命令：

```sql
ALTER USER '<username>'@'<host>' IDENTIFIED BY '<new_password>';
```

**示例**:

```sql
ALTER USER 'jeffery'@'%' IDENTIFIED BY '654321';
```

### 修改用户主机

要更改用户的主机（即允许从哪些主机连接），可以先删除用户，再重新创建一个新的用户。

**示例**：

```sql
-- 删除旧用户
DROP USER 'jeffery'@'localhost';

-- 重新创建用户并指定新的主机
CREATE USER 'jeffery'@'%' IDENTIFIED BY 'password123';
```

## 权限管理

### 授予权限

新创建的一个用户没有什么权限，如果需要使用新建的用户，必须为其添加所使用的权限，使用 `GRANT` 命令来授予权限，授予权限的时候必须使用 `root` 用户进行操作

```sql
GRANT <privileges> ON <database_name.*> TO '<username>'@'<host>';
```

- `privileges`： 所需权限，权限类别有以下多种形式，如 `ALL PRIVILEGES`、`SELECT`、`INSERT`、`UPDATE`、`DELETE` 等
- `database_name.*`：表示对某个数据库或表授予权限，`database_name.*` 表示 database_name 库下的所有表，也可以是指定的表 `database_name.table_name`
- `'<username>'@'<host>'`：表示授予权限的用户和主机

**示例**：

```sql
GRANT ALL PRIVILEGES ON test_db.* TO 'jeffery'@'localhost';
```

### 撤销权限

要撤销用户的权限，使用 `REVOKE` 命令：

```sql
REVOKE <privileges> ON <database_name.*> FROM '<username>'@'<host>';
```

**示例**：

```sql
REVOKE ALL PRIVILEGES ON test_db.* FROM 'jeffery'@'localhost';
```

### 刷新权限

授予或撤销权限后，需要刷新权限使更改生效，使用 `FLUSH PRIVILEGES` 命令。