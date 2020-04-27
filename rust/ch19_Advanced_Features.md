# 总结

* unsafe rust
  * 移除Rust的某些保证
* 高级特征
  * 关联类型, 默认类型形参, 完全限定语法, 超特征(*supertrait*), 特征的*newtype pattern*
* 高级类型
  * *newtype pattern*, 类型别名, `never`类型, 动态大小类型
* 高级函数和闭包
  * 函数指针, 返回闭包
* 宏
  * 一种定义代码的方式, 此代码在编译时定义更多代码的

[toc]



# 1.不安全Rust

*unsafe rust*是指不会强制执行内存安全保证的rust, 属于Rust中的另一种语言

* *unsafe rust*存在的原因
  * 由于静态分析是保守的, 一些有效的代码会被拒绝; 而*unsafe rust*可以使得某些不安全的代码通过编译, 代价是需要自己承担风险
  * 底层计算机硬件是天然不安全的; 如果Rust不允许你进行不安全操作, 则有些任务就无法通过Rust来完成. 而Rust的语言目标之一是能够进行底层系统编程

## *unsafe superpower*

在*unsafe rust*中, 你可以使用五种在*safe rust*不能使用的操作, 这些操作被称为*unsafe superpower*

* 解引用原始指针(*raw pointer*)
* 调用不安全的函数或方法
* 访问或修改一个可变的静态变量
* 实现一个不安全的特征
* 访问`union`的字段

为了切换到*unsafe rust*, 使用`unsafe`关键字后跟一个代码块; 在块里面就可以使用*unsafe*代码.

* 说明
  * `unsafe`并不会关闭借用检查器或取消其他的Rust安全检查
    * 如果在`unsafe`代码中使用引用, 它依然被检查
  * `unsafe`只是提供了上面五种特性, 让这五种特性不会被编译器因内存安全而拒绝编译

* 意义

  * `unsafe`关键字只是表示你自己确保其中编写的代码会以有效的方式访问内存

  * 将可能出现内存问题的代码都放在`unsafe`块中; 如果出现内存错误时, 可以肯定错误位于`unsafe`块中

* 惯用做法: 
  * 通常将`unsafe`代码封装在一个安全的抽象中, 并提供一组安全的API; 这样的话, 可以防止内存到处泄漏

## 1.1解引用原始指针

两种原始指针类型: `*const T`, `*mut T`, 分别对应于不可变的原始指针和可变的原始指针

* 其中的`*`星号是类型的一部分
* 原始指针的性质
  * 无视借用规则, 即可以有多个不可变或可变的指针指向同一个位置
  * 可能指向无效的内存
  * 允许为空, `null`
  * 没有任何自动释放的功能
* 使用原始指针的原因
  * 与C代码交互
  * 需要构建一个借用检查器无法理解的安全抽象

 可以在*safe rust*中创建原始指针, 但只能在`unsafe`块中解引用

* 在`safe rust`中创建原始指针

  ```rust
  let mut num = 5;
  let r1 = &num as *const i32;
  let r2 = &mut num as *mut i32;
  ```

  * 这里使用`as`将引用转换成对应的原始指针类型

  ```rust
  let address = 0x012345usize;
  let r = address as *const i32;
  ```

  * 这里是创建一个指向某个位置的原始指针; 这只是作为一种展示, 通常不会这么做

* 只能在`unsafe`块中解引用原始指针

  ```rust
  unsafe {
  	println!("r1 is: {}", *r1);
  	println!("r2 is: {}", *r2);
  }
  ```

## 1.2调用不安全的函数或方法

不安全函数或方法

* 不安全函数或方法有一个`unsafe`关键字在定义的前面

* 只能在`unsafe`块中调用不安全函数或方法
  * 不安全函数的函数体也是`unsafe`块

注意: 

* 包含*unsafe*代码的函数可以是安全函数, 也可以是不安全函数

例子:

```rust
unsafe fn dangerous() {}

unsafe {
	dangerous();
}
```

### 有不安全代码的安全函数

```rust
use std::slice;

fn split_at_mut(slice: &mut [i32], mid: usize) -> (&mut [i32], &mut [i32]) {
    let len = slice.len();
    let ptr = slice.as_mut_ptr();

    assert!(mid <= len);

    unsafe {
        (
            slice::from_raw_parts_mut(ptr, mid),
            slice::from_raw_parts_mut(ptr.add(mid), len - mid),
        )
    }
}

fn main() {
    let mut vector = vec![1, 2, 3, 4, 5, 6];
    let (left, right) = split_at_mut(&mut vector, 3);
}
```

### `extern`函数

Rust有一个`extern`关键字, 有助于创建和使用外部函数接口(*Foreign Function Interface(FFI)*)

* *FFI*是一种编程语言定义函数的方式, 以允许不同的编程语言调用该类函数

用`extern`块来使用外部函数接口

* 形式: `extern "ABI" {/*block*/}`

* 在`extern`块中声明外部函数的名字和签名

* `extern`块内声明的函数对Rust来说总是不安全, 因为Rust无法检查这些函数;
  * 因此调用外部代码时需要`unsafe`块

* 用例

  ```rust
  extern "C" {
      fn abs(input: i32) -> i32;
  }
  
  fn main() {
      unsafe {
          println!("Absolute value of -3 according to C: {}", abs(-3));
      }
  }
  ```
  * `extern "C"`块中的`"C"`定义了外部函数所使用的ABI(*application binary interface*)
  * `"C"`ABI遵循C语言的ABI

用`extern`关键字来创建Rust的外部函数

* 形式: `extern "ABI" fn ...`, `...`是函数的定义

* 要求: 需要添加`#[no_mangle]`注解, 使得该函数的名字不被*mangle*

* 这种`extern`函数不需要`unsafe`块

* 用例

  ```rust
  fn main() {
  	#[no_mangle]
  	pub extern "C" fn call_from_c() {
  	    println!("Just called a Rust function from C!");
  	}
      call_from_c();
  }
  ```

## 1.3访问或修改可变的静态变量

Rust中的全局变量, 被称为`static`变量

* 命名惯例是*SCREAMING_SNAKE_CASE*.
* 必须有类型注解
* 静态变量只能存储`'static`寿命的引用; 因此不必显式标注寿命

* ```rust
  static HELLO_WORLD: &str = "Hello, world!";
  
  fn main() {
      println!("name is: {}", HELLO_WORLD);
  }
  ```

访问一个不可变的静态变量是安全的; 但访问/修改一个可变的静态变量是不安全的

* 原因之一: 如果两个线程都访问/修改同一个可变静态变量, 那么可能出现数据竞争(*data race*)

* 因此, 可变静态变量只能在`unsafe`块中使用

* ```rust
  static mut COUNTER: u32 = 0;
  
  fn add_to_count(inc: u32) {
      unsafe {
          COUNTER += inc;
      }
  }
  
  fn main() {
      add_to_count(3);
  
      unsafe {
          println!("COUNTER: {}", COUNTER);
      }
  }
  ```

## 1.4实现不安全特征

不安全特征

* 某个特征中的一个方法有编译器无法验证的*invariant*, 该特征就是不安全特征

* 形式: 
  * 声明不安全特征时在`trait`前添加`unsafe`关键字
  * 实现不安全特征时在`impl`前添加`unsafe`

## 1.5访问`union`的字段

[Unions](https://doc.rust-lang.org/reference/items/unions.html)

`union`类型, 与`struct`类似

* `union`的实例每次只能使用一个字段
* 主要用途: 与C代码中的*union*交互

访问`union`字段是不安全的

* 因为Rust无法保证当前存放在`union`实例中的数据类型

# 2.高级特征

## 2.1关联类型

一个特征的关联类型, 是该特征的类型占位符

* 在定义特征时,  可以在方法签名中使用这些占位符
* 在实现特征时, 需要为这些占位符指定具体的类型

例子: 标准库中的`Iterator`特征有一个关联类型`Item`, 代表一种类型, 该类型的值被实现`Iterator`的类型的值遍历

```rust
pub trait Iterator {
    type Item;

    fn next(&mut self) -> Option<Self::Item>;
}
```

为一个类型实现特征时

```rust
impl Iterator for Counter {
    type Item = u32;

    fn next(&mut self) -> Option<Self::Item> {
        // --snip--
```

## 2.2泛型类型形参的默认实参与运算符重载

可以为泛型类型形参指定一个默认的具体类型, 也就是默认实参

* 形式: 在声明泛型类型时, `<PlaceholderType=ConcreteType>`

运算符重载

* 重载运算符, 其实是实现`std::ops`中的特征

  * 这些特征的泛型类型形参都有默认实参

* 例子: 

  * 重载`+`运算符, 对应的特征是`std::ops::Add`

  ```rust
  use std::ops::Add;
  
  #[derive(Debug, PartialEq)]
  struct Point {
      x: i32,
      y: i32,
  }
  
  impl Add for Point {
      type Output = Point;
  
      fn add(self, other: Point) -> Point {
          Point {
              x: self.x + other.x,
              y: self.y + other.y,
          }
      }
  }
  
  fn main() {
      assert_eq!(
          Point { x: 1, y: 0 } + Point { x: 2, y: 3 },
          Point { x: 3, y: 3 }
      );
  }
  ```

  * `Add`特征的定义

  ```rust
  trait Add<RHS=Self> {
      type Output;
  
      fn add(self, rhs: RHS) -> Self::Output;
  }
  ```

## 2.3完全限定语法

> Rust既不能避免一个特征与另一特征拥有同名的方法, 也不能阻止为同一类型实现这两个特征; 甚至可以在类型上直接实现一个方法, 该方法与特征的方法同名.

如果某个类型中普通方法与特征方法同名, 且直接调用方法, 则编译器默认调用普通方法

* 这里的普通方法, 是指直接定义在类型上的不属于任何特征的方法

此时, 如果想要调用特征的同名方法, 可以在方法名前指定特征名, 形如`TraitName::MethodName`

* 方法的第一个形参是`self`; Rust可以根据`self`的类型来确定特征的具体实现

* 类型名也可以使用这种语法, 形如`TypeName::MethodName`, 相当于`var.MethodName`

* 例子

  ```rust
  trait Pilot {
      fn fly(&self);
  }
  
  trait Wizard {
      fn fly(&self);
  }
  
  struct Human;
  
  impl Pilot for Human {
      fn fly(&self) {
          println!("This is your captain speaking.");
      }
  }
  
  impl Wizard for Human {
      fn fly(&self) {
          println!("Up!");
      }
  }
  
  impl Human {
      fn fly(&self) {
          println!("*waving arms furiously*");
      }
  }
  
  fn main() {
      let person = Human;
      Pilot::fly(&person);
      Wizard::fly(&person);
      person.fly();
      Human::fly(&person); //等价于person.fly()
  }
  ```

如果特征中的关联函数与类型直接定义的关联函数同名, 

* 直接调用: 编译器默认调用直接定义在类型上的关联函数
* 此时, 如果想调用特征的关联函数, 必须使用完全限定语法

完全限定语法

* 形式: `<TYPE as TRAIT>::FUNCTION`

  * 表示在`TYPE`类型上实现的`TRAIT`特征中的`FUNCTION`函数/方法

* 例子

  ```rust
  trait Animal {
      fn baby_name() -> String;
  }
  
  struct Dog;
  
  impl Dog {
      fn baby_name() -> String {
          String::from("Spot")
      }
  }
  
  impl Animal for Dog {
      fn baby_name() -> String {
          String::from("puppy")
      }
  }
  
  fn main() {
      println!("A baby dog is called a {}", <Dog as Animal>::baby_name());
  }
  
  ```

## 2.4超特征Supertrait

某个特征`A`可以使用另一个特征`B`的功能

* 此时, 特征`B`是特征`A`的超特征

指定超特征的形式: `trait A : B {/*bock*/}`

* 表示特征`A`需要特征`B`的功能; 实现特征`A`的类型也必须实现特征`B`

* 例子

  ```rust
  use std::fmt;
  
  trait OutlinePrint: fmt::Display {
      fn outline_print(&self) {
          let output = self.to_string();
          let len = output.len();
          println!("{}", "*".repeat(len + 4));
          println!("*{}*", " ".repeat(len + 2));
          println!("* {} *", output);
          println!("*{}*", " ".repeat(len + 2));
          println!("{}", "*".repeat(len + 4));
      }
  }
  ```

  * 例子中的`to_string`方法是`Display`特征提供的

## 2.5*newtype pattern*

> 孤儿原则: 在一个类型上实现一个特征的前提是, 特征或类型在当前crate定义的

使用*newtype pattern*可以绕开孤儿原则

* 使用元组结构作为一个简单的包装器: 元组结构的唯一字段的类型是希望实现特征的类型
* 然后, 这个元组结构对于当前crate是局部的, 因此可以为该元组结构实现特征, 间接地让内部类型使用特征的功能

*newtype pattern*其实是创建一个新类型, 并为该类型实现特征

* 如果想让新类型拥有内部类型的所有行为, 可以考虑为新类型实现一个`Dref`特征

* *newtype pattern*没有运行时惩罚, 并且包装器在编译器时会被省略

例子: 为持有`Vec<T>`的元组结构实现`Display`特征

```rust
use std::fmt;

struct Wrapper(Vec<String>);

impl fmt::Display for Wrapper {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        write!(f, "[{}]", self.0.join(", "))
    }
}

fn main() {
    let w = Wrapper(vec![String::from("hello"), String::from("world")]);
    println!("w = {}", w);
}
```

# 3.高级类型

# 4.高级函数和闭包

# 5.宏