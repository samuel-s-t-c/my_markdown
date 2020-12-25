# 1.Git简介

Git的特性

* 直接记录快照
* 近乎所有操作都是本地执行
* 数据完整性: 所有数据在存储前都计算校验和
* git一般只添加数据

Git的简介

* 被纳入版本控制的数据有三种状态
  * 已修改(modified): 表示修改了文件, 但还没保存到数据库中
  * 已暂存(staged): 表示对一个已修改文件的当前版本做了标记, 使之包含在下次提交的快照中
  * 已提交(committed): 表示数据已经安全的保存在本地数据库中

* 三个工作区域
  * 工作目录(working directory): 对项目的某个版本独立提取出来的内容
  * 暂存区域(staging area): 一个文件, 保存了下次将提交的文件列表信息 , 一般在 Git 仓库目录中
  * git仓库(git directory/repository): 用来保存项目的元数据和对象数据库的地方

* 基本工作流程
  1. 在工作目录中修改文件
  2. 暂存文件, 将文件的快照放入暂存区域
  3. 提交更新, 找到暂存区域的文件, 将快照永久性存储到git仓库目录
* 

# 2.Git基础

获取帮助

```bash
$ git help <verb>
$ git <verb> --help
$ man git-<verb>
```

## 2.1Git配置

Git 自带一个 `git config` 的工具来帮助设置控制 Git 外观和行为的配置变量. 这些变量存储在三个不同的位置: 

1. `/etc/gitconfig` 文件: 包含系统上每一个用户及他们仓库的通用配置。 如果使用带有 `--system` 选项的 `git config` 时，它会从此文件读写配置变量。
2. `~/.gitconfig` 或 `~/.config/git/config` 文件：只针对当前用户。 可以传递 `--global` 选项让 Git 读写此文件。
3. 当前使用仓库的 Git 目录中的 `config` 文件（就是 `.git/config`）：针对该仓库。

例子:

* 设置用户名称与邮件地址

```bash
$ git config --global user.name "John Doe"
$ git config --global user.email johndoe@example.com
```

* 默认文本编辑器

```bash
$ git config --global core.editor emacs
```

* 检查配置信息

```bash
$ git config --list
$ git config <key>    # <key>表示某一个配置变量
```

## 2.2Git的基础命令

### 获取git仓库

* 在现有目录中初始化仓库: `git init`
* 克隆现有的仓库: `git clone [url [name]]`
  * 自动将其添加为远程仓库并默认以 “origin” 为简写
  * 自动设置本地的默认分支跟踪克隆的远程仓库的默认分支; 默认分支通常为master
  * Git 支持多种数据传输协议: https协议, git协议, ssh传输协议等

### 关于文件的操作

文件的状态变化周期

![lifecycle](assets/lifecycle.png)

* 检查当前文件状态: `git status`
  * `-s`, `--short`: 以简要的格式输出
* 追踪新文件(untracked->staged): `git add [file/dir] ...`
* 暂存已修改文件(modified->staged): `git add [file/dir] ...`
* 提交更新(staged->unmodified): `git commit`
  * `-m MESSAGE`: 直接添加本次提交的说明
  * `-a`: git将会自动把所有已经跟踪过的文件暂存起来一并提交
* 停止跟踪并移除文件(unmodified->untracked): `git rm <FILE>`
  * 默认情况: 从git仓库和工作目录中移除文件
  * `--cached`: 仅从git仓库中移除文件

忽略文件

* 创建一个名为`.gitignore`的文件, 在其中列出要忽略的文件名模式

* 格式规范
  * 所有空行或者以 `＃` 开头的行都会被 Git 忽略
  * 可以使用标准的 glob 模式匹配
  * 匹配模式可以以（`/`）开头防止递归
  * 匹配模式可以以（`/`）结尾指定目录
  * 要忽略指定模式以外的文件或目录，可以在模式前加上惊叹号（`!`）取反

文件差异`git diff`

* 不带参数直接使用`git diff`: 工作目录中当前文件和暂存区域快照之间的差异

* `git diff --cached`: 暂存区域快照和git仓库中上个版本的快照之间的差异
  * git 1.6.1及更高版本还允许使用`git diff --staged`

* 使用其他diff命令: `git difftool`, `git difftool --tool-help`

移动文件`git mv`

* 重命名: `git mv FILE1 FILE2`

### 查看提交历史

`git log`

* 不用任何参数(默认情况)
  * 按提交时间列出所有更新, 最近的更新排在最上面 
  * 列出每个提交的SHA-1校验和, 作者的名字和电子邮件地址, 提交时间以及提交说明

[git log的常用选项](https://www.progit.cn/#log_options)

* `-p`: 显示每次提交的内容差异
* `--stat`: 显示每次提交的简略统计信息

* `--pretty[=OPTION]`: 以不同于默认格式的方式展示提交历史; 有一些子选项
  * `oneline`: 每个提交放在一行显示
  * `format`: 自定义要显示的记录格式; 如`git log --pretty=format:"%h - %an, %ar : %s"`
    * [format的常用的选项](https://www.progit.cn/#pretty_format)

* `--graph`: 使用一个ASCII字符串来形象地展示提交历史

[git log的限制输出选项](https://www.progit.cn/#limit_options)

* `-<num>`: 仅显示最近`<num>`次提交

* 可以在`git log`选项的最后指定特定文件或目录的历史提交: 使用`--`分隔之前的选项和后面限定的路径名

* 例子

  ```console
  $ git log --pretty="%h - %s" --author=gitster --since="2008-10-01" \
     --before="2008-11-01" --no-merges -- t/
  ```

### 撤销操作

在上次提交操作中, 漏掉了几个文件没有添加, 或者提交信息写错了

* `git commit --amend`

取消暂存的文件

* `git rest HEAD <file> ...`

撤销对文件的修改

* `git checkout -- <file>...`

### 远程仓库的使用

查看远程仓库: `git remote`

* `-v`: 显示需要读写远程仓库使用的Git保存的简写与其对应的URL
* `git remote show [remote-name]`: 查看某一个远程仓库的更多信息

添加远程仓库: `git remote add <shortname> <url>`

* `<shortname>`: 给该远程仓库指定一个名字

从远程仓库中抓取与拉取

* `git fetch [remote-name]`: 从远程仓库中拉取所有你还没有的数据
  * 获得该远程仓库中所有分支的引用
  * 不会自动合并或修改本地仓库的当前工作

* `git pull`: 自动抓取对应的远程分支, 然后合并远程分支到当前分支
  * 前提: 该分支设置为跟踪一个远程分支

推送到远程仓库: `git push [remote-name] [branch-name]`

* 只有当你有所克隆服务器的写入权限, 并且之前没有人推送过时, 这条命令才能生效
* 当你和其他人在同一时间克隆, 他们先推送到上游然后你再推送到上游, 你的推送就会毫无疑问地被拒绝.  你必须先将他们的工作拉取下来并将其合并进你的工作后才能推送