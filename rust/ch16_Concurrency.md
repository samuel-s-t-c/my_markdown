# 总结

* Rust的拥有权和类型检查, 使得编译器能够检查很多种类的并发/并行错误

* 如何创建线程来同时运行多个代码段
* 消息传递(*Message-passing*)并发
* 共享状态(*Shared-state*)并发
* `Sync`和`Send`特征

[toc]

# 1.使用线程同时运行代码

在绝大多数现代操作系统中

* 一个执行程序的代码运行于一个进程(*process*)中; 通常操作系统可以管理多个进程.

* 在程序内部, 可以拥有多个同时运行的独立部分; 能够同时运行独立部分的功能, 被称为线程(*thread*)
  * 将程序中的计算拆分为多个线程, 虽然增加复杂性, 但能够提高性能, 这是因为程序可以同时运行多个任务

由于线程的种种影响, 编写多线程程序需要格外小心, 且与单线程程序的代码结构不一样; 线程是同时运行且相互独立的, 其本质并不能确保不同线程中代码的执行顺序; 因此导致以下问题

* 竞争条件(*race condition*): 线程以未定义的顺序访问数据或资源
* 死锁(*deadlock*): 两个线程相互等待对方停止使用其所拥有的资源
* 只会在特定情况下出现的bug, 通常难以重现和修复

编程语言实现线程的模型

* 一对一模型: 有些语言调用操作系统提供的API以创建线程, 此时一个语言线程对应一个系统线程
* 绿色线程模型/M:N模型: 有些语言提供了自己特殊的线程实现, 提供一些特性, 此时M个语言线程对应N个系统线程
  * M和N通常不是相同的数量

运行环境(*runtime*), 是指一些在每个二进制文件中都包含的代码, 是由语言自身提供的, 通常用于设置运行环境

* 运行环境越小, 其提供的特性越少; 但输出的二进制文件也越少, 而这易于与其他语言相结合
* 绿色线程模型需要较大的运行库, 而一对一模型所需的运行库很小
* 因此, Rust语言只提供一对一模型的实现, 以保证其运行库很小; 这使得Rust能够调用C语言, 以获得高性能
  * 有实现绿色线程模型的crate

## 1.1`spawn`: 创建一个新线程

`spawn`函数, 路径为`std::thread::spawn`

* 接受一个闭包, 其代码将在新线程中运行; 返回一个`JoinHandle`
* 当主线程结束时, 其他线程会停止, 无论是否完成运行

`JoinHandle`类型, 路径为`std::thread::JoinHandle`

* 表示进入某个线程的权限;
* `JoinHandle`类型的值是一个有拥有权的值

* 方法
  * `pub fn join(self) -> Result<T>`
    * 阻止当前线程的运行, 直到线程柄所表示的线程终止;
    * 返回一个`Result`; 当子线程panic时, `Result`是一个`Err`, 包含一个传递给`panic`的参数

`sleep`函数, 路径为`std::thread::sleep`

* 使得某个线程停止执行一小段时间; 允许在不同的线程中运行

## 1.2使用线程

`move`闭包经常与`thread::spawn`一起使用

* 因为它允许在子线程中使用来自父线程的数据
* `move`闭包是指带有`move`关键字的闭包, 见ch13

* 例子

  ```rust
  use std::thread;
  
  fn main() {
      let v = vec![1, 2, 3];
  
      let handle = thread::spawn(move || {
          println!("Here's a vector: {:?}", v);
      });
  
      handle.join().unwrap();
  }
  ```

  * 将`v`的拥有权移到新线程中

# 2.消息传递并发

消息传递, 是一种受欢迎的确保安全并发的方式

* 线程之间通过传递包含数据的消息来相互交流

 通道是一个编程概念; Rust标准库提供了一个通道的实现

* 通道由两部分组成: 发送者和接收者
  * 发送者位于通道的上游, 接受者位于通道的下游
  * 在Rust中一个通道可以有多个发送端, 但只有一个接收端
* 代码的某个部分通过发送者传递消息, 另一个部分检查接收者是否有消息
* 如果发送者或接收者被丢弃, 则该通道被关闭了

例子

```rust
use std::sync::mpsc;
use std::thread;

fn main() {
    let (tx, rx) = mpsc::channel();

    thread::spawn(move || {
        let val = String::from("hi");
        tx.send(val).unwrap();
    });

    let received = rx.recv().unwrap();
    println!("Got: {}", received);
}
```

* `channel`函数, 路径为`std::sync::mpsc::channel`
  * `mpsc`是*multiple producer, single consumer*的缩写
  * 声明: `pub fn channel<T>() -> (Sender<T>, Receiver<T>)`
    * 返回一个元组, 第一个元素是发送端, 第二个元素是接受端
* `Sender`
  * 特征
    * `Clone`
  * 方法: 
    *  `pub fn send(&self, t: T) -> Result<(), SendError<T>>`
      * 向通道发送一个值; 如果通道的接受端被关闭, 则返回一个错误
* `Receiver`
  * 方法
    * `pub fn recv(&self) -> Result<T, RecvError>`:
      * 阻止所在线程的执行, 并等待接受一个值; 如果通道的发送端都关闭, 则返回一个错误
    * `pub fn try_recv(&self) -> Result<T, TryRecvError>`
      * 不阻止执行; 立即返回一个`Result`, `Ok`表示有可用的消息, `Err`表示没有任何消息

# 3.共享状态并发

互斥体(*Mutual exclusion, Mutex*), 是在共享内存中一种常见的并发基础机制

## 3.1互斥体

互斥体: 在任何时刻只允许一个线程访问其中的某些数据

* 如果某个线程想要访问互斥体中的数据, 它必须向互斥体请求一个锁(*lock*), 以表明希望访问数据
  * 锁(*lock*)是互斥体中的一个数据结构, 用于记录当前占有数据的线程
* 如果某个线程处理完互斥体中的数据后, 它必须为数据解锁,  这样其他线程才能获得这个锁

## 3.2`Mutex<T>`

Rust提供一个互斥体的实现

 `Mutex<T>`, 互斥体

* 路径: `std::sync::Mutex`

* 方法:
  * `pub fn new(t: T) -> Mutex<T>`
    * 接受一个值, 返回一个`Mutex`
  * `type LockResult<Guard> = Result<Guard, PoisonError<Guard>>`
  * `pub fn lock(&self) -> LockResult<MutexGuard<T>>`
    * 请求获得一个锁, 这会阻止当前线程的执行, 直至获得返回值
    * 如果持有锁的线程panic, 返回`Err`

`MutexGuard<T>`, 指向互斥体中的数据的智能指针

* 路径: `std::sync::MutexGuard`
* 只有当前线程持有锁时, 才能通过`MutexGuard`访问数据
* 当`MutexGuard`离开作用域时, 自动解锁

## 3.3多线程共享一个`Mutex<T>`

注意: `Rc<T>`不能在线程间安全地共享某个值

`Arc<T>`类型, 原子性引用

* `a`是`atomic`的缩写, `rc`是`reference count`的缩写
* 路径: `std::sync::Arc`
* 与`Rc<T>`类似, 不过能安全地在并发时使用
* 拥有与`Rc<T>`相同的API

## 3.4`RefCell`/`Rc` 与 `Mutex`/`Arc` 的相似之处

像`Cell`系列类型那样, `Mutex`是内部可变的;  与`Rc`类似, `Arc`只提供不可变引用

* 所以, 可以使用`Mutex`来改变`Arc`中的内容

`Rc`可能出现引用循环的风险; 而`Mutex`也有造成死锁(*deadlock*)的风险

* 当一个操作需要锁住两个资源, 而两个线程各自持有对方想要的锁, 这会导致它们永远相互等待

# 4.`Sync`和`Send`特征

Rust语言自身只提供很少的并发特性

* 前面所提到的并发特性, 都是标准库所提供的.

* 你可以编写自己的并发特性, 或使用别人编写的特性

Rust语言提供的并发特性中的特征: `std::marker`中的`Sync`和`Send`特征

## 4.1`Send`: 允许线程间的拥有权转移

`Send`标记特征

* 程序员想要表明某个类型的拥有权可以在线程间移动, 那么为该类型实现`Send`特征, 
* 几乎所有Rust类型都有`Send`特征, 除了几个例外, 如`Rc<T>`

  * 完全由`Send`类型组成的类型, 会自动标记为`Send`
  * 除了原始指针(*raw pointer*)外, 所有原始类型都是`Send`的

## 4.2`Sync`: 允许多线程访问

`Sync`标记特征

* 程序员想要表明某个类型可以安全地被多个线程的代码引用, 那么为其实现`Sync`特征
  * 也就是说, 如果类型`T`的引用`&T`是`Send`, 那么类型`T`是`Sync`
* 原始类型都是`Sync`
* 完全由`Sync`类型组成的类型, 自动被标记为`Sync`

## 4.3手动实现`Send`和`Sync`是不安全的

* 作为标记特征(*maker trait*), `Send`和`Sync`没有需要实现的方法, 只是用来标记并发相关的不变量

* 通常并不需要手动实现`Send`和`Sync`特征, 因为由`Send`和`Sync`类型组成的类型自就是`Send`和 `Sync`的

* 如果手动实现`Send`和`Sync`特征, 那么一定会涉及到`unsafe`代码