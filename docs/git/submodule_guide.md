# Git 子模块的使用

Git 工具的 `submodule` 功能可以建立当前项目与子模块之间的依赖关系，用于跟踪管理主项目所依赖的子项目。主项目中的一个子模块，实际上是主项目仓库对子项目仓库中某一个提交的引用。子模块操作通过 `git submodule` 命令来进行。

## 基本操作

### 查看子模块状态

```bash
git submodule status
```

### 添加子模块

使用 `git submodule add` 向主项目仓库添加子模块，例如：

```bash
git submodule add -b master --name foo -- https://example.com/foobar/foo.git libs/foo
```

- `https://example.com/foobar/foo.git`: 子模块的 Git 仓库 URL
- `-b master`：将 `https://example.com/foobar/foo.git` 的 `master` 分支作为默认的远程跟踪分支（remote tracking branch），亦可换成任意 commit id。如果不指定子模块分支，则默认使用子模块仓库中 `HEAD` 指向的提交。
- `libs/foo`：子模块的工作树（worktree）路径，即子模块指向的提交的内容会 `checkout` 到该路径下。
- `--name foo`（optional）：设定子模块的别名。如果未显式指定，则默认使用子模块工作树路径名，即 `libs/foo`。

上面这条命令执行的动作可分解为：

- 在主项目根目录下下创建一个 `.gitmodules` 文件（如该文件尚不存在），并向该文件添加以下内容：

```ini
[submodule "libs/foo"]
    path = libs/foo
    url = https://example.com/foobar/foo.git
    branch = master
```

- 向主项目根目录下的 `.git/config` 文件添加以下内容：

```ini
[submodule "libs/foo"]
    url = https://example.com/foobar/foo.git
    active = true
```

- 将子模块仓库数据（`https://example.com/foobar/foo.git`）克隆到本地 `.git/modules/libs/foo/` 目录，相当于执行 `git clone`。
- 将子模块的 `master` 分支内容 checkout 到 `libs/foo/` 目录。
- 将 `.gitmodules` 和 `libs/foo/` 的变化添加到 index 区（相当于对主项目执行了 `git add .gitmodules libs/foo`），等待用户确认后提交(`git commit`)。

!!! tip

    执行 `git submodule add` 会同时向 `.gitmodules` 文件和 `.git/config` 文件添加 `[submodule]` 节点（section），但两者的用途是完全不同的。

### 设定子模块信息

子模块的相关信息可通过单独的命令进行设定或修改。

- `git submodule set-url`：设定子模块仓库 URL
- `git submodule set-branch`：为子模块设定默认的远程跟踪分支（remote tracking branch）

例如：

```bash
# Set repo url for submodule libs/foo
git submodule set-url libs/foo https://gitee.com/foobar/foo.git
# Set remote tracking branch for submodule libs/foo to dev
git submodule set-branch -b dev libs/foo
```

### 克隆子模块

如果主项目仓库已经包含了子模块，那么在使用 `git clone` 命令克隆主项目仓库时，只需使用 `--recurse-submodules` 选项，便可将子模块（包括嵌套的子模块）一并克隆到本地。例如：

```bash
git clone --recurse-submodules https://example.com/foobar/foo.git
```

如果需要对子模块进行浅克隆（shallow clone），则使用 `--shallow-submodules` 选项。例如：

```bash
git clone --recurse-submodules --shallow-submodules https://example.com/foobar/foo.git
```

甚至可以仅对指定的子模块进行浅克隆：

```bash
# <name> is the alias of the submodule to specify
git config -f .gitmodules submodule.<name>.shallow true
git clone --recurse-submodules https://example.com/foobar/foo.git
```

Git 默认使用串行方式克隆子模块，使用 `--jobs`（`-j`）选项可开启并行方式。例如：

```bash
git clone --recurse-submodules -j 8 https://example.com/foobar/foo.git
```

如果在克隆主项目仓库时没有将子模块一并克隆，后续可通过更新子模块的方法来获取。

!!! tip

    Git 1.6.5 版本引入了 `--recursive` 选项用于在克隆主项目仓库时递归克隆子模块，但从 1.7.11 版本起，使用 `--recurse-submodules` 取代了 `--recursive`。

### 更新子模块

子模块更新命令的常用形式：

```bash
git submodule update [--init] [--recursive] [--remote] [--checkout|-merge|--rebase] -- [<submod>]
```

- `--init`：进行子模块初始化。如果在 `git clone` 时没有将子模块一并克隆，可使用此选项先初始化子模块相关信息。关于 `--init` 选项的作用，见进一步理解子模块初始化。
- `--recursive`：递归更新子模块。
- `--remote`：使用子模块远程跟踪分支来更新子模块。（远程跟踪分支由 `git submodule add -b` 或 `git submodule set-branch` 设定）。如未使用 `--remote`，则使用主项目仓库所记录的子模块 commit id 来更新子模块。
- `<submod>`：子模块工作树路径或别名，用于指定需要更新的子模块。如果`<submod>` 未给出，那么将更新所有子模块。

子模块更新的默认操作是 `git checkout`，即将更新后子模块的 commit id 对应的内容 `checkout` 到子模块工作树，这也是最常见的子模块更新行为。Git 还支持合并行为，即将更新后子模块 commit id 对应的内容变化合并到子模块当前的工作树。合并操作可以通过 `git submodule update` 命令的 `--merge` 或 `--rebase` 来启用。

对于子模块而言，并不需要知道引用自己的主项目的存在。对于自身来讲，子模块就是一个完整的 Git 仓库，按照正常的 Git 代码管理规范操作即可。

对于主项目而言，子模块的内容发生变动时，通常有三种情况：子模块有未跟踪的内容变动、子模块有版本变化、子模块远程有更新

#### 子模块有未跟踪的内容变动

这种情况通常是直接修改子模块文件夹中的代码导致的，此时在主项目中使用 `git status` 能够看到关于子模块尚未暂存以备提交的变更，但是于主项目而言是无能为力的，使用 `git add/commit` 对其也不会产生影响。

```bash
➜ project-main git:(master) git status
位于分支 master
您的分支与上游分支 'origin/master' 一致。
尚未暂存以备提交的变更：
（使用 "git add <文件>..." 更新要提交的内容）
（使用 "git checkout -- <文件>..." 丢弃工作区的改动）
（提交或丢弃子模组中未跟踪或修改的内容）
修改： project-sub-1 (未跟踪的内容)
修改尚未加入提交（使用 "git add" 和/或 "git commit -a"）
```

在这种情况下，通常需要进入子模块文件夹中，在子模块内部的版本控制体系中提交代码，当提交完成后，主项目的状态就进入下面的情况。

#### 子模块有版本变化

当子模块版本变化时，在主项目中使用 `git status` 查看仓库状态时，会显示子模块有新的提交。

```bash
➜ project-main git:(master) ✗ git status
位于分支 master
您的分支与上游分支 'origin/master' 一致。
尚未暂存以备提交的变更：
（使用 "git add <文件>..." 更新要提交的内容）
（使用 "git checkout -- <文件>..." 丢弃工作区的改动）
修改： project-sub-1 (新提交)
修改尚未加入提交（使用 "git add" 和/或 "git commit -a"）
```
在这种情况下，可以使用 `git add/commit` 将其添加到主项目的代码提交中，实际的改动就是那个子模块文件所表示的版本信息。

```bash
git diff HEAD HEAD^
diff --git a/project-sub-1 b/project-sub-1
index ace9770..7097c48 160000
--- a/project-sub-1 ---a/project-sub-1
+++ b/project-sub-1
@@ -1 +1 @@
-Subproject commit ace977071f94f4f88935f9bb9a33ac0f8b4ba935
-子项目提交ace977071f94f4f88935f9bb9a33ac0f8b4ba935
+Subproject commit 7097c4887798b71cee360e99815f7dbd1aa17eb4
```

通常当子项目更新后，主项目修改其所依赖的版本时，会产生类似这种情景的 `commit` 提交信息。

#### 子模块远程有更新

通常来讲，主项目与子模块的开发不会恰好是同时进行的。通常是子模块负责维护自己的版本升级后，推送到远程仓库，并告知主项目可以更新对子模块的版本依赖。

之前曾经提到，主项目可以使用 `git submodule update` 更新子模块的代码，但那是指 `当前主项目文件夹下的子模块目录内容` 与 `当前主项目记录的子模块版本` 不一致时，会参考后者进行更新。

但如今这种情况下，后者 `当前主项目记录的子模块版本` 还没有变化，在主项目看来当前情况一切正常。

此时，需要让主项目主动进入子模块拉取新版代码，进行升级操作

```bash
git pull origin master
```

子模块目录下的代码版本会发生变化，转到情况2的流程进行主项目的提交。在拉取过程中，需要解决子模块仓库可能产生的合并冲突。

!!! note

    通常情况下，不建议在主项目仓库中直接对子模块仓库进行任何更改操作，而是应该在子模块自己的 Git 仓库中进行，最后再由主项目仓库通过子模块更新操作将子模块更改添加进来。

### 重复相同操作

`foreach` 子命令用于对每一个子模块重复相同的操作。例如，对所有子模块进行硬重置（hard reset）：

```bash
git submodule foreach 'git reset --hard'
```

### 删除子模块

对于 Git 1.8.5 以及之后的版本，使用 `deinit` 子命令能够以较少的步骤完成一个子模块的删除。

=== "Git >= 1.8.5"

    - 执行 `git submodule deinit <submod>` 。该命令执行以下动作：
        - 从 `.git/config` 文件删除对应的子模块配置信息
        - 清空子模块工作树。（删除工作树目录下的所有内容，但工作树目录本身尚未删除）
    - 执行 `git rm -f <submod>`。该命令执行以下动作：
        - 删除子模块工作树目录
        - 从主项目仓库 index 中删除子模块的 gitlink 条目。
        - 从 `.gitmodules` 文件中删除子模块相关的信息
    - 执行 `git commit` 向主项目仓库提交子模块更改。
    - （可选）在确定一个子模块不再使用后（并非临时禁用），执行 `rm -rf .git/modules/<submod>` 将子模块仓库数据从本地 Git 数据库中删除。

=== "Git <= 1.8.5"

    - 手动从 `.git/config` 文件中将子模块对应的配置信息删除。
    - 执行 `git rm --cached <submod>` 从主项目仓库 index 中删除子模块 gitlink 条目。
    - 执行 `rm -rf <submod>` 删除子模块工作树。
    - 手动从 `.gitmodules` 文件中将子模块对应的信息删除。
    - 执行 `git add .gitmodules` 将 `.gitmodules` 文件的更改暂存到 index。
    - 执行 `git commit` 向主项目仓库提交子模块更改。
    - （可选）在确定一个子模块不再使用后（并非临时禁用），执行 `rm -rf .git/modules/<submod>` 将子模块仓库数据从本地 Git 数据库中删除。

!!! warning "注意"

    - 如果子模块的工作树有本地更改，那么需要使用 `git submodule deinit --force` 进行强行删除。
    - 如果要一次性删除所有子模块，则使用 `git submodule deinit --all` 或 `git submodule deinit .`。
  
## 进一步理解子模块初始化

从上文中我们发现 `.gitmoduels` 和 `.git/config` 两个文件均记录了子模块的相关信息，那么它们的区别是什么呢？

`.gitmodules` 文件仅用来记录子模块的*名称*（name）和子模块的*路径*（path）的映射关系，这样我们就可以使用子模块的名称来操作子模块。`.git/config` 文件记录了所有已经初始化过并处于待更新状态的子模块。 执行 `git submodule add` 实际上是向 `.gitmodules` 文件添加子模块条目（entries），例如：

```bash
git submodule add --name foo -- https://example.com/foobar/foo.git libs/foo
```

该命令将 `submodule.foo.url` 的值设定未 `https://example.com/foobar/foo.git`，将 `submodule.foo.path` 的值设定为 `libs/foo`。

当项目主仓库提交或者 index 中有子模块记录时，执行 `git submodule init` 会先从 `.gitmodules` 文件中找到相关子模块的条目，并使用条目内容作为模板来更新 `.git/config` 文件（即向 `.git/config` 添加子模块相关信息。

针对包含子模块的项目做版本管理时，应当把 `.gitmodules` 文件纳入版本管理并推送到远程以分享给所有的项目参与者。而 `.git/config` 是每一位项目参与者自己的本地仓库设置，不会也不能推送到远程仓库。开发者通过克隆主项目仓库获取 `.gitmodules` 文件，并使用 `.gitmodules` 文件的内容在本地初始化子模块。

## 典型场景示例

### 工作流程1：子模块作为第三方库用于主项目

假设主项目远程仓库地址为 `https://example.com/demos/demo1.git`，该项目需要用到 4 个第三方库，它们的远程仓库分别是：

- `https://exmpale.com/demos/lib1.git`
- `https://exmpale.com/demos/lib2.git`
- `https://exmpale.com/demos/lib3.git`
- `https://exmpale.com/demos/lib4.git`

库作为子模块，它们的工作树放置到 `lib1`、`lib2`、`lib3` 和 `lib4` 4 个子目录中。

首先，我们先克隆主项目仓库：

```bash
git clone https://example.com/demos/demo1.git
```

并得到以下目录结构：

```
.git/
lib1/
lib2/
lib3/
lib4/
.gitmodules
code1.py
code2.py
code3.py
code4.py
```

其中，`.gitmodules` 文件的内容为：

```ini
[submodule "lib1"]
    path = lib1
    url = https://example.com/demos/lib1
[submodule "lib2"]
    path = lib2
    url = https://example.com/demos/lib2
[submodule "lib3"]
    path = lib3
    url = https://example.com/demos/lib3
[submodule "lib4"]
    path = lib4
    url = https://example.com/demos/lib4
```

注意，此时尚未对子模块进行初始化，本地没有任何子模块的实际内容。

现在我们想重构 `code1.py`，该模块仅使用 `lib1` 和 `lib2`，因此我们并不需要初始化 `lib3` 和 `lib4`。 于是，我们对 `lib1` 和 `lib2` 进行初始化：

```bash
git submodule init lib1 lib2
```

该命令会使用 `.gitmodules` 文件中的子模块信息向 `.git/config` 文件添加以下内容：

```ini
[submodule "lib1"]
    active = true
    url = https://example.com/demos/lib1
[submodule "lib2"]
    active = true
    url = https://example.com/demos/lib2
```

这意味着子模块 `lib1` 和 `lib2` 已经初始化完毕，但子模块的内容尚未写入工作树中。

最后我们通过 `git update` 将 `lib1` 和 `lib2` 的仓库数据下载到本，并把主项目仓库记录的 `lib1` 和 `lib2` 的子模块提交分别 checkout 到 `lib1/` 和 `lib2/` 中：

```bash
git submodule update
```

!!! note

    如果想要子模块中使用更新的提交，而不是使用主项目中记录的子模块提交，则使用 `git submodule update --remote` 把子模块远程跟踪分支的最新提交 checkout 到工作树。

### 工作流程2：子模块作为独立项目用于测试

考虑一个开发团队有若干个项目，它们均需使用一些逻辑相同的代码。于是，该团队创建了一个动态库项目，专门用来实现这些共用的功能。 在动态库的开发过程中，需要对动态库进行测试和验证，以确保它能够在所有依赖它的项目中正确工作。于是，可以将这些使用动态库的项目作为子模块添加到动态库仓库中。

假设动态库项目的目录结构如下：

```
.git/
sharedlib/
subproject1/
subproject2/
subproject3/
subproject4/
.gitmodules
```

其中，`.gitmodules` 文件的内容如下：

```ini
[submodule "subproject1"]
    path = subproject1
    url = https://example.com/demos/subproject1
[submodule "subproject2"]
    path = subproject2
    url = https://example.com/demos/subproject2
[submodule "subproject3"]
    path = subproject3
    url = https://example.com/demos/subproject3
[submodule "subproject4"]
    path = subproject4
    url = https://example.com/demos/subproject4
```

`subproject1` 到 `subproject4` 分别是 4 个子模块的工作树。

主项目（动态库）的源文件保存在目录 `sharedlib` 目录中。我们现在需要对 `subproject1` 和 `subproject2` 进行修改，并且这些修改的内容只会作用于 `subproject1` 和 `subproject2`。因此，我们只需要将 `subproject1` 和 `subproject2` 克隆到本地即可。

与工作流程1类似，我们先初始化 `subproject1` 和 `subproject2`：

```bash
git submodule init subproject1 subproject2
```

这条命令会向 `.git/config` 添加关于 `subproject1` 和 `subproject2` 的信息。

接着，我们将子模块的数据克隆到本地:

```bash
git submodule update --remote
```

这里使用 `--remote` 的原因是：在开发动态库的同时，这些用于测试的项目也可能已经由其他开发者进行了更新。我们要确保动态库能够在这些项目的最新版本中正常工作，就要获取这些项目最新提交，而不是主项目中记录的某个可能已经过时的提交。

当子模块有了更新后，我们还要使用 `git add` 和 `git commit` 将子模块的变更提交到主项目仓库中。