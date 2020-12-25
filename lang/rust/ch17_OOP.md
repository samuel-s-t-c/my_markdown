# 总结

可以通过特征对象获得一些面向对象的特性

[toc]

# 1.面对对象语言的特点

对于面对对象语言所需的特性有哪些这个问题, 编程社区并没有一个一致的认识; 不过, OOP语言通常都会有这几个特点: 

* 对象
* 封装
* 继承

## 1.1对象

在*Design Patterns: Elements of Reusable Object-Oriented Software*书中, 这么定义OOP

> Object-oriented programs are made up of objects. 
>
> An *object* packages both data and the procedures that operate on that data. 
>
> The procedures are typically called *methods* or *operations*.

按照这个定义, Rust是面对对象的

## 1.2封装

封装的思想是: 

* 对于用户代码, 对象的实现细节是不可访问的

Rust可以实现封装

## 1.3继承

继承

* 一个类型的对象可以继承于其他类型对象的定义, 从而得到父类的数据和行为, 而无需重新定义

多态, 通常与继承一起出现

* 能够处理多种类型的代码

  Rust没有继承, 不过提供了与继承不同的解决方案: 特征对象

# 2.特征对象Trait Object

定义一个特征对象

* 智能指针的`<>`中指定`dyn`和相关的特征
  * 如`Box<dyn Err>`
* `&`后跟`dyn`和空格, 然后是相关的特征
  * 如`&dyn Err`

特征对象和有特征边界的泛型的区别; 以结构为例

* 使用带特征边界的泛型的结构: 泛型类型参数每次只能被一个具体的类型替换, 从而生成特定的结构实例
* 使用特征对象的结构: 在运行时可以接受多个不同的类型

例子

*lib.rs*

```rust
pub trait Draw {
    fn draw(&self);
}

pub struct Screen {
    pub components: Vec<Box<dyn Draw>>,
}

impl Screen {
    pub fn run(&self) {
        for component in self.components.iter() {
            component.draw();
        }
    }
}

pub struct Button {
    pub width: u32,
    pub height: u32,
    pub label: String,
}

impl Draw for Button {
    fn draw(&self) {
        // code to actually draw a button
    }
}
```

*main.rs*

```rust
use gui::Draw;

struct SelectBox {
    width: u32,
    height: u32,
    options: Vec<String>,
}

impl Draw for SelectBox {
    fn draw(&self) {
        // code to actually draw a select box
    }
}

use gui::{Button, Screen};

fn main() {
    let screen = Screen {
        components: vec![
            Box::new(SelectBox {
                width: 75,
                height: 10,
                options: vec![
                    String::from("Yes"),
                    String::from("Maybe"),
                    String::from("No"),
                ],
            }),
            Box::new(Button {
                width: 50,
                height: 10,
                label: String::from("OK"),
            }),
        ],
    };

    screen.run();
}
```

## 2.1动态调度

态化所产生的实现代码进行静态调度(*static dispatch*)

* 对泛型使用特征边界时编译器进行单态化, 即编译器为每个泛型类型实参生成一个对应的非泛型函数或方法实现

* 单态化代码进行静态调度, 此时编译器在编译时得知所调用的方法是哪个.

动态调度, 与静态调度相反

* 编译器在编译时不知道所调用的方法具体是哪个; 编译器生成在运行时确定具体方法的代码, 以进行动态调度
  * 由于查找发生在运行时, 所以增加运行时开销
* 当使用特征对象时, Rust肯定使用动态调度.
  * 在编译时无法确定具体的类型; 在运行时通过特征对象中的指针确定所调用的具体方法
* 动态调度会阻止编译器进行函数内联化, 从而阻止一些相关的优化

## 2.2特征对象要求对象安全

特征对象中的特征只能是对象安全的特征(*obejct-safe trait*)

对于使得特征对象安全的属性, Rust有复杂的规则来控制; 不过在使用特征对象时, 只涉及到其中两条规则; 只有特征中所有方法满足以下要求, 该特征就是对象安全的

* 返回类型不是`self`
* 没有泛型类型形参

# 3.实现一种面对对象设计模式

 状态模式(*state pattern*), 是一种面对对象设计模式

* 一个值有某些内部状态, 它由一组状态对象(*state object*)表示
  * 状态对象共同拥有同样的功能
  * 每个状态对象负责其自身行为; 当需要变为另一种状态时, 它自己负责处理
* 一个值的行为根据内部状态而改变

* 值本身不知道不同状态的行为, 和何时状态会改变

使用状态模式的意义

* 当程序的业务需求改变时, 无需改变值如何持有状态的代码, 或者使用值的代码;
* 只需要更新某个状态对象中的代码来改变其规则, 或者是增加更多的状态对象

例子: 实现一个增量式的博文发布工作流; 其功能有

* 博文从空白的草稿开始
* 若完成草稿, 请求审核博文
* 若博文过审, 发表该博文
* 只有过审的博文才能返回要打印的内容; 这样的话, 不会意外发表无过审的博文

*main.rs*

```rust
use blog::Post;

fn main() {
    let mut post = Post::new();

    post.add_text("I ate a salad for lunch today");
    assert_eq!("", post.content());

    post.request_review();
    assert_eq!("", post.content());

    post.approve();
    assert_eq!("I ate a salad for lunch today", post.content());
}
```

*lib.rs*

```rust
pub struct Post {
    state: Option<Box<dyn State>>,
    content: String,
}

impl Post {
    pub fn new() -> Post {
        Post {
            state: Some(Box::new(Draft {})),
            content: String::new(),
        }
    }

    pub fn add_text(&mut self, text: &str) {
        self.content.push_str(text);
    }

    pub fn content(&self) -> &str {
        self.state.as_ref().unwrap().content(self)
    }

    pub fn request_review(&mut self) {
        if let Some(s) = self.state.take() {
            self.state = Some(s.request_review())
        }
    }

    pub fn approve(&mut self) {
        if let Some(s) = self.state.take() {
            self.state = Some(s.approve())
        }
    }
}

trait State {
    // --snip--
    fn request_review(self: Box<Self>) -> Box<dyn State>;
    fn approve(self: Box<Self>) -> Box<dyn State>;

    fn content<'a>(&self, post: &'a Post) -> &'a str {
        ""
    }
}

// --snip--

struct Draft {}

impl State for Draft {
    fn request_review(self: Box<Self>) -> Box<dyn State> {
        Box::new(PendingReview {})
    }

    fn approve(self: Box<Self>) -> Box<dyn State> {
        self
    }
}

struct PendingReview {}

impl State for PendingReview {
    fn request_review(self: Box<Self>) -> Box<dyn State> {
        self
    }

    fn approve(self: Box<Self>) -> Box<dyn State> {
        Box::new(Published {})
    }
}

struct Published {}

impl State for Published {
    // --snip--
    fn request_review(self: Box<Self>) -> Box<dyn State> {
        self
    }

    fn approve(self: Box<Self>) -> Box<dyn State> {
        self
    }

    fn content<'a>(&self, post: &'a Post) -> &'a str {
        &post.content
    }
}


```

## 3.1另一种不同的实现: 将状态和行为编码为类型

这个实现不完全遵守面向对象的状态模式

*main.rs*

```rust
use blog::Post;

fn main() {
    let mut post = Post::new();

    post.add_text("I ate a salad for lunch today");

    let post = post.request_review();

    let post = post.approve();

    assert_eq!("I ate a salad for lunch today", post.content());
}
```

*lib.rs*

```rust
pub struct Post {
    content: String,
}

pub struct DraftPost {
    content: String,
}

impl Post {
    pub fn new() -> DraftPost {
        DraftPost {
            content: String::new(),
        }
    }

    pub fn content(&self) -> &str {
        &self.content
    }
}

impl DraftPost {
    // --snip--
    pub fn add_text(&mut self, text: &str) {
        self.content.push_str(text);
    }

    pub fn request_review(self) -> PendingReviewPost {
        PendingReviewPost {
            content: self.content,
        }
    }
}

pub struct PendingReviewPost {
    content: String,
}

impl PendingReviewPost {
    pub fn approve(self) -> Post {
        Post {
            content: self.content,
        }
    }
}
```