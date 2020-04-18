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

* 在一些语言中,  有`null`的存在, 用于表示当前的对象没有有效值; 
  * 对值为`null`的对象进行运算是没有意义的, 而且有时是一种错误, 因为`null`表示无效
  *  由于`null`本身是一个值, 在语法上允许对`null`进行运算, 这导致编译器无法检测出这种错误
* 在Rust中, 没有`null`; 而是用`Option`来表达类似的概念
  * 当存在一个可能为空的值时必须使用`Option`来表示该值
    * 因为在`Option`中才有空这个概念, 没有`null`值
  * `Option<T>`与`T`是两个不同的类型, 因此编译器不允许像`T`那样使用`Option<T>`
  * 除了`Option`类型之外, 如果其他类型的对象有一个值, 则该值总是有效的, 因为没有`null`
  * 当对象的类型是`Option`时, 才需要确定值是否存在, 并且编译器会检查是否有对应的处理

## 1.2.2`Option`的方法

请查看手册

# 2. `match`控制流操作符



# 3. `if let`的控制流