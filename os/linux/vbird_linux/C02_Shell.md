# 1. vi/vim编辑器

在Linux中, 绝大部分的配置文件都是以ASCII的纯文本文件形式存在, 因此利用文字编辑软件就能够修改设置

## 1.1 vi与vim

为何要学习vi/vim

* 所有UNIX-like系统都会内置vi文件编辑器
* 很多软件的编辑接口会主动调用vi
* vim具有程序编辑的能力, 而且编辑速度相当快速

vim与vi的区别

* vim可以看作vi的高级版本, 支持很多额外的功能

## 1.2~1.3 

请看其他资料

## 1.4 vim的注意事项

编码问题

1. Linux系统支持的语系数据: 终端命令`locale -a`用于查看, `localectl`用于设置
2. 终端的语系: 终端命令`locale`以查看相关的环境变量
3. 文本的编码
4. 打开终端的软件, 如终端模拟器

dos与unix系统的换行

* 两个终端命令`dos2unix`, `unix2dos`

语系编码转换

* 终端命令`iconv`

# 2. shell与bash

## 2.1 简介

### shell

* 是一种软件, 提供了一个可以访问内核服务的界面(interface)
  * 狭义的shell是指命令行方面的软件; 广义的shell包括图形用户界面模式的软件
  * 本章的shell是狭义的

* shell程序有
  * Bourne shell(sh), C shell(csh), K shell, TCSH
  * Bourne Again Shell(bash)-- Linux的默认shellcat

与shell有关的

* 文件`/etc/shells`: 记录了valid login shells
* 文件`/etc/passwd`: 记录用户的login shell, 即登录时获得的shell
* 命令`chsh`: 改变当前用户的login shell

### bash shell

bash: 目前Linux发行版的标准shell

* bash兼容于sh, 并且依据用户需求而加强的shell版本
* 主要优点
  * 历史命令: 
    * 本次登录所执行的命令被缓存在内存中, 前一次登录所执行的命令被记录在文件`~/.bash_history`中
  * 命令与文件补全
  * 命令别名: 内置命令`alias`
  * 任务管理(job control), 前台/后台控制
  * shell脚本
  * 支持通配符:

内置命令`type`: 

* 用于检查给定名称是否为命令名
* 若为命令, 可以指出其是否为`alias, keyword, function, builtin or file`

bash的快捷键

* `<Ctrl-u>`, `<Ctrl-k>`: 删除光标前/后的字符
* `<Ctrl-a>`, `<Ctrl-e>`: 光标移至最前/后面

## 2.2 shell的变量

### 基础使用

使用: `$VAR`, `${VAR}`

赋值: `VAR=VAL`

* 变量名未被赋值时, 默认值为空
* 等号两边不能直接接空格
* 变量名只能是英文字母与数字, 且开头字符不能是数字
* 可以使用反斜杠对字符进行转义, 使其变成一般字符

* 若有特殊字符时, 可以使用双引号或单引号; 如`VAR="VAL"`
  * 双引号的一些特殊字符将保持原本的含义, 如`$`
    * `var="lang is $LANG"`, 其值为`lang is zh_CN.UTF-8`
  * 单引号的字符都被当作一般字符(纯文本)
* 使用```cmd` ``(反引号)或`$(cmd)`形式: shell将会执行其中的`cmd`, 并原地替换成该命令的输出
* 可以自赋值, 如`PATH="$PATH":/home/bin`, `PATH=${PATH}:/home/bin`

* 惯例: 系统使用的变量名通常为全大写, 自定义变量可以使用小写字母

### shell变量展开

查看p332, p333

[gnu bash参考手册: shell形参展开](https://www.gnu.org/software/bash/manual/bash.html#Shell-Parameter-Expansion)

### 环境变量

环境变量, 也称为全局变量

* 与普通变量的区别: 子进程仅会继承父进程的环境变量, 不会继承普通变量
* 因此, 普通变量也被称为局部变量

### 常用的命令

| 命令                 | 意义/作用                                                    |
| -------------------- | ------------------------------------------------------------ |
| `set`                | 置位或复位shell的属性; 可以列出当前环境下的所有变量及其值    |
| `unset`              | 可以取消变量的值                                             |
| `export`             | 将普通变量转成环境变量, 或相反; 可以列出目前shell的所有环境变量及其值 |
| `env`                | 在一个被修改的环境中运行给定程序; 可以列出目前shell的所有环境变量及其值 |
| `read`               | 读取一行数据                                                 |
| `declare`, `typeset` | 1.为变量赋值, 给予属性; 2. 显示变量的值(和属性)              |
| `ulimit`             | 修改shell资源的限制                                          |

### 常见的变量

| 环境变量   | 意义/作用                                         |
| ---------- | ------------------------------------------------- |
| `HOME`     | 当前用户的根目录(或家目录)                        |
| `SHELL`    | 当前环境所使用的shell程序                         |
| `HISTSIZE` | 系统记录最近`HISTSIZE`条已执行过的命令            |
| `MAIL`     | `mail`命令使用该变量以读取邮箱文件(mailbox)       |
| `PATH`     | 执行文件的搜索路径; 目录之间用冒号分隔            |
| `LANG`     | 语系数据                                          |
| `RANDOM`   | 随机数; 可以通过该变量来获取随机数, 范围为0~32767 |
| `PS1`      | 命令行的主提示字符串, 表示shell可以读取一条命令   |
| `OSTYPE`   | 描述操作系统的字符串                              |
| `HOSTTYPE` | 描述主机类型的字符串                              |
| `MACHTYPE` | 描述系统类型的字符串                              |

| 系统变量 | 意义/作用              |
| -------- | ---------------------- |
| `$`      | 当前shell的进程号      |
| `?`      | 上一个执行命令的返回值 |

## 2.3 命令别名与历史命令

### 命令别名

内置命令`alias`

* 格式: `alias NAME='string'`
  * 执行`NAME`时, 等于执行`string`所表示的命令
* 格式: `alias`
  * 列出所有命令别名

内置命令`unalias`

* 格式: `unalias NAME`
  * 取消`NAME`命令别名

### 历史命令

历史命令的读取与记录

1. bash启动后, 系统从`$HISTFILE`所指定的文件中读取记录, 初始化历史列表
   * `$HISTFILE`所指定的文件中的行数由`$HISTFILESIZE`所确定
2. bash退出后, 将历史列表中的最近`$HISTSIZE`条记录写入到`$HISTFILE`文件中
   * 若启用`histappend`shell选项, 则这些记录会附加到`$HISTFILE`文件, 否则会覆盖
   * 若`$HISTFILE`未设置, 或所指定的文件不能写入, 则不会保存记录
3. 在保存历史后, `$HISTFILE`会被截断, 使其行数不超过`$HISTFILESIZE`
   * 若`$HISTFILESIZE`未设置, 或为空, 或为负值, 或为非数值, 则不会进行截断

相关的命令

* 内置命令`fc`: 可以用于列出或编辑和重新执行历史列表中的部分命令
* 内置命令`history`: 可以用于显式或修改历史列表, 和对历史记录文件进行操作

历史展开

* `!n`: 表示历史列表中第`n`行命令
* `!-n`: 表示历史列表中倒数第`n`行命令
* `!!`: 表示前一次执行的命令; 等价于`!-1`
* `!string`: 表示历史列表中, 在当前位置之前, 最近的以`string`开头的命令
* `!?string[?]`: 表示最近的历史列表中, 在当前位置之前, 最近的包含`string`的命令

## 2.4 bash shell的操作环境

搜索命令的顺序

1. 以相对/绝对路径执行命令, 如`./ls`, `bin/ls`
2. 由`alias`找到该命令来执行
3. 在内置命令中查找
4. 通过`$PATH`以查找第一个匹配的命令

bash的登录与欢迎信息

* `/etc/issue`: 在登录提示前需要打印的信息
* `/etc/issue.ent`: 提供给telnet的信息

### bash的环境配置文件

login shell与non-login shell

* login shell: 
  * 需要登录
  * 在调用login shell后, bash默认会读取配置文件
    * 该shell会读取并执行`/etc/profile`中的命令(若存在)
    * 然后按以下顺序读取执行第一个存在且可读的文件: `~/.bash_profile`, `~/.bash_login`, `~/.profile`
  * 在执行内置命令`exit`后, bash会读取并执行`~/.bash_logout`的命令(若存在)
* non-login shell
  * 不需要登录; 例如, 当你登录后, 想要以当前用户启动另一个shell, 则可以使用non-login shell
  * 若为interactive non-login shell, 在开始时默认会读取并执行以下两个文件(若存在):
    * `/etc/bash.bashrc`, `~/.bashrc`

命令`source`, `.`: 读取并执行配置文件的命令

* 格式: `source FILE`, `. FILE`

### 终端的环境设置

> 在登录真终端(tty1~tty6)时, 系统会自动获取终端输入环境的设置
>
> 多数LInux发行版都已经预设了不错的环境

关于终端设置

* 命令`stty`: 修改或查看真终端的设置, 按键
* 配置文件: `/etc/inputrc`, `/etc/DOR_COLORS*`, `/usr/share/terminfo/*`

bash的默认组合键

| 组合键     | 说明                       |
| ---------- | -------------------------- |
| `<Ctrl-c>` | 终止当前的命令             |
| `<Ctrl-d>` | 输入EOF                    |
| `<Ctrl-m>` | 回车                       |
| `<Ctrl-s>` | 暂停输出                   |
| `<Ctrl-q>` | 恢复输出                   |
| `<Ctrl-u>` | 在提示字符下, 删除整行命令 |
| `<Ctrl-z>` | 暂停当前的命令             |

### 通配符与特殊符号

见p344

## 2.5 重定向

命令的输入与输出

* 标准输入: 代码为0, 默认与键盘绑定
* 标准输出: 代码为1, 默认与屏幕绑定
* 标准错误输出: 代码为2, 默认与屏幕绑定

重定向: 是将命令的输入/输出绑定到给定的文件/设备

| 重定向符号   | 说明                                           |
| ------------ | ---------------------------------------------- |
| `NUM>`       | 将代码为`NUM`的输出以覆盖的方式重定向          |
| `NUM>>`      | 将代码为`NUM`的输出以附加(append)的方式重定向  |
| `NUM1>&NUM2` | 将代码为`NUM1`的输出重定向到代码为`NUM2`的输出 |
| `<`          | 将给定的文件/设备重定向到标准输入              |
| `<<`         | 将给定的字符串作为输入终止的标识               |

* `>`: 使用单独的`>`, 等价于`1>`
* `>>`: 使用单独的`>>`, 等价于`1>>`
* `&>`: 等价于`2>&1`

## 2.6 管道

管线pipeline/管道pipe

* 格式: `cmd1 | cmd2`
* 说明: 将`cmd1`的标准输出与`cmd2`的标准输入绑定

| 常用于管道的命令 | 说明                                                       |
| ---------------- | ---------------------------------------------------------- |
| `cut`            | 从每行输入中打印指定范围的内容                             |
| `grep`           | 打印与模式匹配的每行内容                                   |
| `sort`           | 根据当前语系进行排序                                       |
| `uniq`           | 若某行数据是连续重复, 则只显示一行数据                     |
| `wc`             | 为每行进行计数; 分别以newline字符, word, 字节为单位        |
| `tee`            | 从标准输入读取数据, 将数据写到标准输出和给定文件           |
| `tr`             | 从标准输入读取数据, 进行删除或替换字符后, 写到标准输出     |
| `col`            | 作用之一是, 将tab按键替换成空格键                          |
| `join`           | 作用之一是, 在两个文件中, 有相同数据的那一行, 才将它放一起 |
| `paste`          | 以行为单位, 将两个文件中的数据放在一行中                   |
| `expand`         | 将tab按键替换成空格键                                      |
| `split`          | 将文件划分为更小的文件                                     |
| `xargs`          | 参数替换; p358                                             |



# 3. 正则表达式与文件格式化处理

* `grep`
* `sed`
* `printf`
* `awk`
* `diff`, `patch`
* `cmp`

# 4. shell脚本

## 4.1 基础

shebang

* shell脚本的第一行, 且形如`#!/bin/bash`
* 作用: 声明该脚本所使用的shell名称

建议: 

* 在脚本的文件头处记录以下信息
  * 脚本的功能, 版本信息, 作者, 版权声明, 历史记录
* 脚本运行时需要的环境变量预先声明与设置
* 脚本内交特殊的命令, 使用绝对路径的方式来执行
* 在比较特殊的代码部分, 加以注释说明

脚本执行方式

* 直接执行: 在一个新的bash环境下执行脚本
  * 使用绝对/相对路径, 利用`${PATH}`搜索, 使用`bash`或`sh`
* `source`执行: 在当前bash下执行脚本
  * `source`的缩写是`.`

条件表达式

* `test`, 格式: `test [expr]`

* `[ ]`, 格式: `[ expr ]`
  * 需要空格以分隔表达式和`[]`
* `[[ ]]`, 格式: `[[ expr ]]`
  * 需要空格以分隔表达式和`[]`

## 4.2 条件与循环

if语句

```shell
if [ expr ]; then
	cmd
elif [ expr ]; then
	cmd
else
	cmd
fi
```

* 其中`expr`是条件表达式, `cmd`是命令块

case语句

```shell
case VAR in
 PATTERN) cmd ;;
 PATTERN) cmd ;;
esac
```

* 其中`var`是变量名, `PATTERN`是模式, `cmd`是命令块

函数

```shell
function NAME() {
	cmd
}
```

* `cmd`是命令块

while循环

```shell
while [ expr ]
do
	cmd
done
```

until循环

```bash
until [ expr ]
do
	cmd
done
```

for循环

```bash
for VAR in VAR_LIST
do
	cmd
done

for (( expr1; exp2; expr3 ))
do
	cmd
done
```

* `VAR_LIST`表示零个或多个名字