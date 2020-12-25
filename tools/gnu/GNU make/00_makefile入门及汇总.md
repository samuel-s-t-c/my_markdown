# 入门

## 示例

该工程中有3个头文件和8个c文件

```makefile
# 文件名为makefile
edit : main.o kbd.o command.o display.o /
       insert.o search.o files.o utils.o
	cc -o edit main.o kbd.o command.o display.o /
                insert.o search.o files.o utils.o

main.o : main.c defs.h
	cc -c main.c

kbd.o : kbd.c defs.h command.h
	cc -c kbd.c

command.o : command.c defs.h command.h
	cc -c command.c

display.o : display.c defs.h buffer.h
	cc -c display.c

insert.o : insert.c defs.h buffer.h
	cc -c insert.c

search.o : search.c defs.h buffer.h
	cc -c search.c

files.o : files.c defs.h buffer.h command.h
	cc -c files.c

utils.o : utils.c defs.h
	cc -c utils.c

clean :
	rm edit main.o kbd.o command.o display.o /
       insert.o search.o files.o utils.o
```

## 1.1 命令块的基本结构

```makefile
target ... : prerequisites ...
		command
		...
		...
```

* `target`是该命令块的目标; `target`可以是Object File, 也可以是执行文件, 还可以是一个标签(Label)

* `prerequisites`是执行该命令块的前提; 可以是某种文件或标签
* `command`是该命令块中所需执行的命令(任意shell命令)
  * 每条命令以一个`<TAB>`键为开头

* 一个命令块定义了文件的依赖关系

  *  `target`依赖于`prerequisites`
  *  `target`的生成规则是多行`command`

  * 默认情况下, 在`prerequisites`中有比`target`新的文件的时候, 才会执行所定义的`command`命令

Makefile的命名

* 通常直接命名为`makefMile`或`Makefile`
* `make`的`-f FILE`, `--file==FILE`, `--makefile==FILE`
  * 使用`FILE`文件作为Makefile

## 1.2 make命令的基本工作过程

只使用`make`命令, 不使用其他选项

* 在当前目录寻找名为`makefile`或`Makefile`的文件
* 在找到Makefile文件时, 将文件的第一个`target`作为最终的目标文件, 对其进行文件依赖处理
* 在进行文件依赖处理时, 在当前目录寻找`target`所对应的文件; 若有`prerequisites`, 则对其递归进行文件依赖处理
  * 若`target`所对应的文件不存在, 或者`prerequisites`所对应的文件比其新, 则会执行命令块的命令
* 在此过程中, 对于所定义的`command`命令的错误, make不会进行动作; 若出现与文件依赖相关的错误时, 如找不到被依赖的文件等, make会报错并退出

在示例所在目录中使用`make`命令

1. make在当前目录找到示例文件
2. make在文件中找到第一个`target`, 即`edit`
3. 如果`edit`文件不存在, 或是`edit`所依赖的后面的`.o`文件的文件修改时间要比edit这个文件新, 那么会执行后面所定义的命令
4. 如果`edit`所依赖的`.o`文件不存在, 那么会在当前文件中找目标为`.o`文件的依赖性;如果找到则再根据那一个规则生成`.o`文件

## 1.3 Makefile的变量

Makefile的变量, 类似于C语言中的宏, 是一个字符串

```makefile
# 变量声明
VAR = STRING ...
# 使用变量
$(VAR)
```

## 1.4 GNU make的自动推导

对于带有`.o`后缀的`target`, GNU的make会自动推导出相应的命令

```makefile
objects = main.o kbd.o command.o display.o /
	        insert.o search.o files.o utils.o

edit : $(objects)
	cc -o edit $(objects)

main.o : defs.h
kbd.o : defs.h command.h
command.o : defs.h command.h
display.o : defs.h buffer.h
insert.o : defs.h buffer.h
search.o : defs.h buffer.h
files.o : defs.h buffer.h command.h
utils.o : defs.h

.PHONY : clean
clean :
	rm edit $(objects)
```

* 说明: 例子中的`.PHONY`表示`clean`是个伪目标文件

## 1.5 清空目标文件

建议在Makefile中写一个清空目标文件的规则

* 通常将该规则命名为`clean`, 并放在makefile的末尾

* 这不仅便于重编译, 也能保持文件的清洁

```makefile
# 一般写法
clean :
	rm TARGETS
# 更好的写法
.PHONY : clean
clean :
	-rm TARGETS
```

# 汇总

