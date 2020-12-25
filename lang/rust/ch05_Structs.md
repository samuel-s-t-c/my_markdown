# 句法总结

`struct`, 结构体, 一种自定义数据类型

[toc]

# 1.`struct`的定义和初始化

`struct`定义的形式

```rust
struct name {
    mem1: type,
    mem2: type,
    ...
}
```

* `{}`里面的`mem: type`对, 被称为字段(**field**)

* 例子

  ```rust
  struct User {
      username: String,
      email: String,
      sign_in_count: u64,
      active: bool,
  }
  ```

构造一个`struct`值与初始化

* `struct`的名字后跟`{}`; 在`{}`里面是`key: value`对
* 字段的声明顺序: 不必与定义中的声明顺序相同
* 构造`struct`的表达式可以作为函数的最后表达式

* 例子

  ```rust
  let user1 = User {
      email: String::from("someone@example.com"),
      username: String::from("someusername123"),
      active: true,
      sign_in_count: 1,
  };
  ```

  ```rust
  fn build_user(email: String, username: String) -> User {
      User {
          email: email,
          username: username,
          active: true,
          sign_in_count: 1,
      }
  }
  ```

获得`struct`中某个字段的值: 使用`.`符号

* 当`struct`变量是可变时, 它的字段的值才能被修改
* 不允许只让部分字段可变

* 例子

  ```rust
  let mut user1 = User {
      email: String::from("someone@example.com"),
      username: String::from("someusername123"),
      active: true,
      sign_in_count: 1,
  };
  
  user1.email = String::from("anotheremail@example.com");
  ```

## 1.1当变量和字段拥有相同的名字时可以使用字段初始简写

字段初始简写的句法

* 当变量名与字段名相同时, 可以在字段声明中可以省略`: valu`部分

```rust
fn build_user(email: String, username: String) -> User {
    User {
        email,
        username,
        active: true,
        sign_in_count: 1,
    }
}
```

## 1.2结构更新句法

结构更新句法

* 作用: 使用已存在的结构的部分或所有字段的值来初始化新结构的字段

* 具体: 未显式地指定值的字段, 都被给定的结构变量中的对应字段的值初始化

* 形式: 在`struct`构造表达式的花括号中的最后使用`..`加上另一个结构变量名

* 例子

  ```rust
  let user2 = User {
      email: String::from("another@example.com"),
      username: String::from("anotherusername567"),
      ..user1
  };
  ```

## 1.3元组结构tuple struct

元组结构

* 字段没有名字的结构
* 定义形式: `struct name(type_list);`

* 例子

  ```rust
  struct Color(i32, i32, i32);
  struct Point(i32, i32, i32);
  
  let black = Color(0, 0, 0);
  let origin = Point(0, 0, 0);
  ```

  * 注意: `black`和`origin`的类型是不同的; 

使用元组结构

* 像元组那样使用
  * 可以使用`(name_list)`形式来将它的字段分开
  * 可以使用`.`符号加索引值来访问它的字段值
  * 其他元组操作

## 1.4类单元结构

类单元结构, 是指没有任一字段的结构

* 它的行为像`()`单元类型

# 2.关于方法的句法

`derive`注解

* 添加很多对自定义类型有用的行为

## 2.1定义方法

定义方法

* 定义一个`impl`块: `impl`关键字加上结构名, 以表示为该结构添加实现
* 在`impl`块中定义方法:
  * 方法的第一个形参必须是`self`; `self`表示调用该方法的值
  * `self`也可以是引用, 即`&self`和`&mut self`

调用方法

* 某个实例使用`.`符号访问方法, 然后使用`()`符号调用; 在`()`里是所需的实参
  
* 例子: `rect1.area()`
  
* 在调用方法时, `.`符号前的值隐式地作为方法的第一个实参

*  在调用方法时有这样的特性: 自动引用和自动解引用;

  * 也就是说, 当调用方法, 如`obj.method()`时, Rust自动添加`&`, `&mut`或`*`, 使得`obj`与方法的签名匹配 

    ```rust
    //p1是一个变量; 方法要求一个引用
    p1.distance(&p2);
    //自动引用
    (&p1).distance(&p2);
    ```

例子

```rust
#[derive(Debug)]
struct Rectangle {
    width: u32,
    height: u32,
}

impl Rectangle {
    fn area(&self) -> u32 {
        self.width * self.height
    }
}

fn main() {
    let rect1 = Rectangle { width: 30, height: 50 };

    println!(
        "The area of the rectangle is {} square pixels.",
        rect1.area()
    );
}
```

## 2.2有多个形参的方法

方法的第一个形参`self`, 在调用时与调用它的值绑定; 在调用时的`()`中的第一个实参, 传递给方法的第二个形参, 依此类推

```rust
impl Rectangle {
    fn area(&self) -> u32 {
        self.width * self.height
    }

    fn can_hold(&self, other: &Rectangle) -> bool {
        self.width > other.width && self.height > other.height
    }
}

fn main() {
    let rect1 = Rectangle { width: 30, height: 50 };
    let rect2 = Rectangle { width: 10, height: 40 };
    let rect3 = Rectangle { width: 60, height: 45 };

    println!("Can rect1 hold rect2? {}", rect1.can_hold(&rect2));
    println!("Can rect1 hold rect3? {}", rect1.can_hold(&rect3));
}
```

## 2.3关联函数

在`impl`块中, 可以定义关联函数

* 关联函数的第一个形参不是`self`

* 通常, 关联函数被用作构造函数

使用结构名和`::`句法来调用关联函数

```rust
impl Rectangle {
    fn square(size: u32) -> Rectangle {
        Rectangle { width: size, height: size }
    }
}
// 调用关联函数
let sq = Rectangle::square(3); 
```

## 2.4多个`impl`块

对于一个结构, 可以有多个`impl`块, 