bash

[toc]

# 1. 介绍

shell

* 基本上, shell是一个能够执行命令的宏处理软件
* Unix shell既是命令解释器, 也是一种编程语言

`bash` - GNU Bouren-Agaqin SHell

* `bash [options] [command_string | file]`

* `bash`是一个`sh`兼容的命令语言解释器; `bash`也包括`ksh`和`csh`中的一些有用特性
* `bash`符合*IEEE POSIX specification*的*Shell*和*Utilities*部分

## 选项

当*bash shell*被调用时, 在`set`(一个内置命令)的描述文档中的所有单字符*shell option*都可以用于`bash`.

此外, `bash`还有以下的单字符选项*option*

| Options                       | 描述 |
| ----------------------------- | ---- |
| `-c`                          |      |
| `-i`                          |      |
| `-l`                          |      |
| `-r`                          |      |
| `-s`                          |      |
| `-v`                          |      |
| `-x`                          |      |
| `-D`                          |      |
| `[-+]0` [<u>shopt_option</u>] |      |
| `--`                          |      |

`bash`也接受以下的多字符选项; 在命令行中, 这些选项必须在单字符选项前被识别

| Options                   | 描述 |
| ------------------------- | ---- |
| `--debugger`              |      |
| `--dump-po-strings`       |      |
| `--dump-strings`          |      |
| `--help`                  |      |
| `--init-file` <u>file</u> |      |
| `--rcfile` <u>file</u>    |      |
| `---login`                |      |
| `--noediting`             |      |
| `--noprofile`             |      |
| `--norc`                  |      |
| `--posix`                 |      |
| `--restricted`            |      |
| `--verbose`               |      |
| `--version`               |      |

## 其他参数

如果在处理完选项后还有其他参数, 且没有提供`-c`或`-s`选项, 

* 第一个参数被认为是一个含有*shell*命令的文件的名字.
  * `$0`被设为该文件名
* 其余的参数按位置依次赋予给对应的位置参数, 如`$1`, `$2`等

然后, *bash*试图在当前目录中打开该文件; 如果没有找到文件, 则在*PATH*的目录中寻找该脚本文件

接着, *bash*从该文件中读取并执行命令, 然后退出

* *bash*的退出状态是该脚本文件中的最后一条命令的退出状态
* 如果文件中没有命令, 则*bach*的退出状态为0

# 2. 关于本文档的一些定义说明

* *POSIX*: 基于unix的一套开源系统标准
* *blank*: 空格*space*或制表符*tab*
* *builtint*: 在shell内部实现的命令
* *metacharacter*: 用于分隔*word*的字符; 注意: 未被引用的*metacharacter*才能分隔*word*
  * `|` `&` `;` `(` `)` `<` `>` `<space>` `<tab>` `<newline>`
* *token*: 被shell认为是一个整体的字符串; 要么是*word*, 要么是*operator*
  * *word*: 不包含未带引号的*metacharacter*
  * *operator*: 至少包含一个未引用的*metacharacter*; 可分为*control operator*和*redirection operator*; 
    * *control operator*: 用于控制
      * `||` `&` `&&` `;` `;;` `;&` `;;&` `(` `)` `|` `|&` `<newline>`
    * *redirection operator*
* *field*: 一个文本单元, 是*shell expansions*的结果; 当展开后, 在执行命令时*field*可以用作命令名和参数
* *filename*: 用于标识文件的字符串
* *name* / *identifier*: 是指用作*shell*变量名和函数名的*word*
  * 以英文字母或下划线开头的, 只由英文字母, 数字和下划线组成
* *exit status* / *return status*: 命令执行后返回的值; 该值只有8位
* *job*: 是一种抽象, 表示一份需要完成的任务; 具体是指一系列进程, 包括pipeline和这些进程的子进程 

## 保留字/关键字

| Reserved Words |            |          |       |
| -------------- | ---------- | -------- | ----- |
| `!`            | `case`     | `coproc` | `do`  |
| `done`         | `elif`     | `esac`   | `fi`  |
| `for`          | `function` | `if`     | `in`  |
| `select`       | `then`     | `while`  | `{ }` |
| `time`         | `[[ ]]`    |          |       |

# 3. 基础*shell*特性

> 这里的*shell*, 是指*Bourne-Again SHell*, 也就是*bash*

* bash继承了所有Bourne shell的内置命令
* bash的运算和引用规则是来自POSIX规范

本章简要总结shell的基本要素

* 命令, 控制结构, shell函数, shell参数, shell展开, 重定向, 以及shell执行命令的过程

## 3.1 *Shell* 语法

3.1节主要说明: 你的输入, 对于shell来说是什么意思

* shell operation: shell的基本操作
* quoting: 如何移除某个字符的特殊含义
* comments: 如何进行注释

shell读取输入及其执行命令的简要流程

1. 若输入是注释的开头, 则shell忽略注释符号(`#`), 和该行的剩余部分
2. 否则, shell将会读入输入, 将其分为word和operator, 并使用引用规则以确定这些word和operator的含义
3. 然后, shell将这些token解析成命令和其他结构, 
   * 移除特定word或字符的特殊含义
   * 展开其他token
   * 根据需要, 重定向输入和输出
   * 执行指定的命令, 等待命令的退出状态
   * 返回退出状态

### 3.1.1 *Shell* 操作

当*shell*读取并执行命令时, 基本上会执行以下操作

1. 读取输入: 输入源可以是一个文件, `-c`选项所指定的字符串, 或者用户的终端
2. 将输入分成*word*和*operator*, 遵守引用规则
   * 这些*tokens*是被*metacharacters*(元字符)分隔
   * 此时, 也会进行别名展开
3. 将*tokens*解析为简单命令和复合命令
4. 执行各种*shell*展开; 将展开后的token解析为文件名, 命令和实参
5. 根据需要, 执行重定向, 然后从实参列表移除重定向运算符和其操作数
6. 执行命令
7. (可选)等待命令的完成并收集其退出状态

### 3.1.2 引用quoting

引用(*quoting*): 移除shell中特定字符或word的特殊含义

* 如保留字(*reserved word*), 元字符(*metacharacter*), 历史展开字符(*history expansion character*)

三种引用机制是

* 转义符
* 单引号
* 双引号

#### 2.1 转义符`\`

未被引用的反斜杠`\`, 是*bash*的转义符

* 作用: 保留下一个字符的字面值, 除了`<newline>`字符
* 例外: `\<newline>`被shell认为是用于行连续, 也就是从输入流中忽略并移除这两个字符

#### 2.2 单引号`'`

在一对单引号中的字符保留了其本来的字面值

* 单引号不能出现在一对单引号中, 即使有使用转义符`\`

#### 2.3 双引号`"`

在一对双引号中的字符保留了其本来的字面值

例外: 

* `$` `` ` `` `*` `@`
* `\`
*  如果`\`后跟以下字符之一, `\`在双引号中保留其特殊含义, 即作为转义符
    * `$`, `` ` ``, `"`, `\` , `<newline>`
  
* 其他情况, `\`只有字面意义, 因此这样的`\`不会被移除
* `!`
  * 当历史展开已启用时, 且*shell*未处于*POSIX*模式, `!`在双引号中保留特殊含义
  * 当*shell*处于*POSIX*模式, `!`在双引号中没有特殊含义, 即便启用历史展开
  * 注意: 若对`!`进行转义, 即`\!`, `\`不会被移除的

#### 2.4 ANSI-C Quoting

* 在形式如`$'string'`的*word*会被展开成字符串`'string'`,  其中的转义序列被替换成ANSI-C标准中对应的字符

* 展开后的结果依然是单引号引用的, 不过前面的`$`字符被移除

| `\`转义序列  | 描述                                                         |
| ------------ | ------------------------------------------------------------ |
| `\a`         | albert(bell)                                                 |
| `\b`         | backspace退格符                                              |
| `\e`, `\E`   | `<Esc>`字符                                                  |
| `\f`         | form feed换页符                                              |
| `\n`         | newline换行符                                                |
| `\r`         | carriage return回车符                                        |
| `\t`         | horizontal tab水平制表符                                     |
| `\v`         | vertical tab垂直制表符                                       |
| `\\`         | blackslash反斜杠                                             |
| `\'`         | single quote单引号                                           |
| `\"`         | double quote双引号                                           |
| `\?`         | question mack问号                                            |
| `\nnn`       | 一个八进制值为`nnn`的8位字符; `nnn`可以是一至三个八进制数字  |
| `\xHH`       | 一个十六进制值为`HH`的8位字符; `HH`可以是一个或两个十六进制数字 |
| `\uHHHH`     | 一个十六进制值为`HHHH`的*unicode*字符(ISO/IEC 10646); `HHHH`可以是一至四个十六进制数字 |
| `\uHHHHHHHH` | 一个十六进制值为`HHHHHHHH`的*unicode*字符(ISO/IEC 10646); `HHHHHHHH`可以是一至八个十六进制数字 |
| `\cx`        | 一个*control-x*字符                                          |

#### 2.5特定语系翻译Locale-Specific translation

形如`$"string"`的*word*根据当前语系进行展开

* 如果当前语系(*current locale*)是`C`或`POSIX`时, 则直接移除`$`字符
* 其他情况下, 根据*locale*进行翻译, 展开后的结果依然是双引号引用的, 不过前面的`$`字符被移除

*message catalog*

* 一些系统使用`LC_MESSAGES` *shell*变量以指定消息目录(*message catalog*)
* 一些系统使用`TEXTDOMAIN` *shell*变量以指定消息目录(文件可能有`.mo`后缀)
  * 此时, 需要将`TEXTDOMAINDIR` *shell*变量设置成消息目录文件的位置
* 还有一些系统同时使用以上的变量, 形如: `$TEXTDOMAINDIR/$LC_MESSAGES/LC_MESSAGES/$TEXTDOMAIN.mo`

### 3.1.3 注释

* shell会忽略以`#`开头的word, 和在同一行中`#`后的剩余字

* 条件: 
  * 非交互式(*non-interactive*) *shell*,
  * 在交互式(*interactive*) *shell*中, 启用`shopt`内置命令中`interactive_comments`选项(默认使能)

## 3.2 *Shell* 命令

介绍命令的类型

* 简单命令: 形式如`cmd arg1 arg2`
* 其他复杂命令是由简单命令和其他结构组成的

### 3.2.1 简单命令

简单命令: 一连串由blans分隔的word, 以*control operator*作为结尾

* 第一个*word*指定要执行的命令, 并传递给`$0`(参数零, argument zero); 剩余的*word*是作为命令的实参

* 简单命令的返回状态是
  * 给定命令的退出状态: 由POSIX 1003.1的函数`waitpid`返回
  * 信号`n` 加上128: 若命令被信号`n`终止

### 3.2.2 管线Pipeline

管线, 是指由控制运算符`|`或`|&`分隔的一个或多个命令

* 格式: ` [time [-p]] [!] command1 [ | or |& command2] ...`
* 作用: 在管线中每个命令的输出都通过一个*pipe*(管道)连接到下一个命令的输入
  * 也就是说, 上一个命令的输出是下一个命令的输入
* 管线中的连接是在该命令的任意重定向前执行

`|`与`|&`

* 使用`|`时, `command1`的标准输出通过一个*pipe*连接到`command2`的标准输入
* `|&`: 表示`command1`的标准输出和标准错误通过一个*pipe*连接到`command2`的标准输入
  * 其实, `|&` 是 `2>&1 |` 的缩写
  * 注意: 在该命令的任一显式重定向都完成后, 才会进行从标准错误到标准输出的隐式重定向

管线的返回状态: 是管线的最后一个命令的退出状态
* 若启用`pipefail`选项, 
  * 如果管线有某些命令以非零状态退出, 则管线的返回状态是最后一个非零退出状态
  * 如果所有命令都成功退出, 则管线的返回状态是零
* 如果有`!`保留字, 则管线的返回状态是上述的退出状态的逻辑反

`time`保留词

* 作用: 若有`time`保留字, 当管道终止时, 报告此次执行中所消耗的经过时间, 用户时间和系统时间
  * `-p`, 将输出格式改为*POSIX*指定的格式
  * 当*bash shell*处于*posix*模式时, 
    * `time`可以后接一个*newline*字符; 这样的话, shell会显示它自己及其子进程所消耗的总用户时间和总系统时间
    * 如果下一个*token*以`-`开始, *bash shell*不能将`time`识别为保留字
  * 可使用`TIMEFORMAT`变量设为一个格式字符串, 以指定如何显示时间信息

其他说明
* 如果管线不是异步执行, *shell*等待管线中的所有命令终止
* 管线中的每个命令都在其*subshell*(一个独立的进程)中执行
  * 如果启用`shopt`的`lastpipe`选项, 管道的最后一个命令由*shell*运行

### 3.2.3 命令列表

列表(list), 是指一个或多个管线, 且由`; & && ||`等运算符之一分隔, 可选地以`;  &  <newline>`等运算符之一结束

*  `&&`和`||`拥有相同的优先级, `;`和`&`拥有相同的优先级; `&&`和`||`的优先级高于`;`和`&`
* 在列表中可以出现一个或多个`newline`来分隔命令

说明

* 某个命令以`&`结束/分隔
  * *shell*会在一个*subshell*中异步执行该命令, 并立即返回一个为0的状态
    * 这被称为在*background*执行命令, 或者执行异步命令
  * 如果未使能*job control*, 且没有显式重定向, 那么异步命令的标准输入被重定向到`/dev/null`文件
* 某个命令以`;`结束/分隔
  * 顺序执行命令; *shell*依次等待每个命令的终止
* 返回状态是最后一个执行命令的返回状态
* 以`&&`分隔的列表`command1 && command2`, 被称为"与列表"(AND list)
  * 从左向右结合
  * 当`command1`返回一个为0的退出状态时, 才执行`command2`
* 返回状态是最后执行的命令的退出状态
* 以`||`分隔的列表`command1 || command2`,  被称为"或列表"(OR list)
  * 从左向右结合
  * 当`command1`返回一个不为0的退出状态时, 才执行`command2`
  * 返回状态是最后执行的命令的退出状态

### 3.2.4 复合命令Compound Command

复合命令是*shell*编程语言的结构

* 每种结构以一个保留字或控制运算符开始, 以一个对应的保留字或运算符结束
* 与复合命令关联的重定向将会应用到其中的每个命令, 除非显式覆盖重定向

* 绝大多数情况下, 在复合命令中的命令被一个或多个`<newline>`分隔, 可以使用`<newline>`代替分号

三种复合命令结构

* 循环结构
* 条件结构
* 命令组

#### 3.2.4.1 循环结构

> 以下结构中的`;`可以被一个或多个`<newline>`替换

`until`

* 格式: `until test-list; do consequent-list; done`

* 只要`test-lists`的退出状态不为0, 一直执行`consequent-list`
* 返回状态是在`consequent-list`中最后一个执行命令的退出状态; 如果没有执行任意`consequent-list`的命令,  返回状态为0

`while`

* 格式: `while test-list; do consequent-list; done`

* 只要`test-list`的退出状态为0, 一直执行`consequent-list`
* 返回状态是在`consequent-list`中最后一个执行命令的退出状态; 如果没有执行任意`consequent-list`的命令,  返回状态为0

`for`

* 格式: `for name [ [in [words …] ] ; ] do list; done`
  * 展开*words*, 然后依次为结果列表中的每个成员绑定到`name`并执行`list`
  * 如果没有`in words`, 则默认变为`in $@`, 为每个已设置的位置形参执行`list`
  * 返回状态是最后一个执行命令的退出状态; 如果*words*中没有成员,  且没有执行`list`, 此时返回状态为0

* 格式: `for (( expr1 ; expr2 ; expr3 )) ; do list ; done`
  * 首先计算算术表达式`expr1`; 然后重复执行算术表达式`expr2`直到它返回0
  * 每次`expr2`返回非零值时, 会执行`list`, 然后执行算术表达式`expr3`
  * 如果有任一算术表达式是省略的, 其位置的值被设为1
  * 返回状态是`list`中最后一次执行命令的退出状态; 如果任一表达式都是无效的, 返回false

`break`和`continue`内置命令可用于控制循环的执行

#### 3.2.4.2 条件结构

`if`

```shell
if test-list1; then
  consequent-list1;
[elif test-list2; then
  consequent-list2;]
[else consequent-list3;]
fi
```

* 过程
  * 执行`test-list1`; 
  * 如果返回状态为0, 执行`consequent-list1`, 并结束`if`命令
  * 如果返回非零状态, 
    * 若存在`elif`时, 则依次执行`elif`分支; 若某个`elif`的测试返回状态为0, 执行对应的`consequent-list`, 并结束`if`命令
    * 若存在`else`时, 且(若存在的话)`elif`分支的测试返回状态都不为0, 则执行`else`的`consequent-list`, 并结束`if`命令
* 返回状态是最后执行`consequent-list`命令的退出状态; 若所有条件测试都不为0时, 返回0

`case`

```shell
case word in
    [ [(] pattern [| pattern]…) command-list ;;]
    ...
esac
```

* 过程

  * 首先将`word`和`pattern`进行展开

    * `word`的展开: *tilde expansion*, *parameter expansion*, *command substitution*, *arithmetic expansion*, 和*quote removal*

    *  `pattern`的展开: *tilde expansion*, *parameter expansion*, *command substitution*, 和*arithmetic expansion*

  * 然后将`pattern`与`word`匹配; 如果匹配成功, 则执行对应的`command-list`
    
    * 若启用`nocasematch`选项, 则模式匹配不会考虑字母的大小写

* 常见做法: 使用`*`作为最后子句的`pattern`, 以定义默认情况
* *clause*/子句: 由多个`pattern`和对应的`command-list`组成
  * 可以有任意个子句
  * 每条子句的结尾必须是`;;`, `;&`或`;;&`
    * 以`;;`结尾: 表示不会再进行其他的匹配
    * 以`;&`结尾: 表示若存在下一条子句, 则执行下一条子句的`command-list`
    * 以`;;&`结尾: 表示若存在下一条子句, 则对后面的子句进行匹配直至匹配成功或者没有子句; 如果后随的匹配成功, 则执行对应的`command-list`

* 返回状态: 
  * 如果没有与任一`pattern`匹配成功, 则返回值为0的状态
  * 否则, 返回执行的`commad-list`的退出状态

`select`

```shell
select name [in words ...]; do list ; done
```

* 过程
  * 首先, 对`words`进行展开; 
  * 然后, 将展开后的`word`列表在标准错误输出流上打印, 在每个输出前添加一个数字
    * 如果省略了`in word`, 则打印位置形参, 就像是指定了`in "$@"`
  * 然后显示*PS3*提示, 并从标准输入中读取一行
    * 若读取的输入行中包含一个与某个`word`相对应的数字, 将`name`设置为该`word`
    * 若读取的输入行不为空, 但没有一个相对应的数字, 则将`name`设置为null.
    * 若输入行为空的, 重新显示`word`列表和提示
    * 若读取到*EOF*, 则结束select*结构
  * 注意: 读取的输入行被保存在`REPLY`变量中
* 每次输入后, 执行`list`并重复上一步; 可以使用`break`或*EOF*跳出*select*结构

`(( expression ))`: 等价于`let "expression"`

* 作用: 根据算术计算的规则, 计算算术表达式`expression`
* 返回状态: 如果表达式的值不为0, 则返回状态为0, 否则为1

`[[ expression ]]`

* 作用: 根据条件表达式`expression`的值来返回状态0或1
  * 条件表达式`expression`是由*primaries*组成
  * 条件运算符, 如`-f`, 必须是未引用的, 才会被认为是*primaries*
* 在`[[ ]]`中, 
  * 不会进行: *word splitting*和*pathname expansion*
  * 可以进行: *tilde expansion*, *parameter and variable expansion*, *arithmetic expansion*, *command substitution*, *process substitution*, 和*quote removal*

条件运算符

* `<`和`>`运算符, 如果与`[[`一起使用时, 将会使用当前语系进行字典排序
* 当使用`==`和`!=`运算符时, 运算符右侧的字符串被当作模式, 并进行模式匹配; (就像是使能`extglob`选项那样)
  * `==`, `=`: 模式匹配时返回0, 否则返回1
    * `==`和`=`这两个运算符是相同的
  * `!=`: 模式不匹配时返回0, 否则返回1
  * 可以启用`nocasematch`选项以不考虑字母的大小写
  * 模式的任意部分都可以被引用, 使得被引用部分作为字符串来进行匹配
* `=~`二元运算符, 与`==`和`!=`运算符拥有相同的优先级; 当使用`=~`时, 运算符右侧的字符串被当作*POSIX*扩展正则表达式, 并像*regex3*那样进行匹配
  * 如果有字符串与模式匹配, 则返回0, 否则返回1.
  * 如果正则表达式本身是错误的, 则返回2
  * 可以启用`nocasematch`选项以不考虑字母的大小写
  * 模式的任意部分都可以被引用, 使得被引用部分作为字符串来进行匹配
  * 若模式保存在一个*shell*变量中; 对该变量进行引用, 使得模式作为字符串进行匹配
  * 数组变量`BASH_REMATCH`保存与正则表达式中的括号子表达式匹配的子字符串
    * `BASH_REMATCH[0]`是与整个正则表达式匹配的字符串的匹配部分
    * 其他索引*n*的元素是与第n个括号子表达式匹配的字符串的匹配部分

表达式可以使用以下运算符进行结合, 以优先级由高到低列出

* `( expression )`
  * 返回`expression`的值; 用于覆盖正常的运算符优先级
* `! expression`
  * 如果`expression`为false, 返回true
* `expression1 && expression2`
  * 如果两个表达式为true, 返回true
  * 如果`expression1`为false, 则返回false, 不会对`expression2`进行求值
* `expresssion1 || expression2`
  * 如果两个表达式中有一个为true, 返回true
  * 如果`expression1`为true, 则返回true, 不会对`expression2`进行求值

#### 3.2.4.3 命令组

*bash*提供两种组合命令列表的方式, 使命令列表作为整体单元执行

* 若对命令组使用重定向, 则对命令组中的所有命令进行重定向

`( list )`

* 在一个*subshell*环境中执行`list`
* 返回状态是`list`的退出状态

`{ list; }`, 被称为*group command*

* 在当前*shell*环境中执行`list`
* 返回状态是`list`的退出状态
* 由于历史原因, 元字符`{ }`是保留字
  * `list`与`{}`之间必须以blank或其他元字符分隔
  * `list`必须以`;`结束

### 3.2.5 coprocesses

一个*coprocess*是一条带有`coproc`保留词的*shell*命令

* 协程在*subshell*中异步执行, 带有一个连接当前*shell*和*coprocess*的双向*pipe*

格式: `coproc [NAME] command [redirections]`

* 创建一个名为*NAME*的协程;
  * 如果*command*是简单命令, 则不能提供*NAME*, 否则会将*NAME*解释为简单命令的第一个*word*
  * 其他情况下, 可以提供*NAME, 也可以不提供*NAME
  * 若没提供*NAME*, 默认名字为`COPROC`

当执行*coprocess*时, 在当前*shell*中创建一个名为*NAME*的数组变量;

* *NAME*`[0]`是一个文件描述符, 通过一个*pipe*与*command*的标准输出连接

* *NAME*`[1]`是一个文件描述符, 通过一个*pipe*与*command*的标准输入连接
* 该管道在任意给定重定向之前建立
* 这些文件描述符可以用作*shell*命令和重定向的参数, 进行标准*word expansions*
* 这些文件描述符在*subshell*中不可用

其他说明:

* 用于执行*coprocess*的*subshell*的进程ID被赋予变量*NAME*`_PID`中
* 由于*coprocess*是异步执行的, `coproc`命令总是返回成功
* *coprocess*的返回状态是*command*的退出状态

### 3.2.6 GNU Parallel

非bash内置的并行运行命令的方式之一: GNU Parallel

[a few examples for GNU Parellel](https://www.gnu.org/software/bash/manual/html_node/GNU-Parallel.html#GNU-Parallel)

[GNU Parellel](https://www.gnu.org/software/parallel/)

## 3.3 *Shell* 函数

*shell*函数的定义语法

```shell
name () compound-command [ redirections ]
function name [()] compound-command [ redirections ]
```

* 如果使用保留字`function`, 且没有提供圆括号, 则*compound-command*必须是`{ list; }`
  * 由于历史原因, 包围函数体的花括号与函数体必须使用blank或`<newline>`分隔开来; 而且list必须以分号, `&`或`<newline>`结束
* 在执行函数时, 才会执行对应的重定向
* 当*shell*处于*POSIX*模式时, 函数名不能与特殊内置命令同名

返回状态

* 函数定义的返回状态为0, 除非语法错误或存在一个同名的只读函数
* 执行函数的返回状态是其中的最后一个执行命令的退出状态

执行*shell*函数

* 像简单命令那样使用*shell*函数名
*  *shell*函数在当前*shell*上下文中执行

* 在函数执行期间, 函数的实参传递给位置形参; 结束执行后, 位置形参恢复成原来的值
  * 特殊形参`#`表示位置形参的数量; 执行函数时, 变成函数形参的数量; 函数结束后, 恢复为原来的值
  * 特殊形参`0`没有改变
* *shell*变量`FUNCNAME`的第一个元素被设为正在执行的函数的名字

函数执行环境

* 除了两个例外, 函数执行环境的其他方面都与其*caller*的环境相同
* `DEBUG`和`RETURN` trap不会被继承, 除非使用`declare`内置命令为函数赋予`trace`属性, 或者使用`set`启用`-o functrace`选项
* `ERR` trap不会被继承, 除非启用`-o errtrace` *shell*选项

关于函数的变量

* `FUNCNEST`变量: 其值大于0时, 定义了最大函数嵌套层数; 如果函数调用超过该限制, 将会中止整个命令

关于函数的内置命令

* `return`内置命令
  * 在函数内执行`return`, 则完成该函数, 恢复执行到函数调用后面的命令
  * 如果`return`命令有一个数值实参, 则该数值为函数的返回状态; 不然的话, 返回状态是在`return`前的最后执行命令的退出状态
  * 在恢复执行前, 将会执行与`RETURN` trap关联的命令
* `local`内置命令
  * 函数的局部变量可以使用`local`来声明
  * 局部变量会遮蔽在之前的作用域中的同名变量
  * *shell*使用动态作用域来控制函数中变量的可见性
* `unset`内置命令
  * 使用`-f`选项可以删除函数的定义
  * 在函数中, `unset`也使用动态作用域, 以取消变量定义
* `declare`与`typeset`
  * 使用`-f`选项, 可以列出函数的名字和定义
  * 使用`-F`选项, 列出函数的名字; 若启用`extdebug`*shell*选项, 还会列出源文件和行数
* `export`
  * 使用`-f`选项, 可以导出函数, 使得*subshell*自动定义对应的函数

## 3.4 *Shell Parameter*

*shell*形参是一个保存值的实体, 可以是一个*name*, 一个数字或者某个特殊字符

* 当*name*, 数字或者特殊字符被赋值时, 将会设置一个形参
* 空字符串也是一个有效值

变量是一个由*name*表示的形参

* 有一个值和零个或多个属性(使用`declare`赋予属性)

* 赋值语句的形式: `name=[value]`
  * 若不提供*value*;, 变量被赋予一个空字符串
  * *value*在赋值前将进行: tilde expansion, parameter and variable expansion, command substitution, arithmetic expansion, and quote removal
    * 例外: 变量`@`还会执行word splitting
  * 赋值语句可以作为`alias`, `declare`, `typeset`, `export`, `readonly`, `local`等内置命令的实参
  * (?)在*POSIX*模式中, 在这些内置命令的前面可以有一个或多个`command`内置命令, 以保留这个赋值语句的属性
* 可以使用`unset`复位一个变量, 即取消变量
* 赋值语句为某个变量赋值时,  也可以使用`+=`运算符, 以追加值; 
    * 在`declare`等等内置命令的赋值语句中也能使用`+=`
* 使用`declare`或`local`的`-n`选项创建一个有*nameref*属性的变量, 即指向另一个变量的引用
  * `for`循环的控制变量可以有*nameref*属性
  * 数组变量不能有*nameref*属性; 但*nameref*变量是数组变量的引用
  * `unset`的`-n`选项作用于引用本身以取消*nameref*属性, 其他选项作用于引用指向的变量

### 3.4.1 位置形参

位置形参, 是指一个由一位或多位数字表示的形参, 除了单独一个数字`0`外.

* 当shell被调用时, *shell*实参是赋值于对应的位置形参
  * 不能使用赋值语句对位置形参进行赋值
  * 使用`set`和`shift`内置命令可以设置和取消位置形参
* 对于位置形参`N`, 可以以`${N}`形式引用该位置形参; 特别的是, 如果`N`是一位数字, 可以使用`$N`形式
* 当执行*shell*函数时, 位置形参会暂时被函数的实参替换

### 3.4.2 特殊形参

特殊形参只能被引用; 不允许对其赋值

* `*`
  * `$*`展开成位置形参, 从位置形参1开始
  * 当展开不是在双括号中, 每个位置形参展开成一个被分隔的word; 然后每个*word*会进一步执行word splitting和pathname expansion.
  * 当展开在一对双括号中, 它展开成一个*word*; 该word由每个位置形参的值组成, 被特殊变量`IFS`的第一个字符分隔
    * 如果`IFS`未设置, 位置形参的值被空格分隔; 如果`IFS`为空,  位置形参的值之间没有分隔符

* `@`
  * `$@`展开成位置形参, 从位置形参1开始
  * 在执行*word spliting*的上下文中, 它展开成多个被分隔的*word*
    * 如果不在一对双引号中, 这些*word*进行*word splitting*
  * 在不执行*word splitting*的上下文中, 它展开成单独的*word*, 其中是被空格分隔的位置形参的值
* `#`
  * `$#`展开成位置形参的数量, 十进制
* `?`
  * `$?`展开成最近执行的*foreground*管道的退出状态
* `-`
  * `$-`展开成当前的调用选项标志; 由`set`或*shell*选项所设置
* `$`
  * `$$`展开成当前*shell*的进程ID; 若在由`()`所导致的*subshell*中, `$$`展开成调用该*subshell*的父*shell*
* `!`
  * `$!`展开成最近放入*background*的*job*的进程ID
* `0`
  * `$0`展开成*shell*或*shell*脚本的名字
  * 该形参在*shell*初始化时设置的
* `_`
  * 在*shell*启动时, 该形参被设置成当前正在执行的*shell*或*shell*脚本的绝对路径名, 并在环境列表或实参列表中传递该形参
  * 随后, `$_`展开成在*foreground*执行的前一个简单命令的最后一个实参
  * 执行每个命令时, 该形参被设置成调用该命令的的完整路径, 并将该形参导出到该命令的环境中
  * 当检查邮件时, 该形参是邮件文件的名字

## 3.5 *Shell* 展开

在命令行被分成多个*token*后, 执行*shell*展开; 有以下几种展开

* *brace expansion* / 花括号展开
* *tilde expansion* / 波浪号展开
* *parameter and variable expansion* / 形参和变量展开
* *command substitution* / 命令替换
* *arithmetic expansion* / 算术展开
* *word splitting* / 单词分割
* *filename expansion* / 文件名展开

执行展开的顺序是

1. *brace expansion*
2. (从左向右执行) *tilde expansion*, *parameter and variable expansion*, *arithmetic expansion*,  *command substitution*
   * 某些系统支持*process substitution* , 与第2次序的展开同时执行
3. *word splitting*
4. *filename expansion*
5. *quote removal*

### 3.5.1 Brace Expansion

用于*brace expansion*的模式: `[preabmle]{...}[postscript]`

* 可选的*preamble*(前言), 后接一对`{}`, 后跟可选的*postscript*(后语)
* 在花括号中, 要么是一系列逗号分隔的字符串, 要么是一个序列表达式(*sequence expression*)

逗号分隔的字符串

* 逗号分隔的字符串进行*brace expansion*后, 为每个字符串在前面添加 *preabmle*, 在结尾添加*postscript*

```shell
bash$ echo a{d,c,b}e
ade ace abe
```

序列表达式

* 形式是`{x..y[..incr]}`
  * *x*和*y*要么是整数, 要么是单个字符
  * *incr*表示可选的递增量, 是一个整数
* 序列表达式进行*brace expansion*h后, 
  * 若提供了整数, 则展开成*x*和*y*所表示的范围中的所有整数, 包括*x*和*y*本身
    * 若提供的整数有`0`前缀, 则输出的每个整数拥有相同的宽度, 宽度是最宽的带有`0`前缀的整数的宽度, 如需要进行零填充
  * 若提供了字符, 则展开成*x*和*y*所表示的范围中的所有字符, 包括*x*和*y*本身
    * 默认使用*C locale*的顺序
  * 若提供*incr*, 则其中的每个字符/整数的差是*incr*; 默认情况下, *incr*根据需要选择`1`或`-1`

```shell
bash$ echo {1..10..2}
1 3 5 7 9
bash$ echo {a..c}
a b c
bash$ echo {c..a}
c b a
```

#### 1.1说明/注意

* *shell*首先执行*brace expansion*; 

* 对于其他展开中的特殊字符, *brace expansion*不会对其有任何特殊处理;
* *brace expansion*完全是基于字符的处理; 不会对括号中的内容进行任何的语义解释

* 格式正确的*brace expansion*必须包括一对未引用的`{}`; 在花括号中至少有一个未引用的逗号或一个有效的序列表达式;
  * 如果.模式的格式不正确, 不会对其进行*brace expansion*, 该模式将保持不变
* 为了避免与*parameter expansion*发生冲突, 字符串`${`在*brace expansion*是格式不正确的, 并且会抑制*brace expansion*直至遇到一个`}`

### 3.5.2 Tilde Expansion

从未引用的波浪号开始到第一个未引用的斜杠, 被认为是波浪号前缀(*tilde-prefix*)

* 波浪号前缀不包括第一个未引用的斜杠
* 如果没有未引用的斜杠, 则波浪号后面的字符都属于波浪号

若波浪号前缀中的字符没有是被引用的, 其中的*word*被认为是登录名(*login name*), 并将波浪号前缀替换成与指定的登录名相关联的主目录(*home directory*)

* 若*word*是一个空字符串, 即`~`, 那么将波浪号前缀替换成`HOME` *shell*变量的值

  * 若*HOME*变量未设置, 则将波浪号前缀替换成执行该*shell*的用户的主目录(*home directory*)

* 若*word*是一个加号, 即`~+`, 那么将波浪号前缀替换成*PWD* *shell*变量的值

* 若*word*是一个减号, 即`~-`, 那么将波浪号前缀替换成*OLDPWD* *shell*变量的值(如果有的话)

  * 如果未设置*OLDPWD*变量, 则*tilde expansion*失败

* 若*word*是可选的加号或减号后接一个数字`N`, 如`~+N`或`~N`, 那么将波浪号前缀替换成目录栈(*directory stack*)中的对应元素, 就像`dirs`内置命令使用波浪号后的字符作为参数那样

  * 若没有提供加号或减号, 将隐式添加一个加号
  * 若`N`大于目录栈的目录树, 则*tilde expansion*失败

* | 示例        | 说明                              |
  | ----------- | --------------------------------- |
  | `~`         | `$HOME`的值                       |
  | `~/foo`     | `$HOME/foo`                       |
  | `~fred/foo` | 用户`fred`的主目录中的`foo`子目录 |
  | `~+/foo`    | `$PWD/foo`                        |
  | `~-/foo`    | `${OLDPWD-'~-'}/foo`              |
  | `~N`        | `dirs +N`显示的字符串             |
  | `~+N`       | `dirs +N`显示的字符串             |
  | `~-N`       | `dirs -N`显示的字符串             |

#### 说明

* 若登录名无效或*tilde expansion*失败, 则整个字符串保持不变

#### 变量赋值与*tilde expansion*

每次变量赋值中, *bash*会检查在每个`:`或第一个`=`后的未引用波浪号前缀

* 若有未引用的波浪号前缀, 则执行*tilde expansion*

### 3.5.3 Shell Parameter Expansion

> 使用`$`字符可以引入*parameter expansion*, *command substitution*或*arthmetic expansion*

*parameter expansion*的基本形式: `${parameter}`

* *parameter*是需要展开的符号, 它可以是*shell*形参或数组引用
* 花括号是可选的, 除了一些例外
  * 当使用`{`时, *bash*匹配对应的`}`; 此时*bash*不会考虑: 被`\`转义的`}`, 在引用字符串中的`}`, 嵌套的*arithmetci expansion* / *command substitution* / *parameter expansion*中的`}`
  * 例外: *parameter*是多位数字的位置形参

*parameter expansion*的*indirect expansion*形式

* 形式: `${!parameter}`
* 如果*parameter*不是*nameref*: 
  * 首先对*parameter*进行展开, 得到*parameter*展开后的值
    * *parameter*可以执行:  *tilde expansion*, *parameter expansion*, *command substitution*, 和*arithmetic expansion*
  * 然后展开得到的值
* 如果*parameter*是一个*nameref*, 则展开成*parameter*指向的变量的名字

#### *parameter expansion*的更多形式

以下形式中的*word*都会受这些展开的影响: *tilde expansion*, *parameter expansion*, *command substitution*, 和*arithmetic expansion*

| 形式                 | 说明                                                         |
| -------------------- | ------------------------------------------------------------ |
| `${parameter:-word}` | 使用默认值; 若*parametr*未设置或者为空, 返回*word*的展开; 否则返回*parameter*的值 |
| `${parameter:=word}` | 赋予默认值; 若*parametr*未设置或者为空, *word*的展开被赋值给*parameter*; 否则返回*parameter*的值. 位置形参和特殊形参不能以这种方式赋值 |
| `${parameter:?word}` | 显示错误; 若*parametr*未设置或者为空, *word*的展开输出到标准错误, 且*shell*如果为非交互式则退出; 否则返回*parameter*的值. 如果*word*为空, 则输出相关信息 |
| `${parameter:+word}` | 使用备用值; 若*parametr*未设置或者为空, 返回*null*; 否则返回*word*的展开 |

* *bash*会检查形参的存在性和其值是否为*null*; 可以省略其中的`:`, 这样的话*bash*只检查形参的存在性, 即是否设置了该形参, 不检查其值是否为空

substring expansion形式

* 形式: `${parameter:offset}` / `${parameter:offset:length}` 
* 它展开成*parameter*值的子字符串, 具体是从*offset*指定的字符开始计数, 而且最多*length*个字符; 若没有指定*length*, 则直到结尾  
* *offset*和*length*是算术表达式 ; 若`offset`的值小于0, 则是从尾部向头部确定偏移字符; 若*length*的值小于0, 则被解释成从结尾开始的偏移字符, 结果不包括该字符; 
* 特殊情况: 
  * 若*parameter*是`@`, 结果是从*offset*开始的*length*个位置形参; 若*offset*为负, 则是从最大的位置形参开始确定偏移, 如`-1`偏移就是最后一个位置形参; 若*length*为负, 是*expansion error* 
  * 若*parameter*是下标为`@`或`*`的索引数组  结果是从`${parameter[offset]}`开始的*length*个数组成员'; 若*offset*为负, 则是从最大索引的成员开始确定偏移; 若*length*为负, 是*expansion error*
  * 若*parameter*是关联数组  结果是未定义的

* 说明: *substring indexing*都是从零开始的, 除了位置形参外;
  * 位置形参的默认索引从1开始

`${!prefix*}` , `${!prefix@}`: 匹配前缀的名字

* 展开成名字以*prefix*开头的变量名, 使用`IFS`特殊变量的第一个字符分隔变量名
* 若使用`@`, 则展开后的每个变量名都在一对双引号中

`${!name[*]}` , `${!name[@]}`: 数组key列表

* 若*name*是数组变量, 展开成*name*的索引(或者*key*)列表
* 若使用`@`, 则展开后的每个*key*都在一对双引号中

`${#parameter}`: 形参长度

* 返回*parameter*展开后的字符数
* 若*parameter*是`*`或`@`, 则返回位置形参的数量
* 若*parameter*是下标为`*`或`@`的数组, 则返回该数组的元素数

`${parameter#word}` , `${parameter##word}`: 移除匹配前缀

* *word*被当作一个模式, 用于进行模式匹配
* 如果模式与展开后的*parameter*的开始部分匹配, 则返回一个移除匹配部分的展开后的parameter
  * `#`表达移除最短匹配部分, `##`表达移除最长匹配部分
* 若*parameter*是`*`或`@`, 则依次对每个位置形参进行操作, 然后返回一个结果列表
* 若*parameter*是下标为`*`或`@`的数组, 则依次对数组的每个元素进行操作, 然后返回一个结果列表

`${parameter%word}` , `${parameter%%word}`: 移除匹配后缀

* *word*被当作一个模式, 用于进行模式匹配
* 如果模式与展开后的*parameter*的尾部匹配, 则返回一个移除匹配部分的展开后的parameter
  * `#`表达移除最短匹配部分, `##`表达移除最长匹配部分
* 若*parameter*是`*`或`@`, 则依次对每个位置形参进行操作, 然后返回一个结果列表
* 若*parameter*是下标为`*`或`@`的数组, 则依次对数组的每个元素进行操作, 然后返回一个结果列表

`${parameter/pattern/string}`: 模式替换

* *pattern*像*filename expansion*那样展开生成一个模式
* 若展开后的*parameter*与模式匹配, 将最长匹配部分替换成*string*; 
  * 通常是在第一次匹配处进行替换; 
  * 若*pattern*以`/`开头, 则将所有匹配部分替换成*string*
  * 若*pattern*以`#`开头, 则将在*parameter*展开值的头部的匹配部分替换成*string*
  * 若*pattern*以`%`开头, 则将在*parameter*展开值的尾部的匹配部分替换成*string*
* 若*string*为空, 则删除匹配部分; 若*pattern*以`/`开头, 则会忽略该模式
* 若启用*nocasematch*选项, 则模式匹配时不考虑字母的大小写
* 若*parameter*是`*`或`@`, 则依次对每个位置形参进行操作, 然后返回一个结果列表
* 若*parameter*是下标为`*`或`@`的数组, 则依次对数组的每个元素进行操作, 然后返回一个结果列表

`${parameter^pattern}` , `${parameter^^pattern}` , `${parameter,pattern}` , `${parameter,,pattern}`: 大小写修改

* *pattern*像*filename expansion*那样展开生成一个模式;
  * 模式不应该试图与超过一个字符进行匹配
  * 若*pattern*被省略, 则模式为`?`, 即匹配每个字符
* 展开后的*parameter*中的每个字符都要经过*pattern*的检查; 若匹配成功则转换大小写
  * `^`表示将第一个匹配的小写字符转换成大写
  * `^^`表示将所有匹配的小写字符转换成大写
  * `,`表示将第一个匹配的大写字符转换成小写
  * `,,`表示将所有匹配的大写字符转换成小写
* 若*parameter*是`*`或`@`, 则依次对每个位置形参进行操作, 然后返回一个结果列表
* 若*parameter*是下标为`*`或`@`的数组, 则依次对数组的每个元素进行操作, 然后返回一个结果列表

`${parameter@operator}`: 形参转换

* 根据不同的*operator*, 对*parameter*进行不同的处理
* 该展开的结果可以进行: *word splitting* 和 *pathname expansion*

* | *operator* | 描述                                                         |
  | ---------- | ------------------------------------------------------------ |
  | `Q`        | quote的缩写; 以可作为输入重用的格式引用*parameter*的值, 返回被引用的*parameter*的值; |
  | `E`        | escape的缩写;把*parameter*的值中包含的转义序列进行展开, 就像`$',,,'`引用机制那样; 返回处理后的*parameter*值 |
  | `P`        | prompt的缩写;将*parameter*的值按照提示符字符串进行展开       |
  | `A`        | assignment的缩写; 把*parameter*的值和它的名字一起, 转换成可重用的赋值语句的形式; 如果*parameter*带有任何的属性, 会转换成用*declare*命令的形式, 且带有对应的属性 |
  | `a`        | *attribute*的缩写; 获得*parameter*的所有属性的标志           |

* 若*parameter*是`*`或`@`, 则依次对每个位置形参进行操作, 然后返回一个结果列表

* 若*parameter*是下标为`*`或`@`的数组, 则依次对数组的每个元素进行操作, 然后返回一个结果列表

### 3.5.4 Command Substitution

*command substitution*允许将命令替换成命令的输出

* 形式
  * `$(command)`
  * `` `command` ``
* *bash*在一个*subshell*环境指向*command*, 然后将该*command*替换成其标准输出
  * 会删除标准输出中结尾的所有`newline`, 但不会删除嵌套的`newline`; 不过嵌套的`newline`在*word splitting*中被移除

* 使用反引号形式时, *command*中的反斜杠只有字面意思, 除非后接`$`, `` ` ``或`\`
* *command substitution*可以嵌套; 若使用反引号形式时, 需要为嵌套的反引号加上`\`

### 3.5.5 Arithmetic Expansion

*arithmetic expansion*允许将其展开成算术表达式的求值结果

* 形式: `$(( expression ))`
* 其中的*expression*如同在双引号中一样, 就是说*expression*中的所有*token*可以执行*parameter and variable expansion*, *command substitution*, 和*quote removal*
* 如果*expression*是无效的, *bash*向标准错误输出信息, 且不会发生替换
* *arithmetic expansion*可以嵌套

### 3.5.6 Process Substitution

*process substitution*允许一个进程的输入输出被一个文件名引用

* 形式
  * `<(list)`
  * `>(list)`
* 进程*list*异步执行, 然后其输出或输入作为一个文件名出现
  * 作为*process substitution*的结果, 该文件名可以像其他文件名那样作为命令的实参
  * 如果使用`>(list)`, 向文件名写入, 将会为*list*提供输入
  * 如果使用`<(list)`, 读取文件名, 将会得到*list*的输出
* *process substitution*需要系统支持*named pipes(FIFOs)*或为`/dev/fd`方法

### 3.5.7 Word Splitting

*shell*将`$IFS`中的每个字符当作定界符, 使用这些字符将其他展开的结果分割成多个*word*

* *parameter expansion*, *command substitution*, 和*arithmetic expansion*的结果, 若没有出现在双引号中, 则进行*word splitting*

* `IFS`
  * 如果`IFS`未设置, 或其值为`<space><tab><newline>`, 也就是默认值, 被称为`IFS`空白字符(*whitespace charater*)
    * 在之前展开的结果中, 忽略头尾部分的由空白字符组成的序列
    * 在之前展开的结果中, 不在头尾部分的任一`IFS`字符序列用于分割*word*
  * 如果`IFS`有非默认的值, 
    * 在之前展开的结果中, 若某个空白字符在`IFS`中, 忽略头尾部分的由对应的空白字符组成的序列, 
    * 在之前展开的结果中, 非空白字符加上任一相邻空白字符, 或任一`IFS`空白字符序列都能分割*word*
  * 如果`IFS`为空, 则不会进行*word splitting*
* `null`参数
  * 显式的*null*参数, 如`""`或`''`, 不会被忽略掉, 将作为空字符串传递给命令
  * 若被引用的*null*参数是某个*word*的一部分, 且该*word*的展开不为空, 则移除该*null*
    * 如`-d''`展开成`-d`
  * 非引用的隐式的*null*参数会被移除

### 3.5.8 Filename Expansion

*filename expansion*

* 在*word splitting*之后, 除非设置了`-f`选项, *bash*会扫描每个*word*以寻找字符`*`, `?`和`[`
* 若找到, 该*word*被认为是一个模式, 并将该*word*替换成一个与其匹配的文件名列表, 该列表按字母顺序排列
  * 如果启用`nocaseglob`选项, 则匹配时不考虑字母的大小写
  * 如果没有找到匹配的文件名, 
    * 若未启用`nullglob`选项, 则该*word*保持不变
    * 若启用`nullglob`选项, 则移除该*word*
    * 若启用`failglob`选项, 则打印错误信息并不执行所在的命令

`.`和*filename expansion*

* 当某个模式用于*filename expansion*时, 若`.`字符在文件名的开头或紧接一个`/`字符, 必须显式匹配该`.`字符, 除非启用`dotglob`选项
* 文件名`.`和`..`必须显式匹配, 即使启用`dotglob`
* 其他情况下, `.`不被特殊对待

`/`和*filename expansion*

* 当匹配文件名时, `/`字符必须总是与模式中的`/`字符显式匹配

`GLOBIGNORE` *shell*变量

* 用于限制与模式匹配的文件名字的集合
* 若设置`GLOBIGNORE`, 某个文件名与模式匹配成功, 如果它也与`GLOBIGNORE`中的模式匹配, 则该文件名被移出匹配列表; 也就是忽略与`GLOBIGNORE`中的模式匹配的文件名
* 若设置`GLOBIGNORE`, 且不为空
  * 会启用`dotglob` *shell*选项, 因此所有其他以`.`开头的文件名会进行匹配
    * 可以向`GLOBIGNORE`添加`.*`模式, 以忽略以`.`开头的文件名
  * 总是忽略文件名`.`和`..`
* 当未设置`GLOBIGNORE`时`dotglob`选项是未启用的
* 若启用`nocasetoption`选项, 与`GLOBIGNORE`中的模式匹配不会考虑大小写

#### 8.1模式匹配

除了以下描述的特殊模式字符外, 在模式中的其他字符与它自己匹配

* 空字符不可以出现在模式中
* 反斜杠`\`会转义特殊模式字符; 用于转义的反斜杠在匹配时被抛弃

以下是特殊模式字符

`*`

* 匹配任一字符串, 包括空字符串;
* 当启用`globstar` *shell*选项时, 在*filename expansion*中, `**`模式会匹配所有文件和零个或多个目录和子目录;
  * 如果模式为`**/`, 则只匹配目录和子目录

`?`

* 匹配任何单个字符

`[...]`

* 匹配其中任一字符

* 范围表达式: 一对字符被`-`分隔, 表示这两个字符之间的字符闭区间

  * 使用当前语言环境的排列序列和字符集合
  * 范围表达式的排序顺序取决于当前语言环境(*locale*), `LC_COLLATE`和`LC_ALL` *shell*变量(如果有的话)
    * 要使用*C locale*, 可以将`LC_COLLATE`或`LC_ALL`设置为`'C'`, 或者启用`globasciiranges` *shell*选项

* 另一种形式

  * 若紧接`[`后的第一个字符为`!`或`^`, 表示匹配任何未包含的字符

* 匹配`-`或`]`

  * 若想要匹配`-`, 则将其放在最前面或最后面
  * 若想要匹配`]`, 则将其放在最前面

* 字符类别

  * 形式: `[:class:]`

  * 在`[...]`中, 使用`[:class:]`指定某一类字符

  * 可用的字符类别(*POSIX*标准)

    ```shell
    alnum   alpha   ascii   blank   cntrl   digit   graph   lower
    print   punct   space   upper   word    xdigit
    ```

* 等价类别(*equivalence class*)
  * 形式: `[=C=]`
  * 在`[...]`中, 使用`[=C=]`指定等价类别, 以匹配所有与`C`的*collation weight*相同的字符
    * *collation weight*由当前*locale*定义
* 在`[...]`中, `[.symbol.]` matches the collating symbol symbol.



若`shopt`启用`extglob` *shell*选项, 则可以使用以下的扩展模式匹配运算符

* 以下的*pattern-list*是一个或多个`|`分隔的模式列表

| 形式              | 描述                                 |
| ----------------- | ------------------------------------ |
| `?(pattern-list)` | 匹配零个或一个给定模式的*occurrence* |
| `*(pattern-list)` | 匹配零个或多个给定模式的*occurrence* |
| `+(pattern-list)` | 匹配一个或多个给定模式的*occurrence* |
| `@(pattern-list)` | 匹配给定模式                         |
| `!(pattern-list)` | 匹配除了给定模式以外的任何内容       |

### 3.5.9 Quote Removal

在经过前面的展开后, 所有未引用的, 且不是之前展开生成的`\`, `'`和`"`字符都被移除

## 3.6 重定向

在执行命令之前, 命令的输出和输入可以进行重定向

* 重定向运算符(*redirection operator*):
* 重定向操作的处理顺序是它们从左向右出现的顺序

重定向与文件描述符

* 可以把一个文件描述符或者`{varname}`在重定向运算符前
  * 若使用`{varname}`形式, 
    * 该重定向的作用域是超过该命令的作用域
    * 对于除了`>&-`和`<&-`外的其他重定向运算符, *shell*会分配一个大于等于10的文件描述符, 并将其赋值给`varname`
    * 对于`>&-`和`<&-`重定向运算符, `varname`的值表示要关闭的文件描述符
* 如果省略了文件描述符
  * 当重定向运算符的第一个字符为`<`, 则重定向默认指向标准输入(文件描述符0)
  * 当重定向运算符的第一个字符为`>`, 则重定向默认指向标准输出(文件描述符1)
* 谨慎使用大于9的文件描述符进行重定向, 因为它们可能与*shell*内部使用的文件描述符冲突
* 打开或创建文件失败, 会导致重定向失败

以下描述中, 跟在重定向运算符后的*word*, 除非另作说明, 都可以进行: 

* brace expansion, tilde expansion, parameter expansion, command substitution, arithmetic expansion, quote removal, filename expansion, and word splitting.
* 如果该*word*展开成多个*word*, *bash*报错

*bash*在重定向时会特殊处理的文件名;

* 若操作系统提供这些特殊文件, *bash*将会使用它们; 否则, *bash*会在内部模拟它们的行为

| 文件名;              | 描述                                                         |
| -------------------- | ------------------------------------------------------------ |
| `/dev/fd/FD`         | 若*FD*是有效的整数, 复制该文件描述符*FD*                     |
| `/dev/stdin`         | 复制文件描述符0                                              |
| `/dev/stdout`        | 复制文件描述符1                                              |
| `/dev/stderr`        | 复制文件描述符2                                              |
| `/dev/tcp/host/port` | 若*host*是有效主机名或网络地址, *port*是一个整数端口号或服务名, *bash*试图打开对应的TCP套接字(*socket*) |
| `/dev/udp/host/port` | 若*host*是有效主机名或网络地址, *port*是一个整数端口号或服务名, *bash*试图打开对应的UDP套接字(*socket*) |

### 3.6.1 输出重定向

重定向输出的一般形式: `[n]>[|]word`

* 展开*word*, 将展开结果当作文件名
* 若该文件不存在, 则创建该文件; 若该文件存在, 则将其大小截断成零.
* 打开或创建该文件后, 文件描述符*n*可以写入数据到该文件

`noclobber`选项

* 若重定向运算符为`>`, 且用`set`启用`noclobber`选项, 那么当*word*所表示的文件存在, 并且是普通文件时会导致重定向失败
* 若重定向运算符为`>`或`>|`, 且未启用`noclobber`选项, 那么即使*word*所表示的文件存在, *bash*依然试图对其重定向

### 3.6.2 输入重定向

重定向输入的一般形式: `[n]<word`

* 展开*word*, 将展开结果当作文件名
* 打开该文件后,  文件描述符*n*可以从该文件读取数据

### 3.6.3 追加(appending)重定向输出

*Appending redirected output*的一般形式: `[n]>>word`

* 展开*word*, 将展开结果当作文件名
* 若该文件不存在, 则创建该文件
* 打开或创建该文件后, 文件描述符*n*向该文件*append*数据

### 3.6.4 重定向标准输出和标准错误

将标准输出(文件描述符1)和标准错误(文件描述符2)同时重定向到*word*所表示的文件

* 两种格式: 首选第一种格式
  * `&>word`
  * `>&word`
  * 在语义上等同于: `>word 2>&1`
* 使用第二种格式时, *word*不能展开成一个数字或`-`;
  * 若这样做的话, 出于兼容原因, 将应用其他重定向(*duplicating file descriptors*)

### 3.6.5 追加标准输出和标准错误

将标准输出(文件描述符1)和标准错误(文件描述符2)同时*append*重定向到*word*所表示的文件

* 格式: `&>>word`
* 语义上等同于: `>>word 2>&1`

### 3.6.6 Here documents

* 过程
  * *shell*从当前命令中读取输入, 直到看到一行只包含*word*的输入(尾部不能有*blank*)
  * 然后, 至此为之的所有读取行(*here-document*), 被用于所在的命令的文件描述`n`
    * 若没有指定文件描述符`n`, 则默认为标准输入
  * 若重定向运算符为`<<-`, *here-document*和*delimiter*行中的所有在行头的*tab*字符都会被删除

* 格式

  ```shell
  [n]<<[-]word
  		here-document
  delimiter
  ```

* 说明:
  * *word*不能执行:  parameter and variable expansion, command substitution, arithmetic expansion, 或filename expansion
  * 若*word*的任意部分被引用, 则*delimiter*是*word*的*quote removal*的结果, 并且*here-document*不能展开
  * 若*word*未被引用, *here-document*可以执行: parameter expansion, command substitution, 和arithmetic expansion; 其中的`\<newline>`会被忽略, `\`可以用于引用`\`, `$`和`` ` ``

### 3.6.7 Here strings

*here document*的变体形式: `[n]<<< word`

* *word*可以执行:  tilde expansion, parameter and variable expansion, command substitution, arithmetic expansion, 和quote removal
  * 不会执行: pathname expansion, word splitting

区别:

* 其结果为一行字符串, 尾部有一个`<newline>`

### 3.6.8 复制文件描述符

重定向运算符`<&`, 用于复制输入文件描述符

* 形式: `[n]<&word`
* 

### 3.6.9 移动文件描述符

### 3.6.10 打开文件描述符

## 3.7 执行命令

### 3.7.1 简单命令展开

### 3.7.2 命令查找和执行

### 3.7.3 命令执行环境

### 3.7.4 环境

### 3.7.5 退出状态

### 3.7.6 信号

## 3.8 *Shell* 脚本

# 4. *Shell*内置命令

# 5. *Shell*变量

# 6. *Bash*特性

# 7. *Job*控制

# 8. 命令行编辑

# 9. 历史功能
