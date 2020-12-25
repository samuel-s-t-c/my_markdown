# 总结

错误可分为

* 可恢复的错误, 如文件未找到错误
  * `Result<T, E>`类型处理该类错误
* 不可恢复的错误, 如试图访问数组外的内存
  * `panic!`宏用于停止执行当出现不可恢复的错误时

[toc]

# 1.不可恢复错误和`panic!`

默认情况下, 当执行`panic!`宏时, 程序将打印一条错误信息, 展开并清理栈内存, 然后退出执行

* `panic!`最常用的情况是: 检测到某种bug, 但程序员不清楚如何处理它
* 执行`panic!`时默认执行栈展开; 另一种选择是中止(**abort**), 即不清理栈并直接退出程序
  * 具体做法: 在`Cargo.toml`的`[profile]`下的合适区域添加`panic = 'abort'`
    * `[profile]`下的区域有`[profile.release]`, `[profile.debug]`

## 使用`panic!`的回溯

在运行程序时, 将`RUST_BACKTRACE`环境变量设为非零值, 如`RUST_BACKTRACE=1 cargo run`

* 为了获得带有调试信息的回溯, 必须要有debug符号; 当不使用`--release`标记运行`cargo build`或`cargo run`命令时, 默认使用debug符号

* 输出信息的说明: 对于某一行所提到的函数/代码, 它调用了上一行所提到的代码, 而且它被它下一行所提到的代码调用, 
  * 如, 第一行的代码是被第二行的代码调用, 第二行的代码是被第三行调用

# 2.可恢复错误和`Result`

`Result`枚举类, 在Rust的prelude中

* 定义

  ```rust
  enum Result<T, E> {
      Ok(T),
      Err(E),
  }
  ```

  * `T`类型, 是在`OK`中值的类型; `OK`变体在成功时返回
  * `E`类型, 是在`Err`中错误的类型; `Err`变体在出现错误时返回

例子

```rust
use std::fs::File;

fn main() {
    let f = File::open("hello.txt");
    
    let f = match f {
        Ok(file) => file,
        Err(error) => {
            panic!("Problem opening the file: {:?}", error)
        },
    };
}
```

* `File::open`, 用于打开一个文件, 返回一个`Result<std::fs::File, std::io::Error>`枚举类
  * 其中, `std::fs::File`类型表示一个文件句柄(**file handle**); `std::io::Error`类型表示IO错误
* 因此, 当`File::open`成功打开一个文件时, 返回一个含有文件句柄的`OK`实例; 当失败时,返回一个含有IO错误信息的`Err`实例
* 然后, 使用`match`表达式来处理`Result`两种变体的情况

## `match`表达式处理不同的错误

`File::open`返回的`Err`中包含的`io::Error`, 是一个定义在标准库的结构; 它有一个`kind`方法, 返回一个`io::ErrorKind`枚举类型

* `io::ErrorKind`的变体表示一系列不同的IO错误, 如`ErrorKind::NotFound`是文件未找到错误

`File::create`, 用于创建一个文件, 返回一个`Result<std::fs::File, std::io::Error>`

```rust
use std::fs::File;
use std::io::ErrorKind;

fn main() {
    let f = File::open("hello.txt");

    let f = match f {
        Ok(file) => file,
        Err(error) => match error.kind() {
            ErrorKind::NotFound => match File::create("hello.txt") {
                Ok(fc) => fc,
                Err(e) => panic!("Problem creating the file: {:?}", e),
            },
            other_error => panic!("Problem opening the file: {:?}", other_error),
        },
    };
}
```

## `unwrap`和`expect`

`unwrap`, 一个`Result`的辅助方法, 是对`match`表达式的简写

* 定义

  ```rust
  pub fn unwrap(self) -> T {
          match self {
              Ok(t) => t,
              Err(e) => unwrap_failed("called `Result::unwrap()` on an `Err` value", &e),
          }
  }
  ```

  * 当`Result`的值是`OK`时, 返回`OK`实例中的值
  * 当`Result`的值时`Err`时, 将会调用`unwrap_failed`, 而`unwrap_failed`会调用`painc!`宏

* 例子

  ```rust
  use std::fs::File;
  
  fn main() {
      let f = File::open("hello.txt").unwrap();
  }
  ```

`except`, 与`unwrap`类似, 不同的是让我们选择`panic!`宏的错误信息

* 定义

  ```rust
  pub fn expect(self, msg: &str) -> T {
          match self {
              Ok(t) => t,
              Err(e) => unwrap_failed(msg, &e),
          }
  }
  ```

* 例子

  ```rust
  use std::fs::File;
  
  fn main() {
      let f = File::open("hello.txt").expect("Failed to open hello.txt");
  }
  ```

## 传送错误Propagating Errors

当某个函数中出现错误, 将该错误返回给调用者, 让调用者来决定如何处理, 这被称为传送错误

* 这使得我们能够获得更多对该函数的控制, 因为调用者可能拥有更多信息或逻辑, 以决定如何处理错误

例子

```rust
use std::io;
use std::io::Read;
use std::fs::File;

fn read_username_from_file() -> Result<String, io::Error> {
    let f = File::open("hello.txt");

    let mut f = match f {
        Ok(file) => file,
        Err(e) => return Err(e),
    };

    let mut s = String::new();

    match f.read_to_string(&mut s) {
        Ok(_) => Ok(s),
        Err(e) => Err(e),
    }
}
```

### `?`操作符: 传送错误的简写

`?`操作符, 放在`Result`值后, 

* 当`Result`的值是`Ok`时, 将该`OK`实例作为表达式的结果, 然后继续执行当前函数
* 当`Result`的值是`Err`时, 将该`Err`实例作为函数的返回值, 然后函数返回一个`Err`值

`?`所使用的`Err`值被传递到`from`函数; `from`函数定义在标准库的`From`特征中, 用于转换错误类型

* 当`?`调用`from`函数后, `Err`中包含的错误类型被转换成当前函数签名中返回类型所定义的错误类型
* 前提: `Err`包含的错误类型实现了一个`from`函数, 而该`from`函数定义一个该错误类型到返回错误类型的转换

注意: 

*  `?`操作符对所在的函数有要求: 返回类型必须为`Result`, `Option`, 或实现`std::ops::try`的类型

例子:

```rust
use std::io;
use std::fs;
use std::io::Read;
use std::fs::File;

fn read_username_from_file() -> Result<String, io::Error> {
    let mut f = File::open("hello.txt")?;
    let mut s = String::new();
    f.read_to_string(&mut s)?;
    Ok(s)
}
//将其串联起来
fn read_username_from_file() -> Result<String, io::Error> {
    let mut s = String::new();

    File::open("hello.txt")?.read_to_string(&mut s)?;

    Ok(s)
}
//Rust提供了一个fs::read_to_string函数, 能够执行以上的操作
fn read_username_from_file() -> Result<String, io::Error> {
    fs::read_to_string("hello.txt")
}
```

# 3.错误处理的原则

错误处理

* 如果调用`panic!`宏, 则表示这是一个不可恢复的错误; 调用者没有选择的余地
* 如果选择返回`Result`值, 则让调用者选择如何处理

## 示例代码, 原型代码和测试代码适合`panic!`

* 当编写示例来展示一些概念时, 健壮的错误处理代码通常会比较繁琐, 导致示例不那么清晰明了

* 类似地, 编写代码原型时, 在决定如何处理错误之前, 使用`panic!`宏来处理错误, 以便快速实现原型
  * 在准备好让程序更好地处理错误时, 可以原来的`panic!`宏替换成所需的错误处理代码

* 在测试时, 如果某个方法调用在测试中失败了, 通常希望整个测试也被标记为失败

## 指导原则

* 当代码可能以有害状态结束时, 建议使用`panic!`; 
  * 有害状态, 是指代码中假设, 保证, 协议或不变性被破坏的状态;
    * 如: 无效的值, 自相矛盾的值, 不存在的值
  * 有害状态不包含预期会偶尔发生的错误
* 当预期的失败出现时, 通常适合返回`Result`

