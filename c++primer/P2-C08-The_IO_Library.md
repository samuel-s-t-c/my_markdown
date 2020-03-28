# 总结

IO类

* 标准IO: `iostream`

* 文件IO: `fstream`
* `string`流; `stringstream`

[toc]

# 1.IO类*

> io, 是input和output的缩写

io类型, 也称为流类型

| 头文件     | 类型                                                    |
| ---------- | ------------------------------------------------------- |
| `iostream` | `istream`, `wistream`: 从流读取数据                     |
|            | `ostream`, `wostream`: 向流写入数据                     |
|            | `iostream`, `wiostream`: 读取和写入流                   |
| `fstream`  | `ifstream`, `wifstream`: 从文件读取数据                 |
|            | `ofstream`, `wofstream`: 向文件写入数据                 |
|            | `fstream`, `wfstream`: 读取和写入文件                   |
| `sstream`  | `istringstream`, `wistringstream`: 从`string`中读取数据 |
|            | `ostringstream`, `wostringstream`: 向`string`写入数据   |
|            | `stringstream`, `wstringstream`: 读取和写入`string`     |

* io类的对象默认关联到用户的控制台窗口
* 表中以`w`开头的io类型用于处理宽字符(**wide characters**); 相应的对象有`wcin`, `wcout`, `wcerr`等等
* io类型间的关系是继承关系
  * 具体地, `ifstream`和`istringstream`继承于`istream`, `ofstream`和`ostringstream`继承于`ostream`.

## 1.1 io对象*

* 不能拷贝IO类型的对象
  * 因此, 在函数中形参或返回类型不能是IO类型; 
  * 在函数中通常使用指向IO类型的引用类型; 因为读写一个io对象会改变其状态, 所以该引用不是`const`的

* `istream`类
  * 对象`cin`,  `cerr`, `log`
  * `>>`运算符: 从`istream`中读取数据
  * `getline(is, s)`: 从`istream`中读取一行字符, 把除了换行符的字符都存储到给定的`string`
* `ostream`类
  * 对象`cout`
  * `<<`运算符: 向`ostream`中写入数据

## 1.2条件状态

>  IO操作的本质导致一个问题: 可能发生错误. 

IO类定义一些表示条件状态的标志和用于操纵条件状态的函数; 流的条件状态能表示该流

| 条件状态的标志/函数 | 描述                                                         |
| ------------------- | ------------------------------------------------------------ |
| `strm::iostate`     | `strm`表示io类名; `iostate`是一种机器相关的整型, 表示流的条件状态 |
| `strm::badbit`      | 表示流已崩溃的`strm::iostate`类型的值                        |
| `strm::failbit`     | 表示一个io操作失败的`strm::iostate`类型的值                  |
| `strm::eofbit`      | 表示流到达eof的`strm::iostate`类型的值                       |
| `strm::goodbit`     | 表示流未处于错误状态的`strm::iostate`类型的值, 该值保证为零  |
| `s.eof()`           | `s`表示某个流类型的对象; 如果`eofbit`置位, 返回`true`        |
| `s.fail()`          | 如果`failbit`或`badbit`置位, 返回`true`                      |
| `s.bad()`           | 如果`badbit`置位, 返回`true`                                 |
| `s.good()`          | 如果流处于有效状态, 返回`true`                               |
| `s.clear()`         | 将流`s`中的所有条件状态位复位; 流`s`的状态设为有效. 返回`void` |
| `s.clear(flags)`    | `flags`是一个`strm::iostate`值;把流的条件状态设为`flags`,返回`void` |
| `s.setstate(flags)` | 把`flags`中的条件状态位置位; 返回`void`                      |
| `s.rdstate()`       | 返回当前的条件状态, 类型为`strm::iostate`                    |

流的条件状态: 错误状态和有效状态

* 一旦流发生错误, 流处于错误状态, 后续对该流的io操作都会失败
* 只有流处于有效状态时才能在该流上执行io操作
  * 因此, 建议在使用一个流之前检查它是否处于有效状态

`iostate`类型: 

* 一种机器相关的整型, 用于表示流的条件状态, 它的值是作为一个位集合使用的;

* 四个`iostate`类型的`constexpr`值: 表示特定的位模式, 其含义是不同的io条件状态
  * `badbit`: 表示一种系统级别的错误, 如不可恢复的读错误或写错误; 一旦`badbit`被置位, 流就无法再使用
  * `failbit`: 表示发生可恢复的错误, 如期望读取数值类型的数据却读出一个字符数据, 到达文件结束位置
  * `eofbit`: 表示到达文件结束位置; 
    * 到达文件结束位置时, `failbit`和`eofbit`都会被置位
  * `goodbit`: 表示未发生错误; 其值保证为零
  * 如果`badbit`, `failbit`和`eofbit`任一被置位, 流处于错误状态;

### 查询流的状态

查询流的具体状态

* `eof`成员函数: 当条件状态中`eodbit`被置位时, 返回`true`, 否则返回`false`
* `fail`成员函数: 当条件状态中`failbit`或`badbit`被置位时, 返回`true`, 否则返回`false`
  * 注意: `badbit`被置位, `fail`函数也会返回`true`.
* `bad`成员函数: 当条件状态中`badbit`被置位时, 返回`true`, 否则返回`false`

查询流的总体状态

* 直接把流作为条件使用; 当流处于有效状态时自动转换为`true`, 否则为`false`
* `good`成员函数: 当流处于有效状态, 返回`true`, 否则为`false`
* `fail`成员函数: `fail`函数可以判断流的总体状态, 原因如下
  * 到达文件结束位置时, `failbit`和`eofbit`被置位
  * 当`failbit`或`badbit`被置位时, `fail`函数返回`true`, 否则为`false`

### 管理条件状态

* `rdstate`成员函数: 返回一个`iostate`值, 表示流的当前条件状态
* `setstate`成员函数: 置位给定的条件位, 表示发生了对应的错误
* `clear`成员函数:
  * 不接受参数的版本: 清楚所有错误标志位, 使流处于有效状态
  * 接受一个`iostate`值: 把流的条件状态设为该值

## 1.3管理输出缓冲

标准库中的每个输出流都管理一个缓冲, 用来保存程序读写的数据.

* 缓冲的使用能够允许操作系统将多个程序的输出操作组合成单一的系统级写操作
* 因为向设备写入可能十分耗时, 多次程序级输出操作组合成一次系统级写操作, 这可以带来很大的性能提升

导致缓冲刷新(**flushing buffer**)的情况

* 程序正常退出: 刷新输出缓冲是`main`函数的`return`操作的一部分
* 当输出缓冲区满了, 在写入新值之前刷新输出缓冲
* 使用操纵符(如`endl`)显式地刷新输出缓冲
* 使用`unibuf`操纵符设置流的内部状态, 导致在每个输出操作之后清空输出缓冲;
  * 默认情况下, 对`cerr`是设置`unibut`的
* 一个输出流`a`可能被另一个流`b`关联; 在这种情况下, 当读或写流`b`时, 输出流`a`的输出缓冲会被刷新.
  * 默认情况下, `cin`和`cerr`都和`cout`关联; 因此, 读`cin`或写`ceer`都会导致`cout`的缓冲被刷新

### 刷新输出缓冲区

三种可以刷新输出缓冲的操纵符, 其名字都在`std`命名空间

* `endl`: 写入一个换行符, 然后刷新输出缓冲
* `flush`: 刷新输出缓冲
* `ends`: 写入一个空字符, 然后刷新输出缓冲

```c++
cout << "hi!" << endl;
cout << "hi!" << flush;
cout << "hi!" << ends;
```

### `unitbuf`操纵符

`unitbuf`操纵符: 使得输出流在每次写操作后都进行刷新

`nounitbuf`操纵符: 重置输出流, 让系统管理决定何时刷新缓冲

```c++
cout << unitbuf;
cout << nounitbuf;
```

### 关联的流

>  把一个输出流`a`与另一个流`b`关联; 在这种情况下, 当读或写流`b`时, 输出流`a`的输出缓冲会被刷新.

每个流最多关联到一个流; 一个输出流可以被多个流关联

`tie`成员函数: 

* 不接受参数的版本: 形如`x.tie()`
  * 如果对象关联到一个输出流, 返回一个指向该输出流的指针
* 接受一个指向`ostream`的指针的版本: 形如`x.tie(&o)`, 流`x`关联到输出流`o`
  * 把对象关联到给定的`ostream`对象, 返回一个指向之前关联输出流的指针(如果有的话)

```c++
//cin默认关联到cout
cin.tie();							//返回cout
ostream *old_tie = cin.tie(nullptr);//cin不再关联cout, old_tie指向cout
cin.tie(&cerr);						//cin关联到cerr
cin.tie(old_tie);					//cin不再关联cerr, 关联到cout
```

# 2.文件输入输出*

头文件`fstream`定义的三个类型:

* `ifstream`: 从一个给定文件读取数据, 继承`istream`
* `ofstream`: 向一个给定文件写入数据, 继承`ostream`
* `fstream`: 读写给定文件, 继承`iostream`

这些类型继承了头文件`iostream`中的类型的操作, 还有定义一些操作来管理与流关联的文件

* `fstream`特有的操作

| 操作                      | 描述                                                         |
| ------------------------- | ------------------------------------------------------------ |
| `fstream fstrm;`          | 创建未绑定的文件流; `fstream`是头文件`fstream`中定义的一个类型 |
| `fstream fstrm(s);`       | 创建一个文件流, 绑定以默认模式打开文件`s`; `explicit`构造函数 |
| `fstream fstrm(s, mode);` | 创建一个文件流, 绑定一个以`mode`模式打开名为`s`的文件        |
| `fstrm.open(s)`           | 以默认模式打开名为`s`的文件, 将文件与`fstrm`绑定; 返回`void` |
| `fstrm.open(s, mode)`     | 以`mode`模式打开名为`s`的文件, 将文件与`fstrm`绑定; 返回`void` |
| `fstrm.close()`           | 关闭与`fstrm`绑定的文件; 返回`void`                          |
| `fstrm.is_open()`         | 如果与`fstrm`绑定的文件成功打开且还没关闭, 返回`true`, 否则返回`false` |

* 默认的文件模式依赖于文件流的类型

## 2.1 定义和使用文件流对象*

创建文件流: 有3种方式

* 默认初始化: 创建一个未绑定的文件流

* 提供一个文件名`s`: 创建一个文件流, 并自动调用`open(s)`; 

  * 文件名`s`可以是C风格字符串, (C++11标准)也可以是`string`.

* 提供文件名`s`和模式名`mode`: 创建一个文件流,  并自动调用`open(s, mode)`

* ```c++
  ifstream in(ifile);
  ofstream out;
  ```

`open`成员函数

* 作用: 该函数完成一些所需的系统相关的操作, 来定位给定的文件, 并视情况以读或写模式打开文件

* 调用可能会失败

  * 如果调用`open`失败, `failbit`被置位, 之后不能使用该文件流.
  * 建议检查`open`的调用是否成功

* 例子:

  ```c++
  ifstream in(ifile);
  ofstream out;
  out.open(ifile + ".copy");
  if (out) {		//检查out是否成功打开文件
    /*使用out的代码*/
  }
  ```

`close`成员函数: 

* 作用: 关闭文件流关联的文件

关于使用文件流的说明

* 一旦一个文件流已经打开, 它就与对应文件保持关联; 对一个已经打开且没有关闭的文件流调用`open`会失败
  * 想要把一个已经打开的文件流关联到另一个文件, 必须首先关闭已经打开的文件; 一旦文件关闭成功, 我们就可以打开新的文件

  * 例子

    ```c++
    in.close();
    in.open(ifile + "2");
    ```

* 在要求使用父类对象的地方, 可以使用子类对象; 子类对象继承父类的操作
  * 如果有一个函数接受一个`ostream&`参数, 在调用该函数时可以传递一个`ofstream`实参; 类似的, `istream&`和`ifstream`也是如此
  * `ofstream`对象支持使用`<<`运算符写入数据; `ifstream`支持使用`>>`运算符读取数据

* 文件流对象被销毁时, 自动调用`close`

## 2.2文件模式*

每个文件流都有相应的文件模式(**file mode**), 用来指出如何使用文件

| 文件模式 | 含义                         |
| -------- | ---------------------------- |
| `in`     | 为了输出而打开文件           |
| `out`    | 为了输出而打开文件           |
| `app`    | 每次写入前定位到文件末尾     |
| `ate`    | 打开文件后立即定位到文件末尾 |
| `trunc`  | 截断文件                     |
| `binary` | 以二进制模式执行io操作       |

每次打开文件时都会显式或隐式地设置文件模式, 可以指定零个或多个模式; 需要注意如下规则

* `out`: 只能被`ofstream`和`fstream`对象指定
* `in`: 只能被`ifstream`和`fstream`对象指定
* `trunc`: 当在`out`被指定时才能指定`trunc`
* `app`: 如果没有显式指定`trunc`就能指定`app`; 只要指定了`app`, 文件总是以`out`模式打开, 即使没有显式地指定`out`模式
* 默认情况下, 即使没有指定`trunc`, 以`out`模式打开的文件也会被截断.想要保留文件的内容, 要么指定`app`, 要么指定`in`.
* `ate`, `binary`: 可用于任何类型的文件流对象, 且可以与其他任何文件模式组合使用

文件流类型的默认文件模式

* `ifstream`: `in`
* `ofstream`: `out`
* `fstream`: `in|out`

例子

```c++
ofstream out;
out.open("scratchpad", ofstream::out | ofstream::app);//mode is out and app
out.close();
out.open("precious");//mode implicitly out and trunc
out.close();
```

# 3.`string`流

`sstream`头文件定义三种类型来支持内存IO, 可以在`string`对象上读写数据

* `istringstream`: 从`string`读取数据, 继承`istream`
* `ostringstream`: 向`string`写入数据, 继承`ostream`
* `stringstream`: 读写`string`, 继承`iostream`

`stringstream`特有的操作

| 操作               | 描述                                                         |
| ------------------ | ------------------------------------------------------------ |
| `sstream strm;`    | `sstream`表示头文件`sstream`定义的类型之一; 创建未绑定的`stringstream` |
| `sstream strm(s);` | 创建一个`stringstream`, 存放`string s`的副本; `explicit`构造函数 |
| `strm.str()`       | 返回`strm`所保存的`string`的拷贝                             |
| `strm.str(s)`      | 把`string s`拷贝到`strm`中; 返回`void`                       |