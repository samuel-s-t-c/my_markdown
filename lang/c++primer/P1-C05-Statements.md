# 总结

 在默认情况下语句按顺序执行; C++提供了几种语句类型:

* 条件执行: `if`, `switch`
* 循环: `while`, `for`, `do while`
* 跳转: `break`, `continue`, `return`, `goto`
* 异常处理: `throw`, `try`块



[toc]

# 1.简单语句*

表达式语句(**expression statement**): 就是一个表达式的末尾加上分号

* 动作: 执行表达式, 然后丢弃求值结果
* 用法: `expr;`
* 作用: 通常, 表达式语句中的表达式在求值时有副作用(**side effect**), 如给变量赋值, 打印一个结果等等

空语句(**null statement**): 最简单的语句, 只含有一个分号

* 动作: 无

* 用法: `;`

* 作用: 通常, 在语法上需要语句但逻辑上不需要的地方使用.

* 例子:

* ```c++
  //read until we hit end-of-file or find an input equal to sought
  while (cin >> s && s != sought)
      ;	//null statment
  ```

复合语句/块(**compound statements; blocks**): 用花括号括起来的(可能为空的)语句和声明的序列

* 动作: 形成一个作用域; 块在语法上是一条语句.
* 用法: `{statments;}`
* 作用: 在程序的某个地方, 语法上需要一条语句, 但是逻辑上需要多条语句, 应该使用块, 如`while`和`for`循环的循环体
* 说明: 
  * 在块中引入的名字只能在块内部以及嵌套在块中的子块里访问.
  * 块不以分号作为结束
  * 空块的作用等于空语句

# 2.语句作用域

可以在`if`, `switch`, `while`和`for`语句的控制结构内定义变量; 该变量在相应的语句作用域内.

* 定义在控制结构中的变量, 只在相应语句的内部可见, 而且在语句的结束之后变量也就超出作用域.

* 如果需要在控制语句外访问控制变量, 则变量必须定义在语句外.

# 3.条件语句

两种条件执行语句: `if`语句和`switch`语句

## 3.1`if`语句*

`if`语句: 

* 动作: 计算条件中的表达式, 根据条件中的结果是否为真, 决定是否执行另外一条语句.

* 用法: 

* ```c++
  //simple if statement
  if (condition)
      statement
  //if else statement
  if (condition)
      statement
  else
      statement2
  ```

  * `condition`必须用小括号包围; `condition`可以是表达式或者一个初始化变量声明, 只要表达式或变量的类型都能够转换成布尔类型
  * 如果`condition`为`true`, 执行`statement`. 当`statement`执行完后, 程序继续执行`if`语句后面的其他语句
  * 如果`condition`为`false`, 跳过`statement`, 如果为`if-else`语句, 还要执行`statement2`. 然后, 程序继续执行`if`语句后面的其他语句

* 说明:

  * 因为`if`语句也是语句, 所以可以在`if`语句中嵌套`if`语句
  * 悬垂else(**dangling else**)问题, 即嵌套`if`语句中, 很有可能`if`分支多于`else`分支
    * C++的解决方案: 规定`else`与离它最近的尚未匹配的`if`匹配

## 3.2`swicth`语句

`switch`语句: 

* 动作: 求值一个整型表达式, 然后根据结果从给定的执行路径中选择一条来执行

* 用法: 

* ```c++
  switch (expr) {
      case num1 :
          statement1
      case num2 :
          statement2
              break;
      default:
          statement3
  }
  ```

  * `expr`可以是表达式或初始化变量声明; 
  * `case`标签是指`case`关键字和对应的值, 如 `num1`, `num2`等
    * 可以有零个或多个`case`
    * 类型一定是整数常量表达式.
    * `case`标签的值不能相同, `default`是一种特殊的`case`标签; 最多只有一个`default`
  * 标签后面必须跟上一条语句或者另外一个标签
  
* 语句的具体动作

  * 首先, 对`expr`求值, 其结果会转换成整数类型, 并与每个`case`标签的值比较;
  * 如果`expr`的值与某个`case`标签的值匹配, 程序从该标签之后的第一条语句开始执行, **直到遇到一条`break`语句, 或者到达`switch`的结尾.**
  * 如果`expr`的值没有与任何`case`标签匹配时
    * 没有`default`标签, 控制将转移到`switch`语句外的第一条语句
    * 有`default`标签, 程序从该标签之后的第一条语句开始执行, 直到遇到一条`break`语句, 或者到达`switch`的结尾.

* 说明
  * `break`语句用于中断当前的控制;在`switch`语句中, 它把控制权转移到`switch`语句外面.
  * C++规定, 不允许这样的做法: 跳过了变量的初始化语句, 把控制转移到该变量仍然在域内(in scope)的位置.
  * 如果需要为某个`case`分支定义并初始化一个变量, 应该把变量定义在块内.

# 4.迭代语句

迭代语句, 也称为循环(**loop**), 重复执行操作直到满足某些条件为止.

三种迭代语句: `while`, `for`, `do while`

## 4.1`while`语句*

`while`语句

* 动作: 判断条件; 只要条件为`true`, 重复执行目标操作

* 用法:

* ```c++
  while (condition)
      statement
  ```

  * `condition`不能为空, 可以是表达式或带初始化的变量声明

* 语句的具体动作

  1. 首先对条件求值

  2. 如果条件为`true`, 执行`statement`, 然后将控制转移到`condition`
  3. 如果条件为`false`, 将控制转移到`while`语句外的第一条语句

## 4.2传统`for`语句*

`for`语句

* 动作: 判断条件; 只要条件为`true`, 重复执行目标操作

* 用法

* ```c++
  for (init-statement condition; expression)
      statement
  ```

  * `init-statement`必须是声明语句, 表达式语句或空语句;
  * `condition`必须是表达式, 初始化变量声明或者空的, 其结果的类型必须能够转换成布尔类型
  * `expression`必须是表达式, 初始化变量声明或者空的

* 语句的具体动作

  1. 执行一次`init-statement`; 
  2. 对`condition`求值
  3. 如果`condition`的结果为`true`, 则执行`statement`, 然后对`expression`求值, 最后将控制转移到`condition`.
  4. 如果`condition`的结果为`false`, 则将控制转移到`for`语句外的第一条语句

* 说明:

  * 在`for`语句中定义的变量的作用域是语句本身, 所以在语句外, 这种变量是不可见的
  * `condition`为空, 表示`condition`一直为`true`;

## 4.3范围`for`语句(C++11)*

范围`for`语句

* 动作: 遍历容器或其他序列中元素, 并执行给定的操作

* 用法:

* ```c++
  for (declaration : expression)
      statement
  ```

  * `expression`的结果必须表示一个序列
  * `declaratoin`定义一个变量, 其类型必须能够转换成序列中的元素类型

* 语句的具体动作: 

  1. 获得`expression`表示的序列的头迭代器和尾后迭代器, 分别存放在两个变量中, 假设为`beg`和`end`.

  2. 判断`beg`和`end`是否相等; 若相等, 则将控制转移到`for`语句外的第一条语句, 否则进行下一步操作.
  3. 控制变量被初始化为`beg`指向的元素
  4. 执行`statement`
  5. 递增`beg`迭代器
  6. 重复第2~5步骤直至两迭代器相等

* 说明:

  * 想要改变序列中的元素, 将控制变量定义为引用类型
  * 注意到范围`for`语句保存了尾后迭代器的值; 若在语句中对序列进行添删元素的操作, 保存的尾后迭代器的值就变得无效了.

## 4.4`do while`语句

`do while`语句

* 动作: 先执行给定的操作, 再判断条件; 只要条件为`true`, 重复执行目标操作

* 用法:

* ```c++
  do
      statement
  while (condition);
  ```

  * `condition`不能为空;
  * 不能在`condition`中定义变量;
  * 在`condition`使用的变量必须在语句外定义的;

* 语句的具体动作

  1. 执行`statement`
  2. 对`condition`求值
  3. 如果`condition`的结果为`true`, 将控制转移到`statement`.
  4. 如果`condition`的结果为`false`, 则将控制转移到`do while`语句外的第一条语句

# 5.跳转语句

跳转语句用于中断当前的执行流. C++有4种跳转语句: `break`, `continue`, `goto`, `return`

* `return`语句: 退出当前函数并返回一个值

## 5.1`break`语句

`break`语句

* 动作: 结束**最近的**`while`, `do while`, `for`或`switch`语句, 将控制转移到这些语句之后的第一条语句.
* 用法: 在`while`, `do while`, `for`或`switch`语句内部包含`break`语句; `break;`

## 5.2`continue`语句

`continue`语句

* 动作: 结束**最近的**迭代语句(`while`, `do while`, `for`)的当前迭代并开始下一轮迭代
* 用法: 在迭代语句内部包含`continue`语句; `continue;`
* 具体动作: 
  * 结束`statement`的执行, 并进入下一步
    * 对于`while`和`do while`语句, 将控制转移到`condition`的求值中.
    * 对于`for`语句, 将控制转换到`for`语句头的`expression`的求值中.
    * 对于范围`for`语句, 将控制转换到递增迭代器的语句中.

## 5.3`goto`语句

`goto`语句

* 动作: 无条件地跳转到同一函数中的给定位置

* 用法:

* ```c++
  goto lable;
  //In the same function
  lable:
  	statement	//labeled statement
  ```

  * `label`是用于标记语句的标签标识符. 标签标识符独立于变量或其他标识符的名字, 因此标签标识符可以和程序中其他实体的标识符使用同一名字而不会相互干扰

* 注意:

  * C++规定, 不允许这样的做法: 跳过了变量的初始化语句, 把控制转移到该变量仍然在域内(in scope)的位置.
  * 可以向后跳过一个已经执行的定义, 不过会导致该变量销毁并重新创建它.

# 6.try块和异常处理

异常(**exception**), 是指运行时的不属于正常运作范围的行为, 如丢失数据库连接, 得到预期外的输入等等. 如何处理异常是设计系统中最难部分之一.

异常处理(**exception handling**): 

* 程序的检测部分(**detecting part**)和处理部分(**handling part**)可以进行协作
* 异常处理的三个组成部分
  * `throw`表达式: 检测部分可以使用`throw`来表示遇到了它无法处理的情况
  * `try`块: 处理部分可以使用`try`块来处理异常; 在`try`块中的代码抛出异常, 然后`catch`子句处理异常
  * 一组异常类(**exception classes**): 用于在`throw`表达式和相关的`catch`子句传递异常的信息

## 6.1`throw`表达式

`throw`表达式

* 动作: 引发(**raise**)一个异常
* 用法: `throw expression`
  * `throw`是一个关键字
  * `expression`是表达式
* 具体动作: `expression`的结果作为异常被抛出; 
* 说明
  * 出现一个异常时, 程序将寻找匹配的`catch`子句; 若没找到匹配的`catch`子句, 将调用`terminate`标准库函数, 该函数会终止当前程序的执行.

## 6.2`try`块

`try`块

* 用法: 

* ```c++
  try {
      /*program-statements*/
  } catch (exception-declaration) {
      /*handler-statements*/
  } catch (exception-declaration) {
      /*handler-statements*/
  } //...
  ```

  * 以`try`关键字开始 , 后跟一个块, 然后是一个或多个`catch`子句(**catch clause**); 
  * `program-statements`表示程序的正常逻辑
  * `catch`子句, 也被称为异常处理代码(**exception handler**), 是由一个`catch`关键字, 被括起来的对象声明和一个块组成; 
    * 被括起来的对象声明, 称作异常声明(**exception declaration**)
    * `handler-statements`表示用于处理某种异常的逻辑

* 具体动作

  * 当`program-statements`中引发一个异常时, 判断该异常是否与`catch`子句匹配
  * 如果某个`catch`子句与异常匹配, 执行相关的块, 然后将控制转移到`try`块外的第一条语句
  * 如果在当前函数中没有匹配的`catch`子句时,
    * 在`main`函数中, 把控制转移到`terminate`标准库函数; `terminate`函数的具体行为与系统有关, 其作用是停止当前程序的执行. 一般情况下, 执行该函数将导致程序非正常退出.
    * 不在`main`函数中, 退出当前函数, 返回到caller并继续匹配; 若依旧没有匹配的`catch`子句, 继续沿着执行路径逐层回退并匹配.

* 说明

  * 和其他块一样, `try`块中的每个块内声明的变量在块外部无法被访问, 即某个块的变量在另一个块中是不可见的

## 6.3标准异常

c++标准库定义了一组异常类, 用于报告标准库函数遇到的问题, 也能在用户编写的程序中使用.

标准库中有4个头文件定义了异常类

* `exceptoin`头文件: 定义了一个最通用的异常类`exception`, 用于报告异常的发生, 没有提供任何额外的信息
* `new`头文件: 定义了一个异常类`bad_alloc`
* `type_info`头文件: 定义了一个异常类`bad_cast`
* `stdexcept`头文件: 见下表

| 类名               | 异常描述                                       |
| ------------------ | ---------------------------------------------- |
| `exception`        | 最通用的问题                                   |
| `runtime_error`    | 只有在运行时才能检测的问题                     |
| `range_error`      | 运行时错误: 生成的结果超出了有意义的值域范围   |
| `overflow_error`   | 运行时错误: 计算出现上溢                       |
| `underflow_error`  | 运行时错误: 计算出现下溢                       |
| `logic_error`      | 程序逻辑的错误                                 |
| `domain_error`     | 逻辑错误: 参数对应的结果不存在                 |
| `invalid_argument` | 逻辑错误: 无效参数                             |
| `length_error`     | 逻辑错误: 试图创建一个超出该类型最大长度的对象 |
| `out_of_range`     | 逻辑错误: 使用一个超出有效范围的值             |

异常类只定义了几种操作

* 创建异常类型的对象
  * `exception`, `bad_alloc`和`bad_cast`类型: 只支持默认初始化
  * 其他的异常类型: 只支持使用`string`或C风格字符串来初始化, 不能默认初始化
* 复制异常类型的对象
* 为异常类型的对象赋值
* `what`函数成员: 不接受参数, 返回类型是`const char*`; 返回指针指向的字符串用于提供关于异常的一些文本信息