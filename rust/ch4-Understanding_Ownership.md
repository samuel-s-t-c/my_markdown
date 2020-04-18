# 总结

* 所有权(**ownership**): Rust中最独特的特性, 使得Rust没有垃圾回收器也能保证内存安全

* 借用(**borrowing**)
* 切片(**slice**)
* 如何在内存中存放数据

[toc]

# 1.拥有权

## 1.1拥有权规则

* 在Rust中的每个值都有一个对应的变量; 该变量被称为拥有者(**owner**)
* 在每个时刻中, 只能有一个拥有者
* 当某个拥有者离开作用域, 它的值也会被丢弃

## 1.2变量作用域

> 作用域, 是指程序中的被分划的区域
>
> 某个对象的作用域, 是指程序中该对象为有效的区域.

变量的作用域是从它声明所在处到当前作用域的结尾为止

```rust
{                      // s is not valid here, it’s not yet declared
    let s = "hello";   // s is valid from this point forward

    // do stuff with s
}                      // this scope is now over, and s is no longer valid
```

* 当变量`s`进入作用域, 也就是声明`s`, `s`变得有效
* `s`在离开作用域之前依旧是有效的

## 1.3内存和分配

> `String`类型, 表示一个可变长度的字符串

以`String`类型为例: 使用`String`时, 因为在编译时不知道其需要多少空间, 我们需要在堆中为其分配空间, 这意味着

* 在运行时要向操作系统请求分配内存空间
* 当我们不需要`String`时, 我们要将所占用的内存空间返回给操作系统

对于请求内存空间, 与大多数编程语言一样, 由我们自己负责完成; 

对于返回内存空间, 

* 带有垃圾回收器(**garbage collector, GC**)的编程语言: 由GC来负责
* 没有GC的编程语言: 由程序员来负责
* Rust: 当变量离开作用域时返回内存空间

```rust
{
    let s = String::from("hello"); // s is valid from this point forward

    // do stuff with s
}                                  // this scope is now over, and s is no
                                   // longer valid
```

当一个变量离开作用域时, Rust为其调用一个特殊的函数`drop`, 以返回该变量所占用的内存空间

* 在C++中, 这种在对象寿命结束时释放资源的模式, 被称为"资源获取就是初始化", (**Resource Acquisition Is Initialization, RAII**)
  * 初始化操作在对象的构造函数中
  * 释放资源的操作在对象的析构函数中
  * 使用适当的异常处理来应对对象构造期间的异常

* `drop`函数, 与C++中的析构函数类似

### 1.3.1移动`move`

有些类型的变量在赋值/初始化时隐式地执行移动操作, 而不是执行类似深拷贝的操作

* 移动操作: 将一个对象所占用的资源的拥有权给予另一个对象
* 执行移动操作后, 旧的变量是无效的, 不能再次使用.

* 原因: 为了运行时性能

```rust
let s1 = String::from("hello");
let s2 = s1;
let x = 5;
let y = x;
```

* 例子中的`String`类型的变量执行移动操作; `i32`类型的变量`x`和`y`执行拷贝操作

### 1.3.2克隆`clone`

对于在赋值/初始化时执行移动的类型, 如果想要执行深拷贝, 应该使用`clone`方法

* 规范: 对于自定义类型, 如果有深拷贝的操作时, 应该将其定义为`clone`方法

```rust
let s1 = String::from("hello");
let s2 = s1.clone();
println!("s1 = {}, s2 = {}", s1, s2);
```

### 1.3.3拷贝`copy`

有些类型的变量在赋值/初始化时执行拷贝操作

* 执行拷贝操作后, 两个变量都是有效的

特殊的注解

* `Copy`特征, 用于说明某个类型的数据存储在栈里;
  * 不能对有`Drop`特征的类型进行`Copy`注解; 如果对其添加`Copy`注解, 编译时出错

通用的规则

* 任一标量值都可以有`Copy`
* 需要分配或者有任一堆资源的值不能有`Copy`

## 1.4拥有权和函数

给一个函数传递值, 类似于将一个变量赋予另一个变量

* 传递函数实参时, 与赋值一样, 将执行拷贝或移动操作

```rust
fn main() {
    let s = String::from("hello");  // s comes into scope

    takes_ownership(s);             // s's value moves into the function...
                                    // ... and so is no longer valid here

    let x = 5;                      // x comes into scope

    makes_copy(x);                  // x would move into the function,
                                    // but i32 is Copy, so it’s okay to still
                                    // use x afterward

} // Here, x goes out of scope, then s. But because s's value was moved, nothing
  // special happens.

fn takes_ownership(some_string: String) { // some_string comes into scope
    println!("{}", some_string);
} // Here, some_string goes out of scope and `drop` is called. The backing
  // memory is freed.

fn makes_copy(some_integer: i32) { // some_integer comes into scope
    println!("{}", some_integer);
} // Here, some_integer goes out of scope. Nothing special happens.
```

## 1.5返回值与作用域

从函数中返回一个值, 也是会转移拥有权

```rust
fn main() {
    let s1 = gives_ownership();         // gives_ownership moves its return
                                        // value into s1

    let s2 = String::from("hello");     // s2 comes into scope

    let s3 = takes_and_gives_back(s2);  // s2 is moved into
                                        // takes_and_gives_back, which also
                                        // moves its return value into s3
} // Here, s3 goes out of scope and is dropped. s2 goes out of scope but was
  // moved, so nothing happens. s1 goes out of scope and is dropped.

fn gives_ownership() -> String {             // gives_ownership will move its
                                             // return value into the function
                                             // that calls it

    let some_string = String::from("hello"); // some_string comes into scope

    some_string                              // some_string is returned and
                                             // moves out to the calling
                                             // function
}

// takes_and_gives_back will take a String and return one
fn takes_and_gives_back(a_string: String) -> String { // a_string comes into
                                                      // scope

    a_string  // a_string is returned and moves out to the calling function
}
```

# 2.引用和借用

`&`加上变量名, 形如`&name`, 创建一个指向该变量的值的引用

* 引用不会占有该变量的值, 因此
  * 该引用离开作用域时不会`drop`其指向的值
* 默认情况下, 不能通过引用来改变指向的值

在函数签名的形参类型注解中, 使用`&`加上类型名, 形如`&type`, 表示该形参的类型是引用

* 向函数传递一个引用时, 被称为借用(**borrowing**), 因为并没有发生拥有权转移
  * 函数的形参借用实参的值 

引用的作用域是: 从它声明处到最后一次使用的地方

```rust
fn main() {
    let s1 = String::from("hello");

    let len = calculate_length(&s1);

    println!("The length of '{}' is {}.", s1, len);
}

fn calculate_length(s: &String) -> usize { // s is a reference to a String
    s.len()
} // Here, s goes out of scope. But because it does not have ownership of what
  // it refers to, nothing happens.
```

* `&s1`是一个指向`s1`值的引用

## 2.1引用与可变性

> 默认情况下, 不能通过引用来改变指向的值

可以显式地创建一个可变引用, 形如`&mut name`

* 可以通过可变引用来改变指向的值

* 限制
  * 可变引用只能指向可变的变量
  * 对于一份数据, 它在一个**引用作用域**中只能有一个可变引用
  * 对于一份数据, 它在一个**引用作用域**中如果存在不可变引用, 则不能有可变引用
    * 原因: 防止data race

例子

```rust
let mut s = String::from("hello");

let r1 = &mut s;
let r2 = &mut s;

println!("{}, {}", r1, r2);	//error: 在一个引用作用域中同时有两个可变引用
```



```rust
let mut s = String::from("hello");

let r1 = &s; // no problem
let r2 = &s; // no problem
let r3 = &mut s; // BIG PROBLEM

println!("{}, {}, and {}", r1, r2, r3);
```



```rust
let mut s = String::from("hello");

let r1 = &s; // no problem
let r2 = &s; // no problem
println!("{} and {}", r1, r2);
// r1 and r2 are no longer used after this point

let r3 = &mut s; // no problem
println!("{}", r3);
```

* 注意这个例子

## 2.2悬空引用问题

Rust编译器保证不会出现悬空引用

* 悬空引用: 指向一个不存在的值的引用

```rust
fn main() {
    let reference_to_nothing = dangle();
}

fn dangle() -> &String { // dangle returns a reference to a String

    let s = String::from("hello"); // s is a new String

    &s // we return a reference to the String, s
} // Here, s goes out of scope, and is dropped. Its memory goes away.
  // Danger!
```

* 编译器报错

# 3.`slice`类型

`slice`类型是原始类型

## 3.1字符串切片string slice

`&str`类型

* `&str`, 也就是字符串切片, 是一个引用, 指向一个`String`的某个部分

* 形式: `&str[starting_index..ending_index]`
  * `starting_index`是该切片中的第一位置, `ending_index`是该切片中的尾后位置
* 在切片内部, 保存了起始位置和切片的长度
* `..`范围语法
  * 如果`starting_index`为0时, 可以省略它, 形如`&s[..end]`
  * 如果`ending_index`为`String`的长度, 可以省略它, 形如`&s[beg..]`
  * 如果同时省略两个索引值, 即`&s[..]`, 等价于`&s[0..len]`, 表示一个指向整个`String`的切片

```rust
let s = String::from("hello world");

let hello = &s[0..5];
let world = &s[6..11];

let slice = &s[0..2];
let slice = &s[..2];

let len = s.len();
let slice = &s[3..len];
let slice = &s[3..];

let slice = &s[0..len];
let slice = &s[..];
```

注意:

* 字符串切片必须出现在有效的UTF-8字符的边界处; 如果试图在多字节字符的中间创建一个切片, 程序将会错误退出

#### 3.1.1字符串字面量与`&str`

```rust
let s = "Hello, world!";
```

* `s`的类型是`&str`
* 字符串字面量的类型也是`&str`

### 3.1.2将`&str`作为形参类型

将形参定义为`&str`, 使得该函数更加通用

## 3.2其他切片

数组切片, 类型为`&[elem_type]`

```rust
let a = [1, 2, 3, 4, 5];

let slice = &a[1..3];
```

* `slice`的类型是`&[i32]`

可以在其他所有集合中使用切片