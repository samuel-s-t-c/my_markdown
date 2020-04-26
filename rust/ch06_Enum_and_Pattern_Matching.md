# 总结

枚举(**enumeration, enum**), 允许定义一个列举多个可能变体的类型

* `Option`枚举类

* 模式匹配(**pattern matching**)和`match`表达式

* `if let`结构

[toc]

# 1. 定义枚举

枚举类, 用于枚举多个可能的变体(**variant**)

* 枚举类的变量的值只能是变体之中的一个

定义枚举类

* 使用关键字`enum`, 后跟名字和一个花括号
* 花括号中列出所有可能的变体; 

```rust
enum IpAddrKind {
    V4,
    V6,
}
```

## 1.1枚举值

同一枚举类的变体都是同一类型的; 可以通过`::`来使用变体

```rust
let four = IpAddrKind::V4;
let six = IpAddrKind::V6;
```

在定义枚举类时, 可以为变体关联零个或多个类型

* 每个变体都可以有不同的类型和不同数量的关联数据

```rust
enum IpAddr {
    V4(u8, u8, u8, u8),
    V6(String),
}

let home = IpAddr::V4(127, 0, 0, 1);

let loopback = IpAddr::V6(String::from("::1"));
```

```rust
enum Message {
    Quit,
    Move { x: i32, y: i32 },
    Write(String),
    ChangeColor(i32, i32, i32),
}
```

* `Message`枚举类中的变体
  * `Quit` has no data associated with it at all.
  * `Move` includes an anonymous struct inside it.
  * `Write` includes a single `String`.
  * `ChangeColor` includes three `i32` values.

可以为枚举类定义方法

```rust
impl Message {
    fn call(&self) {
        // method body would be defined here
    }
}

let m = Message::Write(String::from("hello"));
m.call();
```

## 1.2标准库枚举类`Option`

`Option`枚举类, 用于表示某个值是否存在

* 标准库定义如下

  ```rust
  pub enum Option<T> {
      None,
      Some(T),
  }
  ```

* 包括在Rust的序幕(**prelude**)
* 可以不需要`Option::`前缀直接使用变体`None`和`Some`

`<T>`句法

* `<T>`是一个泛式类型形参

使用`Option`

* 在使用`None`时, 需要有类型注解, 因为无法从`None`中推断出`Option`的泛式类型形参

* 在使用`Some`时, 可以没有类型注解, 因为可以从`Some`的关联值中推断泛式类型形参

  ```rust
  let some_number = Some(5);
  let some_string = Some("a string");
  
  let absent_number: Option<i32> = None;
  ```

### 1.2.1`Option`的意义

* 在一些语言中,  有`null`的存在, 用于表示当前的变量没有有效值; 
  * 对值为`null`的变量进行运算是没有意义的, 而且有时是一种错误, 因为`null`表示无效
  *  由于`null`本身是一个值, 在语法上允许对`null`进行运算, 这导致编译器无法检测出这种错误
* 在Rust中, 没有`null`; 而是用`Option`来表达类似的概念
  * 当存在一个可能为空的值时必须使用`Option`来表示该值
    * 因为在`Option`中才有空这个概念, 没有`null`值
  * `Option<T>`与`T`是两个不同的类型, 因此编译器不允许像`T`那样使用`Option<T>`
  * 除了`Option`类型之外, 如果其他类型的变量有一个值, 则该值总是有效的, 因为没有`null`
  * 当变量的类型是`Option`时, 才需要确定值是否存在, 并且编译器会检查是否有对应的处理

## 1.2.2`Option`的方法

请查看手册

# 2. `match`控制流操作符

`match`表达式

* 形式

  ```rust
  match value {
      pattern1 => expression1,
      pattern2 => expression1,
      ...
  }
  ```

  * 按顺序将`value`与模式匹配, 当匹配成功, 则执行对应的表达式

* 说明

  * `match`表达式的结果是匹配分支中的表达式结果
  * `value`的类型没有限制

## 2.1与值绑定的模式

`match`表达式中, 匹配分支中的模式可以与对应的值中的部分组成绑定

* 具体是, 在模式中声明一个或多个名字, 与对应的部分组成绑定

```rust
enum UsState {
    Alabama,
    Alaska,
    // --snip--
}

enum Coin {
    Penny,
    Nickel,
    Dime,
    Quarter(UsState),
}

fn value_in_cents(coin: Coin) -> u8 {
    match coin {
        Coin::Penny => 1,
        Coin::Nickel => 5,
        Coin::Dime => 10,
        Coin::Quarter(state) => {
            println!("State quarter from {:?}!", state);
            25
        },
    }
}
```

## 2.2`Option<T>`与`match`

```rust
fn plus_one(x: Option<i32>) -> Option<i32> {
    match x {
        None => None,
        Some(i) => Some(i + 1),
    }
}

let five = Some(5);
let six = plus_one(five);
let none = plus_one(None);
```

## 2.3匹配的分支要全面

`match`中的分支必须覆盖所有可能的情况

## 2.4`_`占位符

`_`占位符, 与对应的值绑定, 表示不打算使用该值

* 模式中声明的名字, 表示与值绑定, 在对应的表达式中使用该名字

```rust
let some_u8_value = 0u8;
match some_u8_value {
    1 => println!("one"),
    3 => println!("three"),
    5 => println!("five"),
    7 => println!("seven"),
    _ => (),
}
```

# 3. `if let`的控制流

`if let`接受一个模式和一个表达式, 两者用`=`分隔

* 当模式与表达式匹配时, 执行后面的代码块; 否则, 跳过代码块, 或者执行`else`的代码块
* 注意: 两个代码块的结果要兼容; 如果没有`else`代码块, 则要返回`()`

* 形式

  ```rust
  if let pattern = expression {
      //do something
  }
  //if let的另一种形式
  if let pattern = expression {
      //do something
  } else {
      //do something
  }
  ```

例子: 

以下两个代码段的效果是相同的

```rust
let some_u8_value = Some(0u8);
if let Some(3) = some_u8_value {
    println!("three");
}
```

```rust
let some_u8_value = Some(0u8);
match some_u8_value {
    Some(3) => println!("three"),
    _ => (),
}
```

以下两个代码段的效果是相同的

```rust
let mut count = 0;
if let Coin::Quarter(state) = coin {
    println!("State quarter from {:?}!", state);
} else {
    count += 1;
}
```

```rust
let mut count = 0;
match coin {
    Coin::Quarter(state) => println!("State quarter from {:?}!", state),
    _ => count += 1,
}
```

