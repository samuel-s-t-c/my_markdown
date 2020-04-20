# 总结

介绍Rust程序中通用的基础概念, 在其他很多程序语言也有出现

[toc]

# 1.变量和可变性mutability

默认情况下, 变量的值是不可变的.

* 使用`let`关键字和可选的其他关键字, 然后加上名字
* 可以显式指定变量为可变的: 在变量名前加上`mut`
* 提供值时可以没有类型注解(**type annotation**) 
  * 编译器可以从提供的值中推断类型
* 没有提供值时必须注解类型(**annotate a type**)
* 形式如下

```rust
let name: type;
let name = init_value;
let name: type = init_value;
let mut name: type;
let mut name = init_value;
let mut name: type = init_value;
```

## 1.1变量和常量的区别

使用`const`然后加上名字以指定一个常量

* 必须指定值的类型
* 值只能是常量表达式, 即必须在编译时可得

* 不可在常量上使用`mut`
* 形式如下

```rust
const name: type = value;
```

常量可以在任一作用域中声明

## 1.2遮蔽Shadowing

可以声明一个新的变量, 该变量名与之前存在的变量的名字相同; 新的变量会遮蔽(**shadow**)旧的同名变量

* 也就是说在新变量声明后该名字表示新变量

例子

```rust
fn main() {
    let x = 5;

    let x = x + 1;

    let x = x * 2;

    println!("The value of x is: {}", x);
}
```

* 在第一个`let`语句中, `x`是一个不可变的变量, 值为5
* 在第二个`let`语句中, `x`是一个不可变的变量, 值为6
* 在第三个`let`语句中, `x`是一个不可变的变量, 值为12
* 这三个`x`是不同的变量

```rust
#![allow(unused_variables)]
fn main() {
let spaces = "   ";
let spaces = spaces.len();
}
```

# 2.数据类型

Rust中的每个值都有一个确定的数据类型

* 编译器经常可以从值中推断出我们想使用的类型

* 如果推断的类型有多种可能时, 必须有类型注解

  ```rust
  #![allow(unused_variables)]
  fn main() {
  let guess: u32 = "42".parse().expect("Not a number!");
  }
  ```

  * 例子中的`parse()`, 编译器可以推断出多种可能的类型, 此时必须有类型注解

## 2.1标量类型Scalar Types

标量类型, 用于表示一个单一的值; Rust有四种内置的标量类型

* 整数类型
* 浮点数类型
* 布尔类型
* 字符类型

### 2.1.1整数类型

| Length  | Signed  | Unsigned |
| ------- | ------- | -------- |
| 8-bit   | `i8`    | `u8`     |
| 16-bit  | `i16`   | `u16`    |
| 32-bit  | `i32`   | `u32`    |
| 64-bit  | `i64`   | `u64`    |
| 128-bit | `i128`  | `u128`   |
| arch    | `isize` | `usize`  |

* `i`表示带符号的整数类型, 以two's complement的方式存储
* `u`表示无符号的整数类型
* 数字表示所占用的位(**bit**)的数量

* `isize`和`usize`的长度取决于程序运行时的计算机的架构
  * 当程序运行时的计算机是64位架构, 则长度为64位
  * 当程序运行时的计算机是32位架构, 则长度为32位

#### 整数字面量的表示形式

| Number literals  | Example       |
| ---------------- | ------------- |
| Decimal          | `98_222`;     |
| Hex              | `0xff`        |
| Octal            | `0o77`        |
| Binary           | `0b1111_0000` |
| Byte (`u8` only) | `b'A'`        |

* 除了`Byte`字面量外, 其他的字面量允许有一个类型后缀, 如`57u8`, 和将`_`作为分隔符, 如`1_000`

* 默认情况下, 字面量的类型为`i32`

#### 整数溢出

当在debug模式下编译时, 程序中包含一个对整数溢出的检查, 这会导致出现`panic` 

* Rust使用`panicking`/ `panic`术语, 表示程序因错误退出

当在`release`模式下编译时, 程序中没有对整数溢出的检查; 如果出现整数溢出, 则发生*two’s complement wrapping*

* 依赖整数溢出的`wrapping`行为, 这通常认为是一个错误; 如果想要显式地`wrap`,  应该使用标准库类型`Wrapping`

### 2.1.2浮点数类型

两种初级的浮点数类型: `f32`, `f64`

* 浮点数的默认类型是`f64`
* 浮点数按照IEEE-754标准表示

### 2.1.3数值运算

五种基础的数学运算: 加法`+`, 减法`-`, 乘法`*`, 除法`/`, 求余`%`

```rust
fn main() {
    // addition
    let sum = 5 + 10;

    // subtraction
    let difference = 95.5 - 4.3;

    // multiplication
    let product = 4 * 30;

    // division
    let quotient = 56.7 / 32.2;

    // remainder
    let remainder = 43 % 5;
}
```

### 2.1.4布尔类型

`bool`布尔类型的两个可能值: `true`和`false`

* 尺寸是一个字节

### 2.1.5字符类型

`char`字符类型, 尺寸是4字节, 表示一个Unicode标量值

* 字符字面量被单引号`'`包围
* 字符串字面量被双引号`"`包围

## 2.2复合类型

两种初级复合类型: `tuple`和数组

### 2.2.1`Tuple`类型

通常将不同类型的值放在一起时使用`tuple`类型

* `tuple`有固定的长度: 一旦被声明, 长度不能改变

创建一个`tuple`时, `tuple`的值是一个被圆括号包围的逗号分隔的列表

```rust
fn main() {
    let tup: (i32, f64, u8) = (500, 6.4, 1);//可选的类型注解
}
```

为了得到`tuple`中的某个值

* 可以使用模式匹配来解构(**destructure**)一个`tuple`
* 可以使用句号`.`后跟索引值来访问一个`tuple`元素; 索引从0开始

```rust
fn main() {
    let tup = (500, 6.4, 1);

    let (x, y, z) = tup;

    println!("The value of y is: {}", y);	// y的值是6.4
    
    let x: (i32, f64, u8) = (500, 6.4, 1);

    let five_hundred = x.0;

    let six_point_four = x.1;

    let one = x.2;
}
```

### 2.2.2数组类型

通常将同一类型的多个值放在一起时使用数组类型

* 数组有固定的长度
* 数组类型: `[elem_type; size]`

创建一个数组时,  数组的值是一个被方括号包围的逗号分隔的列表

* 初始化数组时的另一种方式: `[value; size]`, 表示`size`个相同的值, 该值为`value`

```rust
fn main() {
    let a: [i32; 5] = [1, 2, 3, 4, 5];	//可选的类型注解
    let a = [3; 5];	//	等于[3, 3, 3, 3, 3]
}
```

#### 访问数组元素

数组是在内存中被分配到栈上的一段连续空间

* 可以按索引值访问, 形如`array[index]`; 索引从0开始计数

  ```rust
  fn main() {
      let a = [1, 2, 3, 4, 5];
  
      let first = a[0];
      let second = a[1];
  }
  ```

当使用索引访问数值元素时, 

* 编译器会检查给定的索引; 如果索引大于或等于数组的长度, 引发`panic`
* 当索引值在运行时才能得知, 编译器则不会检查

# 3.函数

使用`fn`关键字来声明一个函数

* Rust的函数名和变量名的规范风格是`snake case`, 也就是所有字母都是小写, 使用下划线来分隔单词

函数定义的形式: `fn name(param_list) -> ret_type { func_body }`

* 形参列表可以为空
* 没有返回值时, 可以省略`-> ret_type`

函数调用

* 函数名后跟实参, 形式如`func_name(args)`

Rust不在乎函数定义的先后顺序

## 3.1函数形参

函数形参是函数签名的一部分

函数形参列表

* 可为空
* 必须为每个形参声明类型
* 有多个形参时, 用逗号分隔形参声明

## 3.2函数体

函数体由零条或多条语句组成

* 可以用表达式作为结尾

#### 重点: Rust的语句和表达式的区别

* 返回值
  * 语句不会返回值
    * 函数定义是语句, `let y = 6;`等变量定义是语句
  * 表达式在求值后返回一个结果的值
    * `5 + 6`是表达式, 返回一个值为整数`11`的结果
    * 函数调用, 宏调用, `{}`块都是表达式
* 表达式可以是语句的部分
  * `let y = 6;`中的`6`是一个返回值`6`的表达式
* 结束的分号
  * 大多数语句在结尾处有一个分号
  * 表达式在结尾处都没有分号

## 3.3有返回值的函数

函数体内最后一条表达式的值, 隐式地作为函数返回值

也可以使用`return`来显式地指定函数返回值

```rust
fn main() {
    let x = plus_one(5);

    println!("The value of x is: {}", x);
}

fn plus_one(x: i32) -> i32 {
    x + 1
}
```

* 注意: `x + 1`没有分号, 不是一条语句, 而是一个表达式

# 4.注释

单行注释`//`

* `//`后的字符都被编译器忽略

# 5.控制流

## 5.1`if`表达式

`if`表达式的形式

```rust
//if形式
if condi 
	{block1}
//if-else形式
if condi 
	{block1} 
else
	{block2}
```

* `condi`表示条件, 其结果必须是`bool`
* 与`if`表达式关联的`{}`块, 有时被称为手臂(**arm**)

* `if`表达式返回`{}`块的结果
  * 条件为`true`时, 执行`block1`并返回对应的结果
  * 条件为`false`时, 执行`block2`并返回对应的结果; 如果没有`else`时, 直接返回`()`, 一个空的`tuple`
* 每个分支的`{}`块的结果必须有兼容的类型
*  `if`表达式只执行第一个条件为`true`的`{}`块, 而且不再检查剩下的条件

`else-if`表达式, 是`if`和`else`的合并

```rust
if condi1
	{block1}
else if condi2
	{block2}
else
	{block3}
//等价于
if condi1
	{block1}
else {
    if condi2
    	{block2}
    else
    	{block3}
}
```

### 5.1.1 在语句中使用`if`表达式

`if`是一个表达式, 因此可以在语句中使用

例子

```rust
fn main() {
    let condition = true;
    let number = if condition {
        5
    } else {
        6
    };

    println!("The value of number is: {}", number);
}
```

## 5.2循坏

三种循环: `loop`, `while`, `for`

### 5.2.1 使用`loop`重复执行代码

`loop`不断执行后面的`{}`块,直到显式停止

* 可使用`break`关键字显式停止

```rust
fn main() {
    loop {
        println!("again!");
        break;
    }
}
```

从`loop`中返回一个值: 在`break`后跟所需的返回值

```rust
fn main() {
    let mut counter = 0;

    let result = loop {
        counter += 1;

        if counter == 10 {
            break counter * 2;
        }
    };

    println!("The result is {}", result);
}
```

### 5.2.2 使用`while`执行条件循环

条件为真时继续执行代码, 否则退出循环

例子

```rust
fn main() {
    let mut number = 3;

    while number != 0 {
        println!("{}!", number);

        number -= 1;
    }

    println!("LIFTOFF!!!");
}
```

### 5.2.3 使用`for`遍历一个集合

形式:

```rust
for elem in iter
{ block }
```

* 为`iter`迭代器指向的每个元素执行`{}`块

例子

```rust
// 例子1
fn main() {
    let a = [10, 20, 30, 40, 50];

    for element in a.iter() {
        println!("the value is: {}", element);
    }
}
// 例子2
fn main() {
    for number in (1..4).rev() {
        println!("{}!", number);
    }
    println!("LIFTOFF!!!");
}

```

* 例子1`a.iter`返回一个数组`a`的迭代器
* 例子2中, `(1..4)`是一个标准库类`Range`, 表示从1到3的整数; 方法`rev`表示反向迭代