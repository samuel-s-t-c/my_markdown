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

## 3.1*newtype pattern*: 类型安全和抽象

*newtype pattern*在很多任务上都很有用

* 静态地确保某些值不被混淆
* 抽象化一个类型的部分实现
* 轻量级封装

## 3.2类型别名

类型别名

* 形式: `type ALIAS = TYPE;`

* 与*newtype pattern*的区别: *newtype pattern*创建一个新类型, 而类型别名只是某个类型的另一个名字

例子

```rust
type Thunk = Box<dyn Fn() + Send + 'static>;
```

## 3.3*never*类型

*never*类型`!`

* 一种特殊的类型, 是类型理论术语中的*empty type*
* `!`类型没有值
* 用作函数的返回类型时, 表示一个函数永远不会返回
* `!`类型的表达式可以强制转换成任何其他类型
  * 属于`!`类型的表达式: `continue`, 永不结束的`loop`循环

## 3.4`Sized`特征与动态大小类型

动态大小类型(*dynamically sized type, DST*), 也叫做不定大小类型(*unsized type*)

* 编译时不知道其大小, 直到运行时才得知

* 如, `str`是DST; 注意是`str`, 不是`&str`

对于DST, 只能在运行时才能知道类型的大小

* 因此, 不能创建非引用的DST变量, 函数不能使用DST本身作为参数
* 可以创建DST引用的变量, 或者将DST放在其他指针类型中
  * 与普通引用对比, DST引用, 除了存储了内存位置, 还有一个额外的元数据来存储尺寸

* 其实, 每个特征(*trait*)都是一个动态大小类型
  * 所以在使用特征对象时, 要将特征放在某个指针类型中

`Sized`特征

* 确定一个类型的大小是否在编译时可知

  * 当某个类型的大小在编译时可知, 则该类型自动实现`Sizd`特征

* 在泛型函数中, Rust隐式地为每个类型形参添加一个`Sized`特征边界

  * 默认情况下, 为每个类型形参添加一个`Sized`, 表示只接受编译时大小已知的类型

  * 可以显式地为类型形参添加`?Sized`边界, 表示类型既可以是`Sized`, 也可以不是`Sized`

    * 由于类型可能不是`Sized`的, 所以需要将其放入某种指针类型中

  * 例子

    ```rust
    fn generic<T>(t: T) {
        // --snip--
    }
    
    fn generic<T: Sized>(t: T) {
        // --snip--
    }
    
    fn generic<T: ?Sized>(t: &T) {
        // --snip--
    }
    ```

# 4.高级函数和闭包

## 4.1函数指针

函数指针

* 作用: 允许将某个函数作为其他函数的实参

* `fn`类型就是函数指针

* 形式: `fn(PARAMETER) -> RETURN`

  * `PARAMETER`表示函数签名中的形参类型, `RETURN`表示返回类型

* 函数指针是一个类型, 它实现了所有闭包特征(`Fn`, `FnMut`, `FnOnce`)

* 例子

  ```rust
  fn add_one(x: i32) -> i32 {
      x + 1
  }
  
  fn do_twice(f: fn(i32) -> i32, arg: i32) -> i32 {
      f(arg) + f(arg)
  }
  
  fn main() {
      let answer = do_twice(add_one, 5);
  
      println!("The answer is: {}", answer);
  }
  ```

## 4.2返回闭包

作为函数形参时, 闭包是由闭包特征表示的,  这意味着编译器不知道它的大小, 不能直接返回闭包

* 所以需要将其放在某个指针中

```rust
fn returns_closure() -> Box<dyn Fn(i32) -> i32> {
    Box::new(|x| x + 1)
}
```

# 5.宏

Rust的宏(*macro*)是指以下的宏

* 声明式宏(*declarative macro*), 使用`macro_rules!`
* 三种过程式宏(*procedural macro*)
  * 自定义`#[derive]`宏
  * 类属性宏(*Attribute-like macro*)
  * 类函数宏(*Function-like macro*)

## 5.1宏和函数的区别

宏, 本质上是一种编写代码的方式, 所编写的代码将会生成其他代码, 这也就是元编程(*metaprogramming*)

* 如`derive`属性, 它会为自定义的结构和枚举生成各种特征的实现代码
* 如`prinlnt!`和`vec!`宏, 会展开成更多的代码

宏与函数的不同

* 一个函数签名必须声明形参的数量和类型; 一个宏接受参数的数量是可变的
* 在编译器翻译代码前, 宏会进行代码展开; 而函数不会
* 宏的定义比函数定义更加复杂, 更难以阅读, 理解和维护
* 在某个文件中调用宏之前, 必须有该宏的定义, 或者将其引入作用域; 而函数可以在任意地方定义和调用

## 5.2声明式宏与`marco_rules!`

声明式宏(*declarative macro*), 也被称为*macros by example*, *`macro_rules!` macros*, 或直接称为*macro*

* 声明式宏是Rust中宏最常用的形式
* 声明式宏允许我们编写类似`match`表达式的宏: 比较一个值和*pattern*
  * 这里的值是字面上的Rust源代码
  * *pattern*对源代码的结构进行比较
  * 如果源代码与*pattern*匹配, 则将这些源代码替换成该*pattern*所关联的代码
  * 这些动作发生在编译过程

可以使用`macro_rules!`来定义一个声明式宏

* 以`macro_rules!`作为宏定义的开始, 后跟该宏的名字(没有感叹号), 接着是一对花括号, 里面是宏定义体
* 在这样的宏定义体中是形如`PATTERN => BLOCK`的分支
  
* 宏*pattern*语法见[*the reference*](https://doc.rust-lang.org/reference/macros-by-example.html)
  
* 例子: `vec!`的简化定义

  ```rust
  
  #![allow(unused_variables)]
  fn main() {
  	#[macro_export]
  	macro_rules! vec {
  	    ( $( $x:expr ),* ) => {
   	       {
   	           let mut temp_vec = Vec::new();
   	           $(
   	               temp_vec.push($x);
   	           )*
  	            temp_vec
  	        }
   	   };
  	}
      
      let v: Vec<u32> = vec![1, 2, 3];
  }
  ```
* `#[macro_export]`注解	

  * 作用: 当该宏定义所在的crate被引入作用域时, 该宏也被引入, 应该是可用的;
  * 如果没有该注解, 宏将不能被引入作用域

## 5.3过程式宏

宏的另一种形式是, 过程式宏(*procedural macro*)

* 这类宏的行为与函数类似: 过程式宏接受一些代码作为输入, 对输入进行操作, 然后产生一些代码作为输出

* 当创建过程式宏时, 其定义必须位于它们自己的crate中; 这种crate是特殊的crate类型

  * 这么做是出于某些复杂技术原因; 将来希望能够消除这些限制

* 定义过程式宏的形式如下

  ```rust
  use proc_macro;
  
  #[some_attribute]
  pub fn some_name(input: TokenStream) -> TokenStream {
  }
  ```

  * `some_attribute`属性表示正在创建的是哪种过程式宏
  * `TokenStream`类型是定义在Rust的`proc_macro` crate中, 表示Rust代码

### 自定义`derive`宏

创建一个`hello_crate`, 里面有一个特征`HelloMacro`和特征的关联函数`hello_macro`; 

* 为`HelloMarco`提供一个`derive`宏; 这样的话, 只要用户为其类型标注`#[derive(HelloMacro)]`, 类型将得到`HelloMacro`特征的默认实现

过程

1. 创建一个library crate, 名为`hello_macro`

   ```shell
   $ cargo new hello_macro --lib
   ```

1. 在文件*src/lib.rs* 中, 定义特征`HelloMacro`

   ```rust
   pub trait HelloMacro {
       fn hello_macro();
   }
   ```

3. 定义`derive`宏

   1. (2020/4/28)由于技术原因, 需要为过程式宏创建一个crate

      * 规范: 

        * 名字为`foo`的crate定义过程式宏, 则该过程式宏的crate名字是`foo_derive`

      * 在`hello_macro`项目中创建新crate, 名为`hello_macro_derive`

        ```shell
        $ cargo new hello_macro_derive --lib
        ```

      * 要使用过程式宏, 需要同时有`hello_crate`和`hello_crate_derive`这两个crate, 并要将其引入作用域

      * 如果改变了`hello_crate`中的特征定义, 也需要改变`hello_crate_derive`中的过程式宏实现

   2. 将`hello_macro_derive` crate声明为过程式宏crate, 并添加依赖`syn`和`quote` crate

      文件*hello_macro_derive/Cargo.toml*中

      ```toml
      [lib]
      proc-macro = true
      
      [dependencies]
      syn = "1.0"
      quote = "1.0"
      ```

   3. 在文件*hello_macro_derive/src/lib.rs*中添加过程式宏的实现

      ```rust
      extern crate proc_macro;
      
      use proc_macro::TokenStream;
      use quote::quote;
      use syn;
      
      #[proc_macro_derive(HelloMacro)]
      pub fn hello_macro_derive(input: TokenStream) -> TokenStream {
          // Construct a representation of Rust code as a syntax tree
          // that we can manipulate
          let ast = syn::parse(input).unwrap();
      
          // Build the trait implementation
          impl_hello_macro(&ast)
      }
      ```

      * `#[proc_macro_derive(HelloMacro)]`, 表示下面的函数是`HelloMacro`特征的`derive`过程式宏
        * 当用户在某个类型上添加`#[derive(HelloMacro)]`时, 将会调用`hello_macro_derive`函数
      * `proc_macro` crate是Rust的编译器API, 以允许我们读取和操纵Rust代码
        * `proc_macro` crate是Rust语言自带的, 因此不需要在*Cargo.toml*中声明; 
      * `syn` crate可以将Rust代码从`TokenStream`字符串中解析成`DeriveInput`数据结构, 以便进行其他操作
      * `quote` crate可以将`syn`数据结构转换成Rust代码

      ```rust
      fn impl_hello_macro(ast: &syn::DeriveInput) -> TokenStream {
          let name = &ast.ident;
          let gen = quote! {
              impl HelloMacro for #name {
                  fn hello_macro() {
                      println!("Hello, Macro! My name is {}!", stringify!(#name));
                  }
              }
          };
          gen.into()
      }
      ```

      * `DeriveInput`中的`ident`字段是一个包含标识符的结构, 可以得到表示类型名的标识符
      * `quote!`, 见[[the `quote` crate’s docs](https://docs.rs/quote)
      * `stringify!`宏是Rust内置宏, 接受一个Rust表达式, 在编译时将表达式转换成字符串字面量
        * 如表达式`1 + 2`, 变成字符串`"1 + 2"`

4. 此时可以构建这两个crate

5. 在其他的crate中使用`derive`宏

   1. 在*Cargo.toml*文件中添加对`hello_macro`和`hello_macro_derive`的依赖

   2. 在文件*src/main.rs*中

      ```rust
      use hello_macro::HelloMacro;
      use hello_macro_derive::HelloMacro;
      
      #[derive(HelloMacro)]
      struct Pancakes;
      
      fn main() {
          Pancakes::hello_macro();
      }
      ```

### 类似属性的宏

类属性的宏, 与自定义`derive`宏类似; 不同于为`derive`属性生成代码, 类属性的宏允许你创建新的属性

* 类属性的宏也更为灵活, 因为属性不仅可以用于结构和枚举, 还能用于其他item, 如函数

例子: 有一个`route`属性被框架定义为过程式宏

* 使用`route`属性注解一个函数

  ```rust
  #[route(GET, "/")]
  fn index() {
  ```

* 它的宏定义签名应该如下

  ```rust
  #[proc_macro_attribute]
  pub fn route(attr: TokenStream, item: TokenStream) -> TokenStream {
  ```

  * 第一个`TokenStream`形参, 表示属性的内容, 也就是`GET, "/"`部分
  * 第二个`TokenStream`形参, 表示属性所标记的item, 也就是`index`函数及其函数体

* 除此之外, 类属性宏与自定义`derive`宏的工作方式一致: 创建 `proc-macro` crate 类型的 crate 并实现希望生成代码的函数

### 类似函数的宏

例子: `sql!`宏

* 调用`sql!`宏

  ```rust
  let sql = sql!(SELECT * FROM posts WHERE id=1);
  ```

* `sql!`宏的签名

  ```rust
  #[proc_macro]
  pub fn sql(input: TokenStream) -> TokenStream {
  ```

* 除此之外, 类函数宏与自定义`derive`宏的工作方式一致