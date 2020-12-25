# 1. Makefile总述

Makefile主要包含五个部分

* 显示规则: 说明如何生成一个或多个目标文件
* 隐晦规则: GNU make的自动推导
* 变量: 变量的定义和使用
* 文件指示:  
  * 如何引用其他的makefile
  * 如何根据情况指定有效部分
  * 定义一个多行的命令
* 行注释: 在行头使用`#`字符

## 1.1 makefile的文件名

默认情况下, make命令在当前目录下按顺序寻找文件

* `GNUmakefile`: GNU make特有的

* `makefile`
* `Makefile`

建议使用`Makefile`作为文件名; 因为大多数make都支持`Makefile`

可使用`-f FILE`, `--file=FILE`, `--makefile==FILE`指定makefile

## 1.2 引用其他的makefile

使用`include`关键字: 被包含的文件会原模原样地放在当前文件的包含位置

```makefile
include <FILENAME> ...
```

* `FILENAME`可以是当前操作系统shell的文件模式(可以包含路径和通配符), 也可以是变量
* 在`include`关键字前面可以有一些空字符, 但不能以`<TAB>`键开头
* `include`和`<FILENAME>`之间用一个或多个空格隔开

* ```makefile
  include foo.make *.mk $(bar)
  ```

`include`的具体过程:

* 在make命令开始时会寻找`include`所引用的其他makefile, 并把其内容放在`include`所在的位置
* 若文件中没有指定(绝对或相对)路径, 首先在当前路径下寻找
* 若在当前目录下找不到, make还会在以下目录寻找
  * `-I`, `--include-dir`所指定的目录
  * `<prefix>/include`(若存在的话)
    * `<prefix>`一般是`/usr/local/bin`或`/usr/include`
* 若无法找到文件或无法读取文件时, make会生成警告信息, 并继续载入其他文件. 在完成对makefile的读取后, make会再次尝试搜索; 若还是不行, make会生成一条致命信息

忽略错误并继续执行:

* `-include`: 就是在`include`前面加上`-`
* `sinclude`: 在其他make中的相关命令

## 1.3 环境变量MAKEFILES

> 建议不要使用这个环境变量, 因为一旦定义该环境变量, 所有makefile都会受到影响

若定义了环境变量`MAKEFILES`, make会把这个变量中的值进行类似`include`的动作

* 与`include`不同的是, 从该环境变量中引用的目标文件不会起作用; 若这些目标文件发现错误时, make不会进行任何动作

## 1.4 make的工作方式

GNU make的执行步骤

1. 读入所有makefile
2. 读入被include的makefile
3. 初始化文件中的变量
4. 推导隐晦规则, 并分析所有规则
5. 为所有的目标文件创建依赖关系链
6. 根据依赖关系, 决定哪些目标要重新生成
7. 执行生成命令

说明

* 1~5步为第一个阶段, 6~7步为第二个阶段
* 在第一个阶段中, 若定义的变量被使用了, make会将其展开在使用的位置中, 但不会马上完全展开; 如果变量出现在依赖关系的规则中, 仅当这条依赖被使用时, 变量才会展开

# 2. 如何定义"目标的规则"

> 目标, 是指要生成的文件/对象; 
>
> 目标的规则, 用于描述怎么达到目标; 也就是通过什么命令序列可以得到目标

"目标的规则"包含

* 依赖关系
* 生成目标的方法

默认情况下,  make将第一个命令块中的目标视为最终的目标; 若该命令块中的目标有多个, 则第一个目标成为最终的目标

## 2.1 语法

```makefile
targets ... : prerequisites
	command
	...
# 或是这样
targets ... : prerequisites ; command
	command
	...
```

* 在第一行, 冒号前面有一个或多个目标, 以空格分开; 目标是文件名, 可以使用通配符
* 在第一行,  在冒号后面可以有零个或多个依赖目标, 以空格分开; 依赖目标是文件名

* 在第一个行后面可以使用`;`作为分隔符, 表示一行的终止
* 其他部分是零条或多条命令行, 以`<TAB>`开头

## 2.2 通配符

make支持三个通配符

* `*`: 匹配零个或多个任意字符
* `?`: 匹配一个任意字符
* `[...]`

通配符可以使用的地方

* 命令行中`command`
* 规则(即命令块)
* 变量

## 2.3 文件搜索

默认情况下, make在当前目录下搜索文件

特殊变量`VPATH`

* 若定义了该变量, make在当前目录下找不到文件时, 到所指定的目录中进行搜索

* ```makefile
  VPATH = DIR1:DIR2
  ```

* 可以指定多个目录, 以`:`分隔

关键字`vpath`

* 三种使用方法

  * `vpath <PATTERN> <DIRECTORIES>`: 为与模式匹配的文件指定了搜索目录
    * 若文件与多个模式匹配, 则make会按照先后顺序来执行搜索
  * `vpath <PATTERN>`: 清除与模式匹配的文件的搜索目录
  * `vpah`: 清除所有已设置的文件搜索目录
  * 说明: 
    * `<PATTERN>`需要包含`%`字符; `%`表示匹配零个或多个字符

* 例子

  ```makefile
  vpath %.h ../headers
  vpath %.c foo
  vpath % blish
  vpath %.c bar
  ```

## 2.4 伪目标

伪目标

* 是指, 该目标不是一个文件
  * 如前面提到的`clean`是伪目标
* 伪目标也是一种目标

为避免与文件重名, 使用`.PHONY`显示地指明哪些目标是伪目标

* 显示指定为伪目标后, 该目标的命令块总是会执行

例子

```makefile
all : prog1 prog2 prog3
.PHONY : all
prog1 : prog1.o utils.o
	cc -o prog1 prog1.o utils.o
prog2 : prog2.o
	cc -o prog2 prog2.o
prog3 : prog3.o sort.o utils.o
	cc -o prog3 prog3.o sort.o utils.o
```

## 2.5 多目标

makefile中, 一个规则中可以有多个目标

```makefile
target1 target2 : prog1 prog2
	command
	...
```

通常与自动化变量, makefile函数配合使用

```makefile
bigoutput littleoutput : text.g
	generate text.g -$(subst output,,$@) > $@
#等价于
bigoutput : text.g
	generate text.g -big > bigoutput
littleoutput : text.g
	generate text.g -little > littleoutput
```

* 例子中使用了一个`subst`函数,  用于截取字符串; 和一个`$@`, 表示目标的集合

## 2.6 静态模式

静态模式可以更加容易地定义多目标的规则

```makefile
targets ... : target-pattern : prereq-pattern ...
	command
	...
```

* 有一个或多个目标
* 有一个目标模式
* 有一个或多个依赖模式

* 说明:
  * 使用目标模式对所有目标进行匹配, 然后根据依赖模式进行依赖的定义

例子

```makefile
objects = foo.o bar.o
all: $(objects)
$(objects): %.o: %.c
	$(CC) -c $(CFLAGS) $^ -o $@
#等价于
foo.o : foo.c
	$(CC) -c $(CFLAGS) foo.c -o foo.o
bar.o : bar.c
	$(CC) -c $(CFLAGS) bar.c -o bar.o
```

* 其中自动化变量`$^`表示依赖文件, `$@`表示目标

## 2.7 编译器能够自动生成依赖性

`-M`选项会自动搜索源文件中包含的头文件, 并生成相应的依赖关系

* 对于GNU的C/C++编译器, `-M`会把标准库中的头文件也包含进来; `-MM`则不包含标准库的头文件

GNU组织的建议: 源文件由编译器自动生成的依赖关系, 应该放到`.d`文件中

* 即每个`name.c`文件自动生成的依赖关系, 放到`name.d`文件中

* 这样做的话, 可以写出`.c`文件和`.d`文件的依赖关系, 并让make自动更新或生成`.d`文件

* 例子: 一个模式规则

  ```makefile
  %.d : %.c
  	@set -e; rm -f $@; \
  	$(CC) -M $(CPPFLAGS) $< > $@.$$$$; \
  	sed 's,\($*\)\.o[ :]*,\1.o $@ :,g' < $@.$$$$ > $@; \
  	rm -f $@.$$$$
  ```

# 3. 命令

在规则中的每条命令, 等价于在shell的命令行

* 每条命令的开头必须是`<TAB>`, 或者该命令紧跟在依赖规则后的分号后面

* 命令行之间的空格或空行会被忽略, 除非它们是以`<TAB>`开头; 此时, 它们被视为空命令

在unix中,  make的命令是被环境变量`SHELL`所定义的shell解释执行, 除非显示指定其他shell

* `SHELL`的值一般是`/bin/sh`, 即UNIX的标准shell

* (以下是原文)但在MS-DOS下有点特殊, 因为MS-DOS下没有SHELL环境变量, 当然你也可以指定. 如果你指定了UNIX风格的目录形式, 首先, make会在SHELL所指定的路径中找寻命令解释器, 如果找不到, 其会在当前盘符中的当前目录中寻找, 如果再找不到, 其会在PATH环境变量中所定义的所有路径中寻找. MS-DOS中, 如果你定义的命令解释器没有找到, 其会给你的命令解释器加上诸如“.exe”、“.com”、“.bat”、“.sh”等后缀. 

## 3.1 命令的显示

通常, make会把要执行的命令在执行前输出到标准输出中

`@`

* 若在命令行前使用`@`, 该命令将不被输出 

make的选项

* `-n`, `--just-print`, `--dry-run`, `--recon`: 只显示命令, 不执行命令
* `-s`, `--silent`, `--quiet`: 禁止显示命令

## 3.2 命令的执行

默认情况下, 每行命令在独立的环境下执行

* 所以, 若要让上一条命令的效果能够作用于下一条命令, 应该使用分号分隔

```makefile
# 当前目录为home目录
exec:
	cd /
	pwd 	# 输出home目录
exec:
	cd /; pwd # 输出"/
```

## 3.3 命令出错

每当命令运行完后, make会检查每个命令的返回码;

* 如果命令返回成功(即0), 那么make会继续执行下一条命令; 
* 如果某个命令出错了(即非零值), 那么make会终止执行当前规则, 这将有可能终止所有规则的执行

若不想检查命令的返回码, 

* 可以在makefile的命令行前加上`-`, 以忽略当前命令的错误

  ```makefile
  clean:
  	-rm -f *.o
  ```

* make使用`-i`, `--ignore-errors`选项, 以忽略所有命令的错误

在命令出错后, 结束当前规则, 但继续执行其他规则

* 使用`-k`, `--keep-going`选项

### 3.4 嵌套执行make

> 在一些大的工程中, 把不同模块或是不同功能的源文件放在不同的目录中, 然后在每个目录中都书写一个该目录的Makefile; 这有利于让我们的Makefile变得更加地简洁

可以在makefile中执行make命令, 从而嵌套执行make

当前makefile的变量可以传递到下一级的makefile中, 但是不会覆盖下一级makefile中所定义的变量, 除非执行了`-e`选项

* 要传递某些变量到下级makefile中: `export VAR ...`

* 传递所有变量到下级makefile中: `export`

* 不希望某些变量传递到下级makefile中: `unexport VAR ...`

* ```makefile
  export variable = value
  # 等价于
  variable = value
  export variable
  ```

* 说明:
  
  * `SHELL`和`MAKEFLAGS`环境变量总是传递到下级makefile中, 无论是否显式`export`

make的`-w`, `--print-directory`选项

* 使得make输出一些目录信息, 有利于调试嵌套执行
* 当使用`-C`参数来指定目录时, 将会自动打开`-w`
* 若同时有`-s`选项, 则`-w`会失效

## 3.5 命令包

```makefile
# 定义命令包
define NAME	# 命令包的名字NAME
command
...
endef		# 结束定义
# 使用命令包
target : 
	$(NAME)
```

# 4. 变量

makefile中的变量, 类似于C/C++中的宏, 表示一个字符串

* 与C/C++的宏不同的是, 可以改变makefile的变量的值

变量的命名

* 可以包含字符, 数字, 下划线
* 不能含有`:`, `#`, `=`或空白字符
* 大小写敏感

## 4.1 变量的基础

* 变量声明时需要有初始值: `VAR = VAL`, `VAR := VAL`, `vAR ?= VAL`

* 使用变量的方法: `$VAR`, `$(VAR)`, `${VAR}`; 不建议使用第一种形式

  * 因此, 若要使用`$`字符本身时, 需要用`$$`表示

* 使用变量的情景:

  * 规则中的目标, 依赖, 命令

  * 变量中

  * ```makefile
    # 这是一个示例, 以展示变量的原理; 实际的makefile中不建议这么做
    foo = c
    prog.o : prog.$(foo)
    	$(foo)$(foo) -$(foo) prog.$(foo)
    # 等价于
    prog.o : prog.c
    	cc -c prog.c
    ```

## 4.2 在变量中使用变量

用变量定义变量的两种方法

* 使用`=`: 延时展开`$(VAR)`

  ```makefile
  foo = $(bar)
  bar = $(ugh)
  ugh = Huh?
  all:
  	echo $(foo) # 输出"Huh?"
  ```

* 使用`:=`: 立即展开`$(VAR)`

  ```makefile
  x := foo
  y := $(x) bar
  x := later
  all:
  	echo $(y) # 输出"foo bar"
  ```

定义一个值为一个空格的变量

```makefile
nullstring :=
space := $(nullstring) # end of the line
all :
	@echo a$(nullstring)a # 输出"aa"
	@echo a$(space)a	  # 输出"a a"
```

* 先定义一个空变量`nullstring`
* 在定义值为一个空格的变量时, 先使用一个空变量来表明初始值的开始, 然后输入一个空格, 最后使用一个`#`注释符来表明变量定义的终止

`?=`操作符, `VAR ?= VAL`

* 若没有定义过变量`VAR`, 则定义一个值为`VAL`的变量`VAR`; 否则, 不做任何事情

## 4.3 变量高级用法

介绍两种变量的用法

* 变量值的替换
* 把变量的值当作变量, 类似于指针的概念

变量值替换的两种方法

* 一种方法: `$(VAR:SUFFIX1=SUFFIX2)`F, `${VAR:SUFFIX1=SUFFIX2}`

  * 把变量`VAR`中所有后缀为`SUFFIX1`的字符串的后缀替换成`SUFFIX2`

  * ```makefile
    foo := a.o b.o c.o
    bar := $(foo:.o=.c) #bar的值为"a.c b.c c.c"
    ```

* 静态模式:

  * ```makefile
    foo := a.o b.o c.o
    bar := $(foo:%.o=%.c) #bar的值为"a.c b.c c.c"
    ```

把变量值当作变量

* ```makefile
  x = y
  y = z
  z = u
  a := $($($(x))) #a的值为"u"
  ```

## 4.4 追加变量值

`+=`操作符

* 若该变量没有定义, 则`+=`等价于`=`

* 若该变量有定义, 则`+=`会继承前一次操作的赋值符; 然后在原本的值后面追加值

* ```makefile
  var += value
  #等价于
  var = value
  ```

* ```makefile
  var := value
  var += more
  # 等价于
  var := value
  var := $(var) more
  ```

* ```makefile
  var = value
  var += more
  #等价于
  var = value
  var = $(val) more	# 变量的递归定义?
  ```

  * 此处make会自动为我们解决变量递归定义问题

例子

```makefile
obj = main.o foo.o
obj += another.o 	 #obj的值为"main.o foo.o another.o"
```

```makefile
# obj是未定义的变量
obj += main.o foo.o # obj的值为"main.o foo.o"
```

## 4.5 override指示符

> 有些变量是特别的, 对其进行赋值会被make忽略, 如make的命令行参数

override指示符允许我们覆盖或修改这些变量

* ```makefile
  override VAR = VAL
  override VAR := VAL
  override VAR += VAL
  ```

* ```makefile
  #由define定义的变量也能通过override进行修改
  override define VAR
  command
  ...
  endef
  ```

## 4.6 多行变量

> 前面提到的命令包所用到的技术

```makefile
define VAR
command
...
endif
```

## 4.7 环境变量

> make运行时的环境变量在make开始运行时被载入到makefile文件中

默认情况下, 系统的环境变量会被makefile定义的同名变量覆盖, 或被make命令行的同名变量覆盖

* make的`-e`, `--environment-overrides`选项, 使得系统的环境变量覆盖同名变量

当嵌套调用make时, 上层的makefile中定义的变量会以环境变量的方式传递到下层的makefile中

* 如之前提到那样, 默认情况下, 通过命令行设置的变量会隐式地传递下去, 而在文件中定义的变量需要显式地通过`export`声明传递

## 4.8 目标变量

除了自动化变量外, 前面提到的变量都是"全局变量", 即能够在文件的任意位置访问这些变量

* 自动化变量属于规则型变量, 这种变量的值依赖于规则的目标和依赖目标的定义

目标变量(Target-specific variable), 即属于某个目标的局部变量

* 它的作用范围只在该目标的规则以及连带规则中

* 它可以和"全局变量"同名

* 语法

  ```makefile
  #第一种形式
  <target ...> : <variable-assignment>
  #第二种形式: 用于系统环境变量, 或make命令行带入的变量
  <target ...> : override <variable-assignment> 
  ```

  * `<variable-assignment>`可以是各种赋值表达式, 如`=`, `:=`, `+=`, `?=`

例子

```makefile
prog: CFLAGS = -g
prog: prog.o foo.o bar.o
	$(CC) $(CFLAGS) prog.o foo.o bar.o
prog.o : prog.c
	$(CC) $(CFLAGS) prog.c
foo.o : foo.c
	$(CC) $(CFLAGS) foo.c
bar.o : bar.c
	$(CC) $(CFLAGS) bar.c
```

## 4.9 模式变量(GNU)

GNU的make还支持模式变量(Pattern-specific Variable)

* 给定一个模式, 模式变量定义在和该模式匹配的所有目标上

* 语法

  ```makefile
  #1
  <pattern ...> : <variable-assignment>
  #2
  <pattern ...> : override <variable-assignment>
  ```

例子

```makefile
%.o : CFLAGS = -O
```

* 所有后缀为`.o`的目标都会有`CFLAGS`变量

# 5. 条件判断

make在读取makefile时计算条件表达式的值, 并根据条件表达式的值来选择语句

* 最好不要把自动化变量放入条件表达式中, 因为自动化变量在运行时才有的
* make不允许把整个条件语句分成两部分放在不同文件中

条件判断的语法

```makefile
#1
<conditional-directive>
<text-if-true>
endif
#2
<conditional-directive>
<text-if-true>
else
<text-if-false>
endif
```

* `<conditional-directive>`表示条件表达式, 不能以`<TAB>`开头
* `else`, `endif`所在行不能以`<TAB>`开头

条件关键字有

* `ifeq`: 两个参数相同, 则为true
* `ifneq`: 两个参数不同, 则为true
* `ifdef`:  变量非空, 则为true
* `ifndef`: 变量为空, 则为true

使用`ifeq`, `ifneq`的四种形式, 以`ifeq`为例

* `ifeq (<arg1>, <arg2>)`
* `ifeq '<arg1>' '<arg2>'`
* `ifeq "<arg1>" "<arg2>"`
* `ifeq "<arg1>" '<arg2>'`
* `ifeq '<arg1>' "<arg2>"`

使用`ifdef`, `ifndef`的四种形式, 以`ifdef`为例

* `ifdef var-name`

# 6. 函数

## 6.1 函数调用

```makefile
#1
$(<func> <args>)
#2
${<func> <args>}
```

* `<func>`为函数名
* `<args>`是对应函数所需的参数, 参数之间用`,`分隔
* 函数名和参数用空格分隔

## 6.2 处理字符串的函数

`$(subst <from>,<to>,<text>)`

* 名称: 字符串替换函数, subst
* 作用: 把字符串`<text>`中的`<from>`子字符串替换成`<to>`
* 返回: 被替换过后的字符串

* 示例

  ```makefile
  $(subst ee,EE,feet on the street)`
  # 返回结果是"fEEt on the strEEt"
  ```

`$(patsubst <pattern>,<replacement>, <text>)`

* 名称: 模式字符串替换函数, patsubst

* 功能: 查找`<text>`中的单词(单词以空格, `<TAB>`, 回车或换行字符分隔)是否与模式`<pattern>`匹配; 若匹配, 则以`<replacement>`替换

  * `<pattern>`可以包括通配符`%`, 表示任意长度的字符串; 若`<replacement>`中也包含`%`, 则`<replacement>`中的`%`是`<pattern>`中的那个`%`所匹配的字符串

* 返回: 被替换后的字符串

* 示例

  ```makefile
  $(patsubst %.c,%.o,x.c.c bar.c)
  #返回结果是"x.c.o bar.o"
  ```

`$(strip <string>)`

* 名称: 去空格函数, strip
* 功能: 移除`<string>`字符串中的开头和结尾的空白字符

* 返回: 移除头尾空白字符的字符串

* 示例

  ```makefile
  $(strip a b c )
  #原来的字符串为"a b c "
  #返回结果为"a b c"
  ```

`$(findstring <find>,<in>)`

* 名称: 查找字符串函数, findstring

* 功能: 在字符串`<in>`中查找`<find>`字符串

* 返回: 若找到, 返回`<find>`; 否则返回空字符串

* 示例

  ```makefile
  $(findstring a,a b c) # 返回结果为"a"
  $(findstring a,b c) # 返回结果为""
  ```

`$(filter <pattern...>,<text>)`

* 名称: 过滤函数, filter

* 功能: 以`<pattern>`模式过滤`<text>`字符串中的单词, 保留符合模式`<pattern>`的单词

* 返回: 符合模式`<pattern>`的单词所组成的字符串

* 示例

  ```makefile
  sources := foo.c bar.c baz.s ugh.h
  foo: $(sources)
  	cc $(filter %.c %.s,$(sources)) -o fo
  #filter函数的返回结果是"foo.c bar.c baz.s"
  ```

`$(filter-out <pattern...>,<text>)`

* 名称: 反过滤函数, filter-out

* 功能: 去除与模式匹配的单词

* 返回: 去除后的字符串

* 示例

  ```makefile
  objects = main1.o foo.o main2.o bar.o
  mains = main1.o main2.o
  $(filter-out $(mains),$(objects))
  # 返回结果是"foo.o bar.o"
  ```

`$(sort <list>)`

* 名称: 排序函数, sort

* 功能: 按字典升序为字符串`<list>`中的单词排序

* 返回: 排序后的字符串

* 示例

  ```makefile
  $(sort foo bar lost)
  #返回结果是"bar foo lost"
  ```

`$(word <n>,<text>)`

* 名称: 取单词函数, word

* 功能: 取出字符串`<text>`中的第`<n>`个单词, (从1开始)

* 返回: 第`<n>`个单词; 若`<n>`大于字符串的单词数, 返回空字符串

* 示例

  ```makefile
  $(word 2, foo bar baz)
  #返回结果是"bar"
  ```

`$(wordlist <s>,<e>,<text>)`

* 名称: 取单词串函数, wordlist

* 功能: 从字符串`<text>`取出从第`<s>`至`<e>`个单词

* 返回: 返回从`<s>`到`<e>`的单词字符串; 若`<s>`大于单词数, 返回空字符; 若`<s>`小于单词数, `<e>`大于单词数, 返回从`<s>`开始到结束的单词串

* 示例

  ```makefile
  $(wordlist 2, 3,foo bar baz)
  #返回结果为"bar baz"
  ```

`$(words <text>)`

* 名称: 单词个数统计函数, words
* 功能: 统计`<text>`中字符串中的单词个数
* 返回: `<text>`中的单词数

* 示例

  ```makefile
  $(words, foo bar baz)
  #返回结果为"3"
  ```

`$(firstword <text>)`

* 名称: 首单词函数, firstword

* 功能: 取字符串`<text>`中的第一个单词

* 返回: `<text>`中的第一个单词

* 示例

  ```makefile
  $(firstword foo bar)
  #返回结果为"foo"
  ```

## 6.3 文件名操作函数

`$(dir <names...>)`

* 名称: 取目录函数, dir

* 功能: 从文件名序列`<names>`中取出目录部分. 目录部分是指最后一个反斜杠（“/”）之前的部分. 如果没有反斜杠, 那么返回“./”

* 返回: 返回文件名序列`<names>`的目录部分

* 示例:  

  ```makefile
  $(dir src/foo.c hacks)
  #返回结果为"src/ ./"
  ```

`$(notdir <names...>)`

* 名称: 取文件函数——notdir. 
* 功能: 从文件名序列<names>中取出非目录部分. 非目录部分是指最后一个反斜杠（“/”）之后的部分
* 返回: 返回文件名序列<names>的非目录部分
* 示例:  

  ```makefile
  $(notdir src/foo.c hacks)
  #返回结果为"foo.c hacks"
  ```

`$(suffix <names...>)`

* 名称: 取后缀函数, suffix
* 功能: 从文件名序列`<names>`中取出各个文件名的后缀
* 返回: 返回文件名序列`<names>`的后缀序列, 如果文件没有后缀, 则返回空字串
* 示例:  

  ```makefile
  $(suffix src/foo.c src-1.0/bar.c hacks)
  #返回结果为".c .c"
  ```

`$(basename <names...>)`

* 名称: 取前缀函数, basename
* 功能: 从文件名序列`<names>`中取出各个文件名的前缀部分
* 返回: 返回文件名序列`<names>`的前缀序列, 如果文件没有前缀, 则返回空字串
* 示例:  

  ```makefile
  $(basename src/foo.c src-1.0/bar.c hacks)
  #返回结果为"src/foo src-1.0/bar hacks"
  ```

`$(addsuffix <suffix>,<names...>)`

* 名称: 加后缀函数, addsuffix
* 功能: 把后缀`<suffix>`加到`<names>`中的每个单词后面
* 返回: 返回加过后缀的文件名序列
* 示例:  

  ```makefile
  $(addsuffix .c,foo bar)
  #返回结果为"foo.c bar.c"
  ```

`$(addprefix <prefix>,<names...>)`

* 名称: 加前缀函数, addprefix
* 功能: 把前缀`<prefix>`加到`<names>`中的每个单词后面
* 返回: 返回加过前缀的文件名序列
* 示例:  

  ```makefile
  $(addprefix src/,foo bar)
  #返回结果为"src/foo src/bar"
  ```

`$(join <list1>,<list2>)`

* 名称: 连接函数, join
* 功能: 把`<list2>`中的单词对应地加到`<list1>`的单词后面. 如果`<list1>`的单词个数要比`<list2>`的多, 那么`<list1>`中的多出来的单词将保持原样. 如果`<list2>`的单词个数要比`<list1>`多, 那么`<list2>`多出来的单词将被复制到返回值中
* 返回: 返回连接过后的字符串
* 示例:  

  ```makefile
  $(join aaa bbb , 111 222 333)
  #返回结果为"aaa111 bbb222 333"
  ```

## 6.4 `foreach`函数

`$(foreach <var>,<list>,<text>)`

* 类似于unix shell中的for语句

* 功能: 把参数`<list>`中的单词逐一取出放到参数`<var>`所指定的变量中, 然后再执行`<text>`所包含的表达式; 每次执行`<text>`对应的表达式, 会返回一个字符串; 在循环过程中, `<text>`的所返回的每个字符串会以空格分隔.
  
  * `<var>`变量是该函数临时局部变量
* 返回: 当整个循环结束时, `<text>`所返回的每个字符串所组成的整个字符串将会是`foreach`函数的返回值
* 示例

  ```makefile
  names := a b c d
  files := $(foreach n,$(name),$(n).o)
  #返回结果为"a.o b.o c.o d.o"
  ```

## 6.5 `if`函数

`$(if <condition>,<then-part>)`

`$(if <condition>,<then-part>,<else-part>)`

* 类似于gnu make的ifeq
* 功能: `<condition>`返回为非空字符串(即true)时, 计算`<then-part>`; 否则计算`<else-part>`
* 返回: 若`<condition>`为true时, 返回`<then-part>`的值; 否则返回`<else-part>`的值(若有该部分), 或返回空字符串(没有`<else-part>`时)

## 6.6 `call`函数

`$(call <expression>,<parm1>,<parm2>,...)`

* 功能: `<expression>`中的变量, 如`$(1)`, `$(2)`,`$(3)`等, 被实参`<parm1>`,`<parm2>`, `<parm3>`依次替换

* 返回: `<expression>`被替换后的值

* 示例

  ```makefile
  reverse = $(2) $(1)
  foo = $(call reverse,a,b) #返回结果为"b a"
  ```

## 6.7 `origin`函数

`$(origin <variable>)`

* 功能: 告诉`<variable>`变量从哪里来
* 返回: 
  * 未定义: `undefined`
  * 默认定义: `default`
  * 环境变量: `environment`
  * 文件中定义: `file`
  * 命令行中定义: `command line`
  * 被override重新定义: `override`
  * 自动化变量: `automatic`

## 6.8 `shell`函数

`$(shell <command line>)`

* 功能: 通过系统shell调用`<command line>`, 与反引号`` ` ``的功能一样

* 返回: 执行`<command line>`命令行后的返回值

* 示例

  ```makefile
  contents := $(shell cat foo)
  files := $(shell echo *.c)
  # 等价于
  contents := `cat foo`
  files := `echo *.c`
  ```

## 6.9 控制make的函数

`$(error <text...>)`

* 功能: 产生一个致命错误, 导致make退出; `<text...>`是错误信息
  
* 把其定义在某个变量中的话, 可以在后续脚本中使用该变量以产生错误
  
* 示例

  ```makefile
  #1
  ifde ERROR_001
  $(error error is $(ERROR_001))
  endif
  #2
  ERR = $(error found an error!) # 不会马上产生错误
  .PHONY: err
  err:;$(ERR)	# 在使用变量时产生错误
  ```

`$(warning <text...>)`

* 功能: 与`error`函数类似, 它用于产生一段警告信息, 不会导致make的退出

# 7. 如何使用make命令

## 7.1 make的退出码

* `0`: 成功执行
* `1`: make运行时出现错误
* `2`: make使用`-q`选项, 且make使得某些目标不需要更新时, 返回2

## 7.2 make与目标

可以在make命令行中指定目标

* 如: `make clean`

在unix世界中, 软件发布时, 特别是GNU这种开源软件, 其makefile都包含了编译, 安装, 打包等功能

* 常见的伪目标
  * `all`: 编译所有目标
  * `clean`: 删除所有被make创建的文件
  * `install`: 安装已编译好的程序, 其实就是把目标执行文件拷贝到指定的目标中
  * `print`: 列出改变过的源文件
  * `tar`: 把源文件进行打包, 得到tar文件
  * `dist`: 创建一个压缩文件; 通常是把tar文件压缩成z文件, 或gz文件
  * `TAGS`: 更新所有目标, 以完整地重编译使用
  * `cheack`, `test`: 用于测试makefile的流程

## 7.3 make的命令行参数(选项)

语法: `make [OPTION]... [TARGET]...`

指定makefile

* `-f FILE`, `--file=FILE`, `--makefile==FILE`: 指定makefile

检查makefile

* `-n`, `--just-print`, `--dry-run`, `--recon`: 打印makefile中要执行的命令, 但不执行命令

* `-t`, `--touch`: 更新目标文件的修改时间, 但没有修改文件
* `-q`, `--question`:  检查makefile是否有给定目标; 若存在, 返回零值, 否则返回非零值
* `-W FILE`, `--what-if=FILE`, `--new-file=FILE`, `--assume-new=FILE`: 假定`FILE`文件被修改了; 通常与`-n`选项结合使用

# 8. 隐式规则implicit rule

## 8.1 使用隐式规则

> 对于某个目标, 若找不到相应的文件, 显式规则或隐式规则时, 会报错

何时会进行隐式规则推导

* 显式规则没有命令

  ```makefile
  target : prerequisite
  # 没有命名
  ```

* 目标的文件不存在, 且没有对应的显式规则

示例

```makefile
# 以下是某个makefile的全部内容
foo : foo.o bar.o
	cc -o foo foo.o bar.o $(CFLAGS) $(LDFLAGS)
```

* 示例中使用了一个隐式规则: 对于目标中后缀为`.o`的依赖, 将其后缀改为`.c`, 并使用`cc -c $(CFLAGS)  <.c>`来生成同名的`.o`文件

## 8.2 常用的内置隐式规则

**编译C程序的隐式规则**

* `<name>.o`的依赖会推导为`<name>.c`
* 生成命令是`$(CC) -c $(CPPFLAGS) $(CFLAGS)`

**编译C++程序的隐式规则**

* `<name>.o`的依赖会推导为`<name>.cc`或`<name>.c`等
  * 某些旧版本的make只支持`.cc`和`.c`的推导
* 生成命令是`$(CXX) -c $(CPPFLAGS) $(CFLAGS)`

编译Pascal程序的隐式规则

* `<name>.o`的依赖推导为`<name>.p`
* 生成命令是`$(PC) -c $(PFLAGS)`

编译Fortran/Ratfor程序的隐含规则

* `<name>.o`的依赖推导为`<name>.r`, `<name>.F`或`<name>.f`
* 生成命令是
  * `.f`, `$(FC) -c $(FFLAGS)`
  * `.F`, `$(FC) -c $(FFLAGS) $(CPPFLAGS)`
  * `.f`, `$(FC) -c $(FFLAGS) $(RFLAGS)`

预处理Fortran/Ratfor程序的隐含规则

* `<name>.f`的依赖推导为`<name>.r`或`<name>.F`
  * 只是将Ratfor或有预处理的Fortran程序转换到一个标准的Fortran程序
* 生成命令是
  * `.F`, `$(FC) -F $(CPPFLAGS) $(FFLAGS)`
  * `.r`, `$(FC) -F $(FFLAGS) $(RFLAGS)`

编译Modula-2程序的隐含规则

* 两个规则
  * `<name>.sym`的依赖推导为`<name>.def`
    * 生成命令是`$(M2C) $(M2FLAGS) $(DEFFLAGS)`
  * `<name>.o`的依赖推导为`<name>.mod`
    * 生成命令是`$(M2C) $(M2FLAGS) $(MODFLAGS)`

**汇编和汇编预处理的隐含规则**

* 两个规则
  * `<name>.o`的依赖推导为`<name>.s`
    * 默认使用编译器`as`
    * 生成命令是`$(AS) $(ASFLAGS)`
  * `<name>.s`的依赖推导为`<name>.S`
    * 默认使用C预编译器`cpp`
    * 生成命令是`$(AS) $(ASFLAGS)`

**链接Object文件的隐含规则**

* `<name>`的依赖推导为`<name>.o`
  * 通过运行C的编译器来运行链接程序(一般是`ld`)
* 生成命令是`$(CC) $(LDFLAGS) <name>.o $(LOADLIBES) $(LDLIBS)`

Yacc C程序时的隐含规则

* `<name>.c`的依赖推导为`<name>.y`
* 生成命令是`$(YACC) $(YFLAGS)`

Lex C程序时的隐含规则

* `<name>.c`的依赖推导为`<name>.l`
* 生成命令是`$(LEX) $(LFLAGS)`

Lex Ratfor程序时的隐含规则

* `<name>.r`的依赖推导为`<name>.l`
* 生成命令是`$(LEX) $(LFLAGS)`

从C程序、Yacc文件或Lex文件创建Lint库的隐含规则

* 对于`<name>.ln`, `<name>.y`和`<name>.l`, 其依赖推导为`<name>.c`
* 生成命令是`$(LINT) $(LINTFLAGS) $(CPPFLAGS) -I`

## 8.3 隐式规则与变量

在隐式规则中, 基本上都要使用一些预先设置的变量

* 可以在makefile中改变这些变量的值, 或在make命令行中传入, 或在环境变量中设置

### 关于命名的预设变量

| 变量名     | 作用                                                       |
| ---------- | ---------------------------------------------------------- |
| `AR`       | 函数库打包程序; 默认值为`ar`                               |
| `AS`       | 汇编语言编译程序; 默认值为`as`                             |
| `CC`       | C语言编译程序; 默认值为`cc`                                |
| `CXX`      | C++语言编译程序; 默认值为`g++`                             |
| `CO`       | 从RCS文件中扩展文件程序; 默认值为`co`                      |
| `CPP`      | C程序的预处理器, 且输出为标准输出设备; 默认值为`$(CC) -E`  |
| `FC`       | Fortran和Ratfor的编译器和预处理程序; 默认值为`f77`         |
| `GET`      | 从SCCS文件中扩展文件的程序; 默认值为`get`                  |
| `LEX`      | Lex方法分析器程序(用于C或Ratfor); 默认值为`lex`            |
| `PC`       | Pascal语言编译程序; 默认值为`pc`                           |
| `YACC`     | Yacc文法分析器(用于C); 默认值为`yacc`                      |
| `YACCR`    | Yacc文法分析器(用于Ratfor); 默认值为`yacc -r`              |
| `MAKEINFO` | 转换Texinfo源文件(.texi)到Info文件程序; 默认值为`makeinfo` |
| `TEX`      | 从TeX源文件创建TeX DVI文件的程序; 默认值为`tex`            |
| `TEXI2DVI` | 从Texinfo源文件创建TeX DVI 文件的程序; 默认值为`texi2dvi`  |
| `WEAVE`    | 转换Web到TeX的程序; 默认值为`weave`                        |
| `CWEAVE`   | 转换C Web 到TeX的程序; 默认值为`cweave`                    |
| `TANGLE`   | 转换Web到Pascal语言的程序; 默认值为`tangle`                |
| `CTANGLE`  | 转换C Web到C; 默认值为`ctangle`                            |
| `RM`       | 删除文件命令; 默认值为`rm -f`                              |

### 关于参数的预设变量

| 变量名     | 作用(没指明其默认值时, 则为空)                       |
| ---------- | ---------------------------------------------------- |
| `ARFLAGS`  | 函数库打包程序AR命令的参数; 默认值是“rv”             |
| `ASFLAGS`  | 汇编语言编译器参数; 当明显地调用`.s`或`.S`文件时使用 |
| `CFLAGS`   | C语言编译器参数                                      |
| `CXXFLAGS` | C++语言编译器参数                                    |
| `COFLAGS`  | RCS命令参数                                          |
| `CPPFLAGS` | C预处理器参数                                        |
| `FFLAGS`   | Fortran语言编译器参数                                |
| `GFLAGS`   | SCCS的`get`程序参数                                  |
| `LDFLAGS`  | 链接器参数                                           |
| `LFLAGS`   | Lex文法分析器参数                                    |
| `PFLAGS`   | Pascal语言编译器参数                                 |
| `RFLAGS`   | Ratfor程序的Fortran编译器参数                        |
| `YFLAGS`   | Yacc文法分析器参数                                   |

## 8.4 隐式规则链

一个目标可能会被一系列的隐式规则所作用; 这一系列隐式规则, 被称作隐式规则链

* 例如, 一个`<name>.o`的文件生成过程: 可能会是先被Yacc的`<name>.y`文件生成`<name>.c`, 然后再被C的编程生成得到`<name>.o`

在隐式规则链中,  除了最开始的目标外, 其他目标被称作中间目标

* make会尽可能地自动推导出能够生成目标的一切方法, 无论有多少中间目标
* 与一般的目标的区别:
  * 在中间目标对应的文件不存在时, 才会引发中间目标的规则
  * 在成功产生最终目标后, 所产生的中间目标文件都会以`rm -f`删除
* 可以显式地声明某个目标为中间目标: `.INTERMEDIATE: <target>...`
* 可以阻止删除中间目标文件: `.SECONDARY: <target>...`
* 以模式的方式使得某个目标成为伪目标`.PRECIOUS`的依赖, 能够保存其隐式规则链所生成的中间文件
  * 如`.PRECIOUS : %.o`, 使得所有`.o`文件的中间目标文件保存下来

在隐式规则链中, 每个目标最多出现一次, 以防止出现无限递归的情况

make会优化一些特殊的隐式规则, 从而没有生成中间文件

* 如从文件`foo.c`生成目标程序`foo`: 
  * 按照隐式规则有: `foo.c`=>`foo.o`=>`foo`
  * 优化后, 使用`cc -o foo foo.c`: `foo.c`->`foo`

## 8.5 模式规则

模式规则, 与一般的规则类似, 除了目标的定义中必须有`%`

* `%`表示长度任意的非空字符串
  * 目标中的`%`对文件名进行匹配
  * 可以在依赖中使用`%`字符; `%`字符的值等于目标中的`%`所匹配的字符串

* 注意: `%`的展开发生在变量和函数的展开之后; 变量和函数的展开发生在make载入makefile的时候, 而模式规则中的`%`繁盛在运行时

* 在模式规则中, 每当目标中的`%`与某个字符串匹配, 依赖中的`%`也相应确定下来, 然后make会去寻找依赖文件

模式规则

* 若模式规则中的目标模式与文件匹配, 且模式规则中的所有依赖都存在或能够构建, 可以使用模式规则来构建(make)一个该文件
* 影响优先级的因素:
  * 规则类型
    * 模式规则
    * 内置隐式规则
  * 依赖的构建
    * 依赖存在, 或能够通过规则构建
    * 依赖都通过隐式规则构建
  * 同一优先级时
    * 若多个规则在同一优先级, 选择最短stem的规则
    * 若多个规则有最短stem, 选择第一个找到的规则

示例

```makefile
%.o : %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@
```

### 自动化变量

由于惯例, 自动化变量通常带上`$`

* 如`$@`自动化变量, 其变量名实际上为`@`, 根据惯例将其写为`$@`

自动化变量只使用在规则的命令中

* 一般使用在显出规则和静态模式规则中; 在隐式规则中没有意义

| 自动化变量的使用 | 意义                                                         |
| ---------------- | ------------------------------------------------------------ |
| `$@`             | 规则的目标名称; 若目标是函数库成员时, 表示函数库的名字; 若有多个目标, 表示导致该规则执行的目标名称 |
| `$%`             | 属于函数库成员的目标; 若目标中没有函数库的成员, 为空         |
| `$<`             | 第一个依赖                                                   |
| `$?`             | 所有比目标新的依赖                                           |
| `$^`             | 所有依赖; 每个依赖的名称以空格分隔; 若依赖中有多个重复, 则只保留一个; 不包含order-only依赖 |
| `$+`             | 类似于`$^`, 除了它不去除重复的依赖目标                       |
| `$|`             | 所有order-only依赖; 以空格分隔                               |
| `$*`             | 在隐式规则中, 表示`%`所匹配的字符串; 若`%`匹配的字符串前有`/`, 则要加上前面的部分 |
|                  | 在静态模式规则中, 表示`%`所匹配的字符串                      |
|                  | 在显式规则中, 若目标带有可识别的后缀, 表示去除后缀的目标名; 否则为空 |

* `$@`, `$%`, `$<`, `$*`在展开时只有一个文件名
* `$?`, `$^`, `$|`, `$+`在展开时时一个文件列表

除了`$|`外, 以上自动化变量都有两个变种, 分别用于获取文件所在的目录, 和文件名; 

* 以`$@`为例: `$(@D)`和`$(@F)`
  * 若`$@`是`dir/foo.o`, 则`$(@D)`是`dir`, 而`$(@F)`是`foo.o`
  * 若`$@`是`foo.o`, 则`$(@D)`为空, 而`$(@F)`是`foo.o`

### 模式匹配

目标模式的形式: `<PREFIX>%<SUFFIX>`

* `<PREFIX>`和`<SUFFIX>`可以为空
* 在与模式匹配的字符串中, 除去`<PREFIX>`和`<SUFFIX>`的子字符串被称作stem(茎?)

目标模式的匹配过程

* 若目标模式中没有`/`, 则把进行匹配的字符串中的目录部分移除

* 进行模式匹配

* 在成功匹配后, 若有移除的目录, 将目录重新添加回去; 若某个依赖中有`%`, 也将目录加入到依赖名中

* 示例说明: 有模式规则`e%t : c%r`和文件`src/eat`

  * 文件名移除目录部分, 得到文件名`eat`
  * `eat`与模式`e%t`进行匹配; `%`与`a`匹配, 因此依赖的名称为`car`, stem的值为`a`

  * 然后重新添加目录部分, 因此依赖名为`src/car`, stem的值为`src/a`

### 重载隐式规则

定义一个新的模式规则可以重载其他隐式规则(包括内置隐式规则)

* 若打算取消一条内置隐式规则, 可以定义一个目标和依赖相同的模式规则, 但该模式规则没有命令

  ```makefile
  # 以下模式规则会取消关于汇编器的内置隐式规则
  %.o : %.s
  ```

### 终止的规则

```makefile
<target> :: <prerequisite>
	command
	...
```

* 对于终止的模式规则, 若其依赖存在, 或能够通过显式规则构建时, 才会考虑使用
* 终止的显式规则, 等同于一般的显式规则

### 任意匹配的模式规则

任意匹配规则(match-anything rule), 也就是目标为`%`的模式规则

```makefile
% : <prerequisite>
	command
	...
```

由于任意匹配规则能够"匹配任意字符串", make会花费大量时间才能考虑到任意匹配规则; 因此, make有以下限制:

* 非终止的任意匹配规则不能用于隐式规则的依赖
* 若某文件与其他隐式规则匹配, 非终止的任意匹配规则不能用于该文件

若使用任意匹配规则, 通常将其标记为终止

```makefile
% :: <prerequisite>
	command
	...
```

## 8.6 默认规则

可以定义一个没有依赖且终止的任意匹配规则, 作为默认规则

* 若某目标(或依赖)的文件不存在, 且没有显式规则或其他隐式规则, 该目标会与该默认规则匹配

```makefile
% ::
	command
	...
```

可以为`.DEFAULT`目标定义规则

* 若某目标(或依赖)的文件不存在, 或者没有显式规则或隐式规则, 那么该目标会使用`.DEFAULT`的规则, 若`.DEFAULT`规则的依赖存在或能够构建的话

* 清除`.DEFAULT`的定义: 只需要为`.DEFAULT`定义一个没有命令和依赖的规则即可

## 8.7 旧风格的后缀规则

后缀规则, 是一种旧式的定义隐式规则的方法

* 有两类后缀规则: 双后缀规则, 单后缀规则
* 后缀规则不允许有依赖; 否则,  被make认作显式规则

make通过已知的后缀列表来识别后缀规则的定义

* 目标是一个已知的后缀, 且没有依赖, 则被认作单后缀规则
* 目标是由两个已知的后缀组成, 且没有依赖, 则被认作双后缀规则

可以通过`.SUFFIXES`来添加或删除后缀

* 添加一些后缀到后缀列表后面

  ```makefile
  .SUFFIXES: .hack .win
  ```

* 删除所有已知的后缀

  ```makefile
  .SUFFIXES:	#没有依赖
  ```

## 双后缀规则

双后缀规则定义了一对后缀, 分别是目标的后缀和依赖的后缀

* 前面的是依赖的后缀, 后面的是目标的后缀

```makefile
.c.o:
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<
#相当于
%.o : %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<
```

## 单后缀规则

单后缀规则定义了一个后缀, 是依赖的后缀

* 该类规则与任意目标匹配
* 后缀规则不允许有依赖

```makefile
.c
#相当于
% : %.c
```

## 8.8 隐式规则的搜索算法

搜索隐式规则的情况

* 没有命令的终止规则
* 没有命令的显式规则
* 没有对应规则的依赖

以下是为某个目标`T`搜索隐式规则的过程: 

* 把`T`分成两部分, 分别是目录部分`d`和其余部分`n`
* 找出所有与`T`或`n`匹配的模式规则, 得到一个模式规则列表
* 若列表中有规则不是任意匹配规则, 或者`t`是某个隐式规则的依赖, 则移除所有非终止的任意匹配规则
* 从列表中移除没有命令的规则
* 对于列表中的每个规则
  1. 得到stem `s`; stem是`%`所匹配的非空部分
  2. 通过替换其中的`%`得到依赖的具体名字; 若目标模式中没有`/`, 在依赖的名字前添加`d`
  3. 检查所有依赖是否存在, 或者能够存在
     * 若一个文件名在makefile中定义为目标或显式规则的依赖, 则认为该文件能够存在
  4. 若所有依赖都存在或能够存在, 或者没有依赖, 则使用该规则
* 若还没有找到合适的模式规则, 对于列表中的每个规则进行以下步骤
  1. 若规则是终止的, 忽略它并查看下一条规则
  2. 计算依赖的具体名字
  3. 检查所有依赖是否存在或能够存在
  4. 对于每个不存在的依赖, 递归执行本算法, 以确定该依赖能否通过隐式规则得到
  5. 若所有依赖存在或能够存在, 或者能够通过隐式规则得到, 则使用该规则
* 若还没有找到合适的隐式规则, 检查是否有`.DEFAULT`规则; 若有, 使用该规则, 否则没有合适的隐式规则

在找到合适的隐式规则后, 

* 除了与`t`或`n`匹配的规则外, 对于其他规则的每个目标模式, 模式中的`%`被替换成`s`, 并存储得到的文件名直到执行完构建目标文件`t`的命令
* 在执行构建目标文件`t`的命令时, 自动化变量被设为相对应的目标或依赖
* 在命令执行结束后, 被存储的文件名被输入到数据库中, 并被标记成已经更新, 并且具有与文件t相同的更新状态

注意:

* makefile被读入时, 其中的后缀规则都转换成模式规则
* 对于目标是函数库的成员(形如`archive(member)`时, 该算法最多会执行两次
  * 第一次使用完整的目标名进行搜索
  * 若第一次找不到规则, 第二次使用`(member)`作为目标进行搜索

# 9. make与函数库文件(archive)

函数库文件(archive)

* 包含被称为成员的子文件

* 在unix下, 一般使用`ar`程序对其进行各种操作
* 用途是, 用于链接的子程序库

## 9.1 函数库成员作为目标

函数库文件中的成员可以用作目标或依赖

```makefile
archive(member ...)
```

* 该格式只能用于目标和依赖的定义, 不能用于命令中

* 只有`ar`或其他类似的命令能够进行函数库操作

示例

```makefile
foolib(hack.o) : hack.o
	ar cr foolib hack.o
```

```makefile
foolib(hack.o kludge.o) :
	...
#相当于
foolib(hack.o) foolib(kludge.o) :
	....
```

```makefile
#可以使用shell的文件通配符
foolib(*.o) :
	...
```

## 9.2 函数库成员目标的隐式规则

当make搜索形如`a(m)`的目标时, 会考虑与`a(m)`或`(m)`匹配的规则; 而make有一条目标为`(%)`的规则

* 这个规则会复制`m`文件到函数库`a`中, 从而更新目标`a(m)`

* 示例: 若当前目录没有makefile, 只有`bar.c`文件, 并在shell中调用`make "foo.a(bar.o)"`
  * 首先, 根据隐式规则, 由`bar.c`生成`bar.o`
  * 然后, 根据上述规则, 由`bar.o`生成`foo.a`

  ```makefile
  # 等价于
  cc -c bar.c -o bar.o
  ar r foo.a bar.o
  rm -f bar.o
  ```

虽然函数库成员的名字不可能有目录部分, 但make允许这么做

```makefile
# 目标为"foo.a(dir/file.o)"
ar r foo.a dir/file.o
```

## 9.3 函数库的注意事项

在进行函数库打包文件生成时, 请小心使用make的并行机制
* 如果多个ar命令在同一时间运行在同一个函数库打包文件上, 很可能会损坏这个函数库文件
* 在make未来的版本中会提供一种机制来避免并行操作发生在函数打包文件上

## 9.4 函数库的后缀规则

可以使用后缀规则来处理函数库文件

* 形如`.x.a`的后缀规则会转换成形如`(%.o) : %.x`的模式规则
  * 为了使得`.a`用作其他类型文件的后缀, 同时会转换成形如`%.a : %.x`的模式规则

```makefile
.c.a:
	...
#等价于
(%.o) : %.c
	...
```



