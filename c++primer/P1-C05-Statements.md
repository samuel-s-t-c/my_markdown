# 总结

语句是顺序执行的

* 条件执行, 循环, 跳转
* 异常处理



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
  * `case`标签是指`case`关键字和对应的值, 如 `num1`, `num2`等, 类型一定是整数常量表达式.
    * `case`标签的值不能相同, `default`是一种特殊的`case`标签.
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

* 动作: 只要条件为`true`, 重复执行目标操作

* 用法:

* ```c++
  while (condition)
      statement
  ```

  * `condition`不能为空, 可以是表达式或带初始化的变量声明

* 语句的具体动作

  1. 首先对条件求值

  2. 如果条件为`true`, 执行`statement`, 然后重复第1条
  3. 如果条件为`false`, 将控制转移到`while`语句外的第一条语句

## 4.2传统`for`语句

## 4.3范围`for`语句

## 4.4`do while`语句

# 5.跳转语句

# 6.try块和异常处理