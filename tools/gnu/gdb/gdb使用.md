# 总结

关于编译器

* GCC: `-g`, `-Og`

GDB

* 选项: 
  * `-silent`, `-quite`, `-q`: 屏蔽GDB的导言和版权信息
  * `-symbols=FILE`, `-s FILE`: 只从`FILE`文件中读取符号表
  * `-exec=FILE`, `-e FILE`: 将`FILE`文件作为执行文件
  * `-core=FILE`, `c FILE`: 指定`FILE`文件为core dump文件
  * `-cd DIRECTORY`: 将`DIRECTORY`作为GDB的工作目录; 默认为当前目录
  * `-args ARG1 ARG2 ...`: 向可执行文件传递参数`ARG1 ARG2 ...`
  * `-directory=DIRECTORY`, `-d DIRECTORY`: 将`DIRECTORY`添加到搜索源文件的路径中
  * `-command=FILE`, `-x FILE`: 从`FILE`文件中执行GDB命令
  * `-ex COMMAND`: 执行GDB命令`COMMAND`
  * `-tty=DEVICE`: 指定`DEVICE`作为执行程序的标准输入和标准输出

* 常用命令:
  * `break`: 设置断点
  * `run`: 执行程序, 直至遇到断点或结束
  * `start`: 执行到`main()`函数的第一行语句前
  * `continue`: 继续执行程序
  * `next`: 执行下一行代码
  * `print`: 打印
  * `list`: 显示源代码的内容, 包括行号
  * `quit`: 退出GDB

[toc]

# 命令

命令的几种类型

* 别名aliases: 其他命令的别名
* 断点breakpoints: 程序在某个点停止
* 数据data: 检验数据
* 文件files: 指定并检验文件

* 内部internals: 维护命令
* 运行running: 运行程序
* 栈stack
* 状态status: 状态查询
* support
* 追踪点tracepoints: 对运行中的程序进行追踪
* 用户自定义user-defined
* obscure: obscure feature( 其他类型的命令)

## 程序执行

启动

* `run`
* `start`

执行

* `next`: 执行n行代码; n的默认值为1
  * 如果当前代码行调用其他函数, 不会进入该函数中, 而是执行到下一行代码
* `step`: 执行n行代码; n的默认值为1
  * 如果当前代码行调用其他函数, 进入被调用的函数中, 执行到被调用函数的第一行代码
* `until`: 
  * 不带参数时: 如果在循环内, 不断执行直至跳出当前循环; 其他情况下, 与`next`一样
  * 带参数`until n`: 执行到行号n的代码行

* `finish`: 执行直到跳出该函数

跳转

* `return`: 结束当前执行的函数, 到达上层函数调用处并停止执行; 若有指定值, 返回该值
* `jump`: 从当前执行处直接跳转到指定位置处, 并继续执行后续代码

## 断点设置

断点breakpoint: 可有`if CONDITION`

* `break`
* `tbreak`
* `rbreak`

观察点watchpoint: 可有`if CONDITION`

* `watch`
* `rwatch`
* `awatch`

捕捉点catchpoint

* `catch`
* `tcatch`

`condition`命令: **增添删**断点的条件

* 能够为以上三种断点添加条件, 也能修改(或删除)断点的条件表达式

`ignore`命令

* 能够使某个断点失效N次; N为所指定的次数

删除断点

* `clear`
* `delete`

## 显示

* `print`: 
  * `print EXPRESSION`: 执行给定的表达式, 打印表达式所得的值; 若该表达式是赋值表达式, 可修改操作数的值
  * 支持`@`和`::`
* `display`: 使用一次`display`命令后, 每次程序暂停执行都会自动打印目标变量或表达式的值

* `undisplay`: 取消`display`表中的变量或表达式

## 异步(后台)执行GDB命令

异步执行/后台执行

* 当某个调试命令正在执行时, 可以继续执行下一个调试命令
* `COMMAND&`: 在调试命令后添加一个`&`字符; 如`continue&`
* 支持后台执行的命令
  * `run`, `attach`, `step`, `stepi`, `next`, `nexti`, `continue`, `finish`, `until`

`interrupt`: 暂停后台线程的执行

* all-stop模式下, `interrupt`指令作用于所有线程; 在non-stop模式下,  `interrupt`指令仅作用于当前线程

## 反向调试

所谓反向调试, 指的是临时改变程序的执行方向, 反向执行指定行数的代码, 此过程中GDB调试器可以消除这些代码所做的工作, 将调试环境还原到这些代码未执行前的状态

> 注意: 目前GDB调试器对反向调试的功能支持还不完善, 反向调试尚不能适用于所有的调试场景
>
> * 例如, 对于打印到屏幕的数据并不会因为打印语句的回退而自动消失
> * 反向调试不适用于包含I/O操作的代码

常用命令

* `record`, `record btrace`: 让程序开始记录反向调试所必要的信息, 包括保存程序每一步运行的结果等等信息
  * 进行反向调试的前提是执行该命令
* `reverse-continue`, `rc`: 反向运行程序, 直到遇到使程序中断的事件.比如断点或者已经退回到`record`命令开启时程序执行到的位置
* `reverse-step`: 反向执行一行代码
* `reverse-next`
* `reverse-finish`
* `set exec-direction forward|reverse`
  * `forward`: 正常执行命令
  * `reverse`: 反向执行命令; 此时直接只用`step`, `next`, `continue`, `finish`命令来反向调试程序

# GDB与多线程

常用命令

* `info threads`
* `thread ID`: 切换到`ID`所表示的线程
* `thread apply ID COMMAND`: 对`ID`线程运行`COMMAND`命令
* `break LOCATION thread ID`: 为`ID`线程在`LOCATION`位置建立一个断点
* `set scheduler-locking off|on|step`: 线程锁
  * `off`: 默认情况; 此时GDB使用全停止(all-stop)模式; 也就是说, 若某一线程暂停执行, 其他线程都会随机暂停; 若某一线程开始执行, 其他线程也会随即启动.
  * `on`: 锁定线程, 只有当前线程或指定线程可以运行
  * `step`: 当单步执行某一线程时, 其他线程不会执行, 同时保证在调试过程中当前线程不会发生改变; 若执行`continue`, `until`, `finish`命令时, 其他线程也会执行

non-stop模式: 

* 在non-stop模式下, `continue`, `next`, `step`命令只作用于当前线程
  * 此时, 若想要作用于所有线程, 在命令后添加`-a`; 如`continue -a`
* `set non-stop on|off`
* `show non-stop`
* 注意: 并非所有的调试环境都支持non-stop模式, 即便借助`set non-step`命令启动了该模式, GDB调试器也可能会自动切换至all-stop模式

# GDB与多进程

> GDB没有对多进程程序提供友好的调试功能

调试子进程的一种做法: 利用`attach`命令

* 在目标进程所执行代码的开头位置, 添加一段延时执行的代码(或死循环)
* 然后, 使用`attach`命令捕捉到要调试的进程
* 使用`print`命令或`jump`命令跳出延时执行的代码(或死循环)

`follow-fork-mode`的两种状态: `set follow-fork-mode parent|child`

* `parent`: 默认情况; 默认只调试父进程
* `child`: 只调试子进程; 当程序中包含多个子进程时, 我们可以逐一对它们进行调试.

`detach-on-fork`的两种状态: `set detach-on-fork on|off`

* `on`: 默认值, 表明 GDB 只调试一个进程, 可以是父进程或者某个子进程
* `off`: 程序中出现的每个进程都会被GDB记录, 我们可以随时切换到任意一个进程进行调试

常用指令

* `info inferiors`: 查看当前调试环境中有多少个进程
* `inferiors ID`: 切换到`ID`编号的进程
* `detach inferior ID`: 断开GDB与`ID`进程之间的联系, 使该进程可以独立运行. 不过, 该进程仍存在`info inferiors`打印的列表中, 其Describution列为`<null>`, 并且借助`run`仍可以重新启用
* `kill inferior ID`: 断开GDB与`ID`进程之间的联系, 并中断该进程的执行.不过, 该进程仍存在`info inferiors`打印的列表中, 其Describution列为`<null>`, 并且借助`run`仍可以重新启用
* `remove-inferior ID`: 彻底删除`ID`进程; 在此之前, 需先使用`detach inferior ID`或`kill inferior ID`命令, 并确认其不是当前进程

# 信号处理

GDB默认情况下会自动捕获C/C++程序中出现的信号, 并根据事先约定好的方式处理它.

* `info signals`

* `handle signal nostop|stop|noprint|print|nopass|pass`

# 栈

## 帧命令: 作用于一个帧

* `frame`
* `info frame`
* `up`, `down`
* `info args`
* `info locals`

## `backtrace`命令：打印多个帧的信息

* 注意： 
  * 对于多线程程序, 该命令仅作用于当前线程