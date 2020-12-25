# 总结

Rust中的特殊语法: 模式(*pattern*), 用于匹配类型的结构

[toc]

# 1.可以使用*pattern*的位置

*Pattern*可以在Rust中的很多地方出现; 本章给出所有可以使用*pattern*的位置

## 1.1`match` arms

`match`表达式的形式

```rust
match VALUE {
    PATTERN => EXPRESSION,
    PATTERN => EXPRESSION,
    PATTERN => EXPRESSION,
}
```

* 在每个arm中都有一个*pattern*

注意: 使用`match`表达式时要*exhaustive*

## 1.2`if let`表达式

`if let`表达式是`match`表达式的语法糖

* 例子

  ```rust
   if let Some(color) = favorite_color {
          println!("Using your favorite color, {}, as the background", color);
  }
  ```

## 1.3`while let`条件循环

`while let`的结构与`it let`类似

* 只要条件中的模式一直匹配, 则一直执行`while`循环

* 形式

  ```rust
  while let pattern = expression {
      /*loop body*/
  }
  ```

* 例子

  ```rust
      let mut stack = Vec::new();
  
      stack.push(1);
      stack.push(2);
      stack.push(3);
  
      while let Some(top) = stack.pop() {
          println!("{}", top);
      }
  ```

## 1.4`for`循环

`for`表达式中`for x in y`的`x`是模式

```rust
    let v = vec!['a', 'b', 'c'];

    for (index, value) in v.iter().enumerate() {
        println!("{} is at index {}", value, index);
    }
```

## 1.5`let`语句

`let`语句的形式

* `let PATTERN = EXPRESSION;`

例子

```rust
let (x, y, z) = (1, 2, 3);
```

## 1.6函数形参

函数形参是模式

* 包括闭包中的函数形参

例子

```rust
fn foo(x: i32) {
    // code goes here
}
```

* `x`是模式

```rust
fn print_coordinates(&(x, y): &(i32, i32)) {
    println!("Current location: ({}, {})", x, y);
}
```

* `&(x, y)`是模式

# 2.Refutability: *pattern*是否会匹配失败

*pattern*有两种形式

* *refutable*(可反驳的) *pattern*: 对于某些可能的值, 会匹配失败
  * 如`if let Some(x) = a_value`中, 如果`a_value`是`None`值, 则`Some(x)`模式匹配失败
* *irrefutable*(不可反驳的) *pattern*:  能够匹配任何可能的值
  * 如`let x = 5;`中的`x`可以匹配任何值

函数形参, `let`语句和`for`循环只能接受*irrefutable pattern*

* 原因: 匹配失败对这些语句来说是无意义的;

`if let`和`while let`表达式可以接受*irrefutable pattern*和*refutable pattern*

* 如果接受*rrefutable pattern*, 编译器对此给出警告, 因为这些表达式的本意是处理可能的失败

*match arm*

* 除了最后那个*arm* , 其他*arm*必须使用*refutable pattern*

# 3.*pattern*语法

## 3.1匹配字面量

可以与字面量*pattern*进行匹配

```rust
let x = 1;

match x {
	1 => println!("one"),
	2 => println!("two"),
	3 => println!("three"),
	_ => println!("anything"),
}
```

## 3.2匹配命名变量

命名变量*pattern*是*irrefutable*

```rust
let x = Some(5);
let y = 10;

match x {
	Some(50) => println!("Got 50"),
	Some(y) => println!("Matched, y = {:?}", y),
	_ => println!("Default case, x = {:?}", x),
}
```

* `match`表达式中的`y`是命名变量*pattern*; 它覆盖了外部作用域中的`y`变量
* 如果想在`match`表达式中比较外部`x`和`y`的值, 需要使用*match guard conditional*

## 3.3多个*pattern*

使用`|`拼接多个`pattern`

```rust
let x = 1;

match x {
	1 | 2 => println!("one or two"),
	3 => println!("three"),
	_ => println!("anything"),
}
```

## 3.4范围匹配: `..=`

`..=`, 只能用于数值或`char`值

* 表示一个闭区间
* 如`1..=4`, 等于`1 | 2 | 3| 4`

注意: 编译时会检查范围是否为空

```rust
let x = 'j';

match x {
	'a'..='j' => println!("early ASCII letter"),
	'k'..='z' => println!("late ASCII letter"),
	_ => println!("something else"),
}
```

## 3.5解构语法

可以使用*pattern*以解构结构体, 枚举, 元组和指向它们的引用

### 解构`struct`

解构`struct`的*pattern*形式:

```rust
struct_name {field_name:pattern, ...}
struct_name {field_name,..}
```

* 两种形式可以混用

例子

```rust
struct Point {
    x: i32,
    y: i32,
}

fn main() {
    let p = Point { x: 0, y: 7 };

    let Point { x: a, y: b } = p;
    assert_eq!(0, a);
    assert_eq!(7, b);
}
```

* 创建两个变量`a`和`b`, 分别对应`p`中的`x`和`y`字段
* `a`和`b`是命名变量*pattern*

```rust
struct Point {
    x: i32,
    y: i32,
}

fn main() {
    let p = Point { x: 0, y: 7 };

    let Point { x, y } = p;
    assert_eq!(0, x);
    assert_eq!(7, y);
}
```

* 创建两个变量`x`和`y`, 分别对应`p`中的`x`和`y`字段
* 第9行的`x`和`y`是命名变量*pattern*

```rust
struct Point {
    x: i32,
    y: i32,
}

fn main() {
    let p = Point { x: 0, y: 7 };

    match p {
        Point { x, y: 0 } => println!("On the x axis at {}", x),
        Point { x: 0, y } => println!("On the y axis at {}", y),
        Point { x, y } => println!("On neither axis: ({}, {})", x, y),
    }
}
```

* 第10, 11行中使用字面量*pattern

### 解构`enum`

解构`enum`值的*pattern*形式需要与该值定义的方式相对应

例子

```rust
enum Message {
    Quit,
    Move { x: i32, y: i32 },
    Write(String),
    ChangeColor(i32, i32, i32),
}

fn main() {
    let msg = Message::ChangeColor(0, 160, 255);

    match msg {
        Message::Quit => {
            println!("The Quit variant has no data to destructure.")
        }
        Message::Move { x, y } => {
            println!(
                "Move in the x direction {} and in the y direction {}",
                x, y
            );
        }
        Message::Write(text) => println!("Text message: {}", text),
        Message::ChangeColor(r, g, b) => println!(
            "Change the color to red {}, green {}, and blue {}",
            r, g, b
        ),
    }
}
```

* `Message`的值都有不同的定义方式, 因此对应的*pattern*有不同的形式

### 解构嵌套的`struct`和`enum`, 元组

可以解构嵌套*item*

例子

```rust
let ((feet, inches), Point { x, y }) = ((3, 10), Point { x: 3, y: -10 });
```

## 3.6忽略*pattern*中的值

### `_`下划线: 通配符*pattern*

`_`下划线符号

* 一个通配符*pattern*, 可以匹配任意值, 但不进行绑定

可以忽略整个值

```rust
fn foo(_: i32, y: i32) {
    println!("This code only uses the y parameter: {}", y);
}

fn main() {
    foo(3, 4);
}
```

可以忽略部分值

```rust
let mut setting_value = Some(5);
let new_setting_value = Some(10);

match (setting_value, new_setting_value) {
	(Some(_), Some(_)) => {
		println!("Can't overwrite an existing customized value");
	}
	_ => {
	setting_value = new_setting_value;
	}
}

println!("setting is {:?}", setting_value);
```

### 下划线开头的变量名: 忽略警告

如果创建一个变量却没有在其他地方使用过, Rust通常会发出警告因为这可能是一个bug. 如果是真的打算这么做,  可以以下划线作为名字的开头声明变量; 这样做的话, 是告诉Rust这是一个未使用的变量

```rust
fn main() {
    let _x = 5;
}
```

注意: 以下划线开头的名字, 仍然会绑定一个值

### `..`: 忽略剩余的值

`..`符号作为*pattern*时, 会忽略剩余的任何没有显式匹配的部分, 即不进行绑定

* 使用`..`时只要是无歧义的就可以了

```rust
fn main() {
    let numbers = (2, 4, 8, 16, 32);

    match numbers {
        (first, .., last) => {
            println!("Some numbers: {}, {}", first, last);
        }
    }
}
```

## 3.7*match guard*: 提供额外条件判断

*match guard* , 是指在*match arm*的*pattern*后的额外`if`条件

* 形式

  ```rust
  match expression {
      pattern if condition => expression,
      ...
  }
  ```

* 在与*pattern*匹配成功后必须满足*match guard*才能执行该*arm*的表达式

例子

```rust
let x = 4;
let y = false;

match x {
	4 | 5 | 6 if y => println!("yes"),
	_ => println!("no"),
}
```

## 3.8`@`绑定

在某个值与模式匹配成功后, `@`运算符会创建一个变量以存放匹配的值

```rust
fn main() {
    enum Message {
        Hello { id: i32 },
    }

    let msg = Message::Hello { id: 5 };

    match msg {
        Message::Hello {
            id: id_variable @ 3..=7,
        } => println!("Found an id in range: {}", id_variable),
        Message::Hello { id: 10..=12 } => {
            println!("Found an id in another range")
        }
        Message::Hello { id } => println!("Found some other id: {}", id),
    }
}
```

