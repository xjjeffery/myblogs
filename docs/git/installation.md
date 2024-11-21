# Git 安装与配置

## 安装Git

在 Ubuntu 系统上，可通过 `ppa` 来安装较新版本的 Git。

```bash
sudo add-apt-repository ppa:git-core/ppa
sudo apt update
sudo apt install git
```

在 Windows 上可以通过 [Git 官网](https://www.git-scm.com/)进行下载安装，或者通过 Chocolatey 包管理器进行安装(也可以使用其他包管理器)，安装命令如下

```bash
choco install git
```

Windows 上下载 Git 还会有一个 GUI 工具一起下载，使用更加简单，更易上手。

可以通过指令 `git --version` 查看所安装的 Git 版本号：

```bash
$ git --version
git version 2.41.0 // 此版本是 Linux 上的
```

## 配置等级

Git 配置主要分为三个级别，即 system、global 和 local，优先级是 system < global < local。

system 级别配置为所有用户共享，优先级最低。在 Ubuntu 系统中，system 级别配置文件位于 `/etc/gitconfig`；在 Windows 系统中，system 级别配置文件位于自己的安装目录。使用 `git config --system` 命令或通过 `/etc/gitconfig` 文件可对 system 级配置进行查询和修改。

global 级别配置特定于单一用户。在 Ubuntu 系统中，global 级配置文件位于 `${XDG_CONFIG_HOME}/git/config` 或 `~/.gitconfig` （如前者不存在）。如果环境变量 `XDG_CONFIG_HOME` 未设置或为空值，则默认为 `~/.config`。使用 `git config --global` 命令或通过 `${XDG_CONFIG_HOME}/git/config` (`~/.gitconfig`) 文件可对 global 级别配置进行查询和修改。在Windows系统中，global 级别配置文件位于 `C:\Users\<system user_name>\.gitconfig`。

local 级别配置特定于单一 Git 仓库，优先级最高，local 级别配置文件的路径为 `<REPO_DIR>/.git/config`。使用 `git config --local` 命令（设置 local 级别选项时可省略 `--local`）或通过 `<REPO_DIR>/.git/config` 文件可对 local 级别配置进行查询和修改。

![](./assets/git-installation_1.png)

**Git 查看配置指令**

```bash
# 查看 Git 配置
git config --list
git config -l

# 查看系统配置
git config --system --list

# 查看全局配置
git config --global --list

# 查看当前仓库的配置
git config --local --list
```

## 配置

### 初始化用户信息

当安装 Git 后首先要做的事情是配置你的用户信息 —— 告诉 Git 你是谁？配置用户名、邮箱地址，每次提交文件时都会带上这个用户信息，查看历史记录时就知道是谁干的了。

配置用户信息：

```bash
$ git config --global user.name "Your Name"
$ git config --global user.email "email@example.com"
# 配置完后，看看用户配置文件：
$ git config --global --list
user.name=<Your Name>
user.email=<email@example.com>
```

!!! tip

    - `user.name` 为用户名，`user.email` 为邮箱。
    - `--global`：config 的参数，表示用户全局配置。如果要给特定仓库配置用户信息，则用参数 `--local` 配置即可，或直接在仓库配置文件 `.git/config` 里修改。

### 设置代理

Git 支持 SOCKS/HTTP/HTTPS 代理协议，但与大多数终端应用程序不同，我们无法通过设置 Shell 环境变量 `HTTP_PROXY` 或 `HTTPS_PROXY` 来让 Git 使用代理。正确的方法是通过 Git 配置选项来设定，例如：

```bash
git config --global http.proxy 'socks5://127.0.0.1:7890'
git config --global https.proxy 'socks5://127.0.0.1:7890'
```

### 全局配置（推荐）

这里推荐一些自己使用的推荐设置，为方便操作，对一些常用命令设置了别名；同时规定了换行符在 `checkin/checkout` 时的转换规则。

对于篇幅较长或者对排版有要求的提交信息，很难在命令行直接输入，因此需使用文本编辑器进行撰写。`editor` 选项指定了用于撰写提交信息的文本编辑器，可根据喜好自行修改。如果 `editor` 选项未设定，Git 会默认打开由环境变量 `${EDITOR}` 设定的文本编辑器。

配置中的最后两行意在使用 `ghproxy.net` 镜像站点来下载 Github 资源。对于任何以 `https://github.com/` 开头的 URL，Git 会自动将其转换成以 `https://ghproxy.net/` 开头的镜像地址。例如，`https://github.com/foo/bar.git` 会被自动转换成 `https://ghproxy.net/https://github.com/foo/bar.git`。ghproxy 会因访问量太大而暂时无法连接，也可能被一些网关屏蔽而完全无法访问，如果你所处的网络环境允许你畅通无阻地访问 Github，又或者你已使用稳定的代理来访问 Github，那么可注释掉该设定。

```ini
[user]
  name = <your name>
  email = <your email>
[alias]
  co = checkout
  ci = commit
  s = status -sb
  st = status
  br = branch
  lg = log --graph --pretty=format:'%C(bold yellow)%h%C(reset) -%C(auto)%d%C(bold reset) %s %C(bold green)(%ad)%C(reset) %C(bold cyan)[%cn]' --abbrev-commit --date=short
  type = cat-file -t
  dump = cat-file -p
[i18n]
  logOutputEncoding = utf-8
  commitEncoding = utf-8
[core]
 autocrlf = false
 safecrlf = true
 editor = nvim
[url "https://ghproxy.net/https://github.com/"]
  insteadOf = https://github.com/
; [http]
;   proxy = http://127.0.0.1:7890
; [https]
;   proxy = https://127.0.0.1:7890
```

???+ tip

    Git 配置文件使用 `;` 或 `#` 作为注释起始字符。通常，`#` 用于描述性文字，`;` 用于取消选项设定。

如果需要在 Ubuntu 系统的 `bash` 中显示 git 分支信息，可以在 `.bashrc` 文件中添加如下内容：

```bash
parse_git_branch() {
  git branch 2> /dev/null | sed -e '/^[^*]/d' -e 's/* \(.*\)/(\1) /'
}
export PS1="$PS1\[\e[91m\]\$(parse_git_branch)\[\e[00m\]\n> "
```

## `.gitignore`

工作目录中的文件并不是全都需要纳入版本管理，如日志、临时文件、私有配置文件等不需要也不能纳入版本管理，那该怎么办呢？在工作区根目录下创建 `.gitignore` 文件，文件中配置不需要进行版本管理的文件、文件夹。 `.gitignore` 文件本身是被纳入版本管理的，可以共享。有如下规则：

- `#` 符号开头为注释。
- 可以使用 Linux 通配符。
    - 星号（`*`）代表任意多个字符，
    - 问号（`?`）代表一个字符，
    - 方括号（`[abc]`）代表可选字符范围，
    - 大括号（`{string1,string2,...}`）代表可选的字符串等。
- 感叹号（`!`）开头：表示例外规则，将不被忽略。
- 路径分隔符（`/f`）开头：`,` 表示要忽略根目录下的文件f。
- 路径分隔符（`f/`）结尾：`,` 表示要忽略文件夹f下面的所有文件。

```bash
# 为注释
*.txt # 忽略所有“.txt”结尾的文件
!lib.txt # lib.txt除外
/temp # 仅忽略项目根目录下的temp文件,不包括其它目录下的temp，如不包括“src/temp”
build/ # 忽略build/目录下的所有文件
doc/*.txt # 会忽略 doc/notes.txt 但不包括 doc/server/arch.txt
```

!!! note

    各种语言项目的常用 .gitignore 文件配置：[https://github.com/github/gitignore](https://github.com/github/gitignore)
