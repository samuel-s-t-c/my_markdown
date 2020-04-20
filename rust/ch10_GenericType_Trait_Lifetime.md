# 总结

* 泛型(**Generic**): 泛型类(**generic type**), 泛型函数(**generic function**)
* 特征(**trait**): 定义泛型行为

* 寿命(**lifetime**): 一种泛型, 向编译器提供这些信息: 引用如何相互关联

[toc]

# 1.泛型数据类型

## 1.1泛型函数

声明类型形参

* 在名字后跟`<>`; 在`<>`中声明一个或多个类型形参
* 规范: 名字尽量短和骆驼命名法, 通常使用`T`
* 声明过后, 类型形参可以像类型一样使用

```rust
fn largest<T>(list: &[T]) -> T {/*body*/}
```

## 1.2泛型结构

与在函数中声明类型形参类似, 在结构名字后的`<>`中声明类型形参

```rust
struct Point<T> {
    x: T,
    y: T,
}
```

* 例子中将`Point`中的`x`和`y`都声明为同一类型`T`

## 1.3泛型枚举

例子

```rust
enum Result<T, E> {
    Ok(T),
    Err(E),
}
```

## 1.4方法与泛型

为泛型结构定义方法

* 在`impl`后跟`<>`; 在`<>`里面声明类型形参, 表示为泛型结构定义方法

```rust
struct Point<T> {
    x: T,
    y: T,
}

impl<T> Point<T> {
    fn x(&self) -> &T {
        &self.x
    }
}
```

* 可以为特定类型实参的泛型结构定义方法

```rust
impl Point<f32> {
    fn distance_from_origin(&self) -> f32 {
        (self.x.powi(2) + self.y.powi(2)).sqrt()
    }
}
```

定义泛型方法

```rust
struct Point<T, U> {
    x: T,
    y: U,
}

impl<T, U> Point<T, U> {
    fn mixup<V, W>(self, other: Point<V, W>) -> Point<T, W> {
        Point {
            x: self.x,
            y: other.y,
        }
    }
}
```

## 1.5使用泛型的代码性能

Rust在编译时对使用泛型的代码完成单态化(**monomorphization **)

* 单态化是指将泛型代码转化成使用特定类型的代码的过程
* 在单态化过程中, 编译器会寻找所有使用泛型的地方, 然后生成使用具体类型的代码

* 因此, 在运行时使用泛型的代码的性能与其他代码一样

# 2.特征: 定义可共享的行为

特征, 定义了泛型行为, 即抽象地定义行为

* 特征可以与泛型关联, 使得泛型必须是有特定行为的类型

## 2.1定义特征

> 类型的行为是由其方法组成的

特征能够将多个方法签名组合起来, 以定义一组实现某些目的所需的行为

定义特征

* 形式: `trait NAME { /*method signatures*/}`
* 特征默认是私有的, 可以使用`pub`将其指定为公有的

可以为其中的部分或全部方法提供默认实现

* 当特征中的方法没有默认实现时, 实现该特征的类型必须提供该方法的定义
* 当特征中的方法有默认实现时, 实现该特征的类型可以选择保留或覆盖默认实现
* 在某个方法的默认实现中, 可以调用同一特征的其他方法, 即使这些方法没有默认实现

例子

```rust
pub trait Summary {
    fn summarize_author(&self) -> String;

    fn summarize(&self) -> String {
        format!("(Read more from {}...)", self.summarize_author())
    }
}
```

## 2.2在某个类型上实现特征

假设有特征`TRAIT`, 在结构`STRUCT`上实现特征`TRAIT`

* 形式: `impl TRAIT for STRUCT {/*method signatures*/}`

默认实现

* 如果想覆盖某个方法的默认实现, 则要提供方法的定义
* 如果要保留默认实现, 则不声明该方法签名

实现某个特征后, 可以像普通方法一样调用特征中的方法

孤儿规则(**orphan rule**)

* 如果打算在一个类型上实现一个特征, 那么该类型和该特征中至少有一个是定义在当前crate的
* 也就是说, 在某个crate中, 不能在一个外部类型上实现一个外部特征

例子

```rust
pub struct NewsArticle {
    pub headline: String,
    pub location: String,
    pub author: String,
    pub content: String,
}

impl Summary for NewsArticle {
    fn summarize(&self) -> String {
        format!("{}, by {} ({})", self.headline, self.author, self.location)
    }
}

pub struct Tweet {
    pub username: String,
    pub content: String,
    pub reply: bool,
    pub retweet: bool,
}

impl Summary for Tweet {
 	 fn summarize_author(&self) -> String {
        format!("@{}", self.username)
    }
}
```

## 2.3特征作为形参

类型注解 `impl TRAIT`, 表示实现`TRAIT`特征的类型

```rust
pub fn notify(item: impl Summary) {
    println!("Breaking news! {}", item.summarize());
}
```

## 2.4特征绑定

`impl TRAIT`句法是特征绑定的语法糖

```rust
pub fn notify(item: impl Summary) {
    println!("Breaking news! {}", item.summarize());
}
//也就是
pub fn notify<T: Summary>(item: T) {
    println!("Breaking news! {}", item.summarize());
}
```

特征绑定, 是指能将类型形参与特征绑定的机制

* 形式: `<T: TRAIT>`
  * 将`TRAIT`特征绑定到类型形参`T`上, 使得`T`的实参必须是实现`TRAIT`特征的类型
* 使用`+`句法绑定多个特征的形式: `<T: TRAIT1 + TRAIT2>`
  * `impl TRAIT`句法也适用: `impl TRAIT1 + TRAIT2`

* 使用`where`的形式: 

  * 在函数签名后跟`where`

  * 形如

    ```rust
    fn func<T>(t: T) -> return_type where T: TRAIT {}
    ```

  * 例子

    ```rust
    fn some_function<T, U>(t: T, u: U) -> i32
        where T: Display + Clone,
              U: Clone + Debug
    {/*body*/}
    //等价于
    fn some_function<T: Display + Clone, U: Clone + Debug>(t: T, u: U) -> i32 
    {/*body*/}
    ```

## 2.5返回实现某些特征的类型

可以在函数签名的返回类型处使用`impl TRAIT`句法, 表示返回类型是一个实现`TRAIT`特征的类型

```rust
fn returns_summarizable() -> impl Summary {
    Tweet {
        username: String::from("horse_ebooks"),
        content: String::from("of course, as you probably already know, people"),
        reply: false,
        retweet: false,
    }
}
```

## 2.6使用特征绑定以有条件地实现方法

对`impl`块使用特征绑定

* 类型实参只有实现给定的特征, 才能够实现对应的`impl`块
* 特别的: 只有实现给定特征的类型才能够实现其他特征, 这样的特征实现被称为*blanket implementation*
  * 如: `impl<T: Display> ToString for T {/*body*/}`
  * 表示为所有实现了`Display`的类型实现`ToString`特征

例子

```rust
use std::fmt::Display;

struct Pair<T> {
    x: T,
    y: T,
}

impl<T> Pair<T> {
    fn new(x: T, y: T) -> Self {
        Self {
            x,
            y,
        }
    }
}

impl<T: Display + PartialOrd> Pair<T> {
    fn cmp_display(&self) {
        if self.x >= self.y {
            println!("The largest member is x = {}", self.x);
        } else {
            println!("The largest member is y = {}", self.y);
        }
    }
}
```

* 实现了`Display + partialOrd`的类型实参才能实现`cmp_display`方法



# 3.寿命与引用有效性

每个引用都有寿命; 寿命也就是说引用处于有效的作用域

* 引用的寿命在大多数情况下能够被推断出; 如果引用的寿命能够以不同方式关联引用时, 必须显式地注解该引用的寿命
* Rust要求用泛式寿命形参来注解他们的关系, 以确保实际的引用在运行时绝对时有效的

## 3.1寿命防止悬空引用

寿命这个概念的主要目标是防止悬空引用

例子

```rust
{
    let r;

    {
        let x = 5;
        r = &x;
    }

    println!("r: {}", r);
}
```

编译得到

```rust
error[E0597]: `x` does not live long enough
  --> src/main.rs:7:5
   |
6  |         r = &x;
   |              - borrow occurs here
7  |     }
   |     ^ `x` dropped here while still borrowed
...
10 | }
   | - borrowed value needs to live until here

```

* 错误信息提到: `x`没有"活得足够长"; 
* 例子中`x`在第7行处离开作用域, 而`r`在外层作用域仍然是有效的; 我们会说`r`"活得更长"

## 3.2借用检查器

Rust编译器有一个借用检查器, 用于比较作用域以确定所有借用是否有效

* 被引用的对象要比引用的寿命长

例子

```rust
{
    let r;                // ---------+-- 'a
                          //          |
    {                     //          |
        let x = 5;        // -+-- 'b  |
        r = &x;           //  |       |
    }                     // -+       |
                          //          |
    println!("r: {}", r); //          |
}                         // ---------+
```

* 上例中, 将`r`的寿命标记为`'a`, 将`x`的寿命标记为`'b`; 可以看出`'b`比`'a`短
* 在编译时, Rust会比较这两者寿命的大小, 并发现`r`的寿命为`'a`而它引用一个寿命`'b`的内存, 因此拒绝编译此程序

## 3.3寿命注解lifetime annotation

寿命注解

* 作用: 描述多个引用的寿命之间的关系
* 注意: 寿命注解并不会改变任何引用的寿命

* 寿命形参的形式: `'name`

  * 寿命形参名字的规范: 尽量短, 字母小写, 通常使用`'a`

* 寿命形参注解跟在引用类型的`&`后面, 并使用一个空格分隔后面的类型注解

  ```rust
  &i32        // a reference
  &'a i32     // a reference with an explicit lifetime
  &'a mut i32 // a mutable reference with an explicit lifetime
  ```

### 函数签名中的寿命注解

在函数名后的`<>`中声明寿命形参, 然后可以在之后的类型注解中使用寿命形参

* 在函数中注解寿命时, 寿命注解是在函数签名中, 而不是在函数体中;  原因:
  * 对于函数体内的引用, Rust能够分析函数内的代码而不需要其他协助
  * 对于指向函数外部的内部引用, 和指向函数内部的外部引用, Rust自身无法分析出这些引用的寿命
    * 这也是为什么需要寿命注解

* 例子: 

  ```rust
  fn longest<'a>(x: &'a str, y: &'a str) -> &'a str {
      if x.len() > y.len() {
          x
      } else {
          y
      }
  }
  ```

  *  `x`, `y`和返回类型中都拥有寿命形参`'a`, 这表示返回值的寿命等于形参引用的最短寿命

  * 注意: `longest`函数并不需要知道`x`和`y`到底能够活多长, 只要符合限制即可

  ```rust
  fn main() {
      let string1 = String::from("long string is long");
  
      {
          let string2 = String::from("xyz");
          let result = longest(string1.as_str(), string2.as_str());
          println!("The longest string is {}", result);
      }
  }
  ```

  * 当具体的引用传递给`longest`函数时, `'a`的具体寿命等于`x`和`y`两者中的较短寿命, 因此返回的引用的寿命等于`x`和`y`中较短的寿命

### 关于寿命的思考

* 函数的具体功能, 决定了如何区指定寿命形参
* 当函数返回一个引用时, 返回类型的寿命形参需要与某个函数形参的寿命形参匹配
  * 如果返回的引用不指向任一形参, 那它必然指向函数中的局部值, 而这是一个悬空引用
* 引用寿命这个概念是为了将函数形参的寿命与返回值的寿命进行关联, 从而让Rust拥有足够的信息来进行检查

## 3.4结构定义中的寿命注解

可以定义存放引用的结构, 这需要为结构中的每个引用进行寿命注解

* 这些寿命注解的意思是: 结构实例不能比引用活得更久, 也就是说如果引用失效, 实例也会失效

```rust
struct ImportantExcerpt<'a> {
    part: &'a str,
}

fn main() {
    let novel = String::from("Call me Ishmael. Some years ago...");
    let first_sentence = novel.split('.')
        .next()
        .expect("Could not find a '.'");
    let i = ImportantExcerpt { part: first_sentence };
}
```

## 3.7寿命省略规则

> 注意: 以下的规则是编译器的规则

编译器的寿命省略规则(**lifetime elision rules**), 一系列可省略寿命注解的特定场景

* 术语
  * 输入寿命: 函数或方法的形参寿命
  * 输出寿命: 返回值的寿命

* 编译器使用三条规则来判断是否需要显式的寿命注解; 若检查这三条规则后仍然存在未知寿命的引用时, 编译器将会停止并报错
  * 这三条规则适用于`fn`定义和`impl`块
* 规则
  * 每个引用形参都有自己的寿命形参
  * 如果只有一个输入寿命形参, 则它被赋予所以输出寿命形参
  * 如果有多个输入寿命形参, 其中之一是`&self`或`&mut self`, 那么`self`的寿命被赋予所有输出寿命形参

## 3.8方法定义中的寿命注解

当为带有寿命形参的结构实现方法时, 那么它的句法与泛型类型形参的句法一样

例子

```rust
impl<'a> ImportantExcerpt<'a> {
    fn level(&self) -> i32 {
        3
    }
}
```

* `impl`和结构名后的`'a`是必要的, 而`&sefl`的寿命形参因省略规则而可以省略

## 3.9静态寿命

一个特殊的寿命: `'static`, 表示该引用可以存活于整个程序运行期间

* 所有字符串字面量都有`'static`寿命

  ```rust
  let s: &'static str = "I have a static lifetime.";
  ```

有时在编译返回的错误信息中会建议使用`'static`寿命; 不过将引用指定为 `'static` 之前，思考一下这个引用是否真的在整个程序的生命周期里都有效。你可能会考虑希望它一直有效，如果可能的话。大部分情况，代码中的问题是尝试创建一个悬垂引用或者可用的生命周期不匹配，请解决这些问题而不是指定一个 `'static` 的生命周期。