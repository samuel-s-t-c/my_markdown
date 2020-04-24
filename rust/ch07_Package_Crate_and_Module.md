# 总结

模组系统(**module system**)

* 包(**package**): 用于创建, 测试和发布crate的Cargo特性

  * 一个包中可以有多个binary crate, 和一个可选的library crate
  * 可以从包中提取crate, 使其变成外部依赖(**external dependency**)

* Crate(箱子?): 模组树, 用于构成一个库或可执行程序

* 模组(**module**): 可以控制程序的组织, 作用域和路径

  * 为了便于管理, 大型程序通常分为多个模组(**module**)

* 路径(**path**): 命名一个对象的方式
  

[toc]

# 1.包和crate

crate

* crate就是一个可执行的二进制文件, 或者是一个库
* 作用: 
  * 一个crate将与其关联的功能放在一个作用域中
    * 如`rand` crate提供生成随机数的功能; 如果我们将`rand` crate引入我们项目的作用域中, 则`rand` crate的所有功能都可以通过`rand`这个名字来访问
  * 把一个crate的功能放在它自己的作用域中, 可以防止潜在的名字冲突
* crate root是构成某个crate的根模组(**root module**)的源文件; 在编译一个crate时, Rust编译器从该文件开始进行编译

包package

* 包由一个或多个crate组成
  * 只能有零个或一个library crate
  * 没有限制binary crate的数目
* 包中有一个`Cargo.toml`文件, 用于描述如何构建这些crate

使用`Cargo`创建一个包: 输入`cargo new`命令, 如`cargo new my-project`

* Cargo创建了一个目录, 该目录就是一个包; 该包中有一个`Cargo.toml`文件, 和一个`src`目录
  * 例子中, 创建一个名为`my-project`的包/目录
* Cargo按照规范在包的`src`目录中创建一个`main.rs`文件, 并作为与该包同名的binary crate的crate root
  * 例子中, 该binary crate与包同名, 名字都是`my-project`
  * 由于这是规范, `Cargo.toml`没有记录`src/main.rs`
  * 如果命令为`cargo new my-project --lib`, 则创建一个`lib.rs`文件
* 可以在包的`src`目录创建一个`lib.rs`文件, 那么该包有一个同名的library crate, 而且`lib.rs`是该library crate的crate root
  * 例子中, 该library crate的名字为`my-project`
* Cargo在构建时将crate root文件传递给`rustc`编译器, 以构建对应的库或二进制文件
* 可以在包目录中创建一个`src/bin`目录, 并在里面放置多个文件, 每个文件表示不同的binary crate

# 2.模组, 作用域和隐私

模组的作用

* 模组的机制使得我们可以将crate的代码分成多组, 这会便于使用和提高可读性

* 模组可以控制对象的隐私权; 隐私权, 具体可以分为
  * 公有(**public**): 某个对象可以被外部代码使用, 
  * 私有(**private**): 某个对象不能被外部代码使用, 只是内部的实现

定义模组

* 形式

  ```rust
  mod module_name {
      /*module body*/
  }
  ```

* 在模组中可以存放对象, 如结构, 枚举, 常数, 特征, 函数甚至其他的模组
* 也可以将对应的定义放在一起

模组之间的关系

* 当模组b包含模组a, 则称模组a是模组b的子模组, 模组b是模组a的父模组
* 每个模组文件都有一个隐式的模组`crate`; `crate`表示该文件的模组树的根部, 也就是当前的crate

例子: 一个表示餐厅的library crate

* 命令: `cargo new --lib restaurant`

* 在`src/lib.rs`中

  ```rust
  mod front_of_house {
      mod hosting {
          fn add_to_waitlist() {}
  
          fn seat_at_table() {}
      }
  
      mod serving {
          fn take_order() {}
  
          fn serve_order() {}
  
          fn take_payment() {}
      }
  }
  ```

* 模组树的结构: 其中`crate`是表示当前的library crate, 也就是`restaurant`

  ```
  crate
   └── front_of_house
       ├── hosting
       │   ├── add_to_waitlist
       │   └── seat_at_table
       └── serving
           ├── take_order
           ├── serve_order
           └── take_payment
  ```

# 3.路径

路径(**path**): 

* 绝对路径(**absolute path**), 是指从根部模组开始的路径
  * 使用某个crate的名字或者`crate`开始一个绝对路径
    * 当前文件所表示的crate, 必须用`crate`来表示
* 相对路径(**relative path**), 是指从当前模组开始的路径
  * 使用`self`, `super`或者当前模组中的某个名字开始一个相对路径
    * `self`表示所在的模组, `super`表示所在模组的父模组

路径的表示: 

* 无论绝对或相对路径, 使用`::`双引号来分隔一个或多个标识符

注意: 

* 如果想要访问模组中的对象, 必须保证路径中的模组以及该对象是可访问的

例子: 在`restaurant`包的`src/lib.rs`中, 

```rust
pub fn eat_at_restaurant() {
    // Absolute path
    crate::front_of_house::hosting::add_to_waitlist();

    // Relative path
    front_of_house::hosting::add_to_waitlist();
}
```

* 注意: 路径上的对象有些是无法访问的, 因此编译会报错, 下小节将修复错误
* 对于`eat_at_restaurant`而言, 它位于根部模组/crate root, 与`front_of_house`模组是兄弟关系
* 绝对路径: `crate::front_of_house::hosting::add_to_waitlist()`
  * `crate`是指当前所在的crate, 也就是`restaurant`
* 相对路径: `front_of_house::hosting::add_to_waitlist()`
  * 从可见的`front_of_house`开始

## 3.1模组的隐私权与`pub`

模组机制不仅很好地组织代码, 也定义隐私边界(**privacy boundary**)

* 隐私边界封装了该模组的实现细节,  使得外部代码无法访问边界后的东西

隐私权

* 在模组中的所有对象(**item**)默认是私有的;
  * 在父模组中的对象不能使用子模组的私有对象
  * 子模组的对象可以使用父模组的对象, 以及更上层的祖模组(**ancestor module**)
* `pub`关键字可以使对象变为公有的

例子;

```rust
mod front_of_house {
    pub mod hosting {
        pub fn add_to_waitlist() {}
    }
}

pub fn eat_at_restaurant() {
    // Absolute path
    crate::front_of_house::hosting::add_to_waitlist();

    // Relative path
    front_of_house::hosting::add_to_waitlist();
}
```

## 3.2以`super`开始的相对路径

`super`表示当前所在模组的父模组

例子

```rust
fn serve_order() {}

mod back_of_house {
    fn fix_incorrect_order() {
        cook_order();
        super::serve_order();
    }

    fn cook_order() {}
}
```

* 例子中`fix_incorrect_order`所在的模组是`back_of_house`, 而`back_of_house`的父模组是`crate`, 或者说是`restaurant`

## 3.3模组中的`struct`和`enum`

在模组中, 在`struct`定义前使用`pub`, 使得该结构公开, 但是结构内部的字段仍然是私有的

* 可以逐一决定每个字段是否公开
* 如果结构中存在私有的字段, 且外部代码需要用到该结构, 通常提供一个公有的返回该结构的方法
  * 如果没有该方法, 则外部代码无法得到该结构的实例

在模组中, 如果使得一个枚举类公开, 则其所有变体都是公有的

例子

```rust
mod back_of_house {
    pub struct Breakfast {
        pub toast: String,
        seasonal_fruit: String,
    }

    impl Breakfast {
        pub fn summer(toast: &str) -> Breakfast {
            Breakfast {
                toast: String::from(toast),
                seasonal_fruit: String::from("peaches"),
            }
        }
    }
}

pub fn eat_at_restaurant() {
    // Order a breakfast in the summer with Rye toast
    let mut meal = back_of_house::Breakfast::summer("Rye");
    // Change our mind about what bread we'd like
    meal.toast = String::from("Wheat");
    println!("I'd like {} toast please", meal.toast);

    // The next line won't compile if we uncomment it; we're not allowed
    // to see or modify the seasonal fruit that comes with the meal
    // meal.seasonal_fruit = String::from("blueberries");
}
```

```rust
mod back_of_house {
    pub enum Appetizer {
        Soup,
        Salad,
    }
}

pub fn eat_at_restaurant() {
    let order1 = back_of_house::Appetizer::Soup;
    let order2 = back_of_house::Appetizer::Salad;
}
```

# 4.引入路径与`use`关键字

`use`关键字可以将一条路径引入到一个作用域中; 当引入路径后, 可以像局部对象那样使用该路径上的对象

* 形式: `use PATH`
  
  * `PATH`可以是绝对路径或相对路径
* `use`引入路径时, 也要检查能否访问该路径

* 例子

  ```rust
  mod front_of_house {
      pub mod hosting {
          pub fn add_to_waitlist() {}
      }
  }
  //相对路径的写法: use front_of_house::hosting;
  use crate::front_of_house::hosting;
  
  pub fn eat_at_restaurant() {
      hosting::add_to_waitlist();
      hosting::add_to_waitlist();
      hosting::add_to_waitlist();
  }
  ```

  * 引入了`hosting`模组, 因此`eat_at_restaurant`可以将`hosting`模组当作兄弟关系那样使用

## 4.1创建惯用`use`路径

用`use`引入一个函数时, 惯用做法是引入该函数的父模组, 而不是直接引入该函数

* 原因是: 这样做的话, 在使用该函数时需要指定其父模组, 从而在阅读代码时能够清晰地知道该函数不是在本地定义的

用`use`引入结构, 枚举类和其他对象时, 惯用做法是引入该对象

* 并没有什么原因要这样做; 只是习惯做法而已
* 注意: 当要引入两个相同名字的对象时, 不能这么做

## 4.2`as`关键字提供别名

形式: `use PATH as NAME`

* `NAME`是`PATH`在当前作用域的别名

```rust
use std::fmt::Result;
use std::io::Result as IoResult;

fn function1() -> Result {
    // --snip--
}

fn function2() -> IoResult<()> {
    // --snip--
}
```

## 4.3用`pub use`再出口名字

* 当使用`use`向作用域引入一个名字后, 该名字默认是私有的, 即对外部代码不可见

* 如果在`use`前添加`pub`, 形如`pub use PATH`, 则该名字是公有的
  * 这用法被称为再出口(**re-exporting**)

例子

```rust
mod front_of_house {
    pub mod hosting {
        pub fn add_to_waitlist() {}
    }
}

pub use crate::front_of_house::hosting;

pub fn eat_at_restaurant() {
    hosting::add_to_waitlist();
    hosting::add_to_waitlist();
    hosting::add_to_waitlist();
}
```

* 例子中, 将`hosting`再出口, 那么外部代码可以通过`hosting::add_to_waitlist`调用`add_to_waitlist`函数

## 4.4使用外部rust包

如何使用外部依赖/外部rust包

* 在`Cargo.toml`的`[dependencies]`区域下添加外部rust包, 为该rust包添加对应的依赖
  * 如果本地没有对应的外部rust包及其所需的依赖, Cargo会根据情况下载对应的包

* 在程序中用`use`引入rust包

`std`库

* Rust语言自带`std`库, 所以不用在`Cargo.toml`文件中包含`std`
* 如果要使用`std`中的功能, 还是要用`use`来引入

Rust社区的成员已经在[crates.io](https://crates.io/)中制造并上传了许多rust包

## 4.5使用嵌套路径

当需要使用某个包中或某个模组中的多个对象, 可以使用嵌套路径

* 在`use`语句的`PATH`部分, 先指定路径的公有部分, 再使用`::{}`; 在花括号中, 指定路径的不同部分, 用逗号分隔

  * 特别的是, 在嵌套路径的花括号中使用`self`, 表示引入路径的公有部分

* 例子

  ```rust
  use std::io;
  use std::cmp::Ordering;
  //等价于
  use std::{cmp::Ordering, io};
  
  use std::io;
  use std::io::Write;
  //等价于
  use std::io::{self, Write};
  ```

  * `use std::io::{self, Write};`中的`self`表示引入`std::io`

## 4.6 glob运算符

如果想要将某个路径中的公有对象引入, 可以在该路径后跟`*`

* `*`是glob运算符

例子

```rust
use std::collections::*;
```

* 例子引入了`std::collection`中的所有公有对象

# 5.将模组分成多个文件

当模组变得很大时, 将它们的定义放在另外的文件中, 这能够更好地管理代码

* 使用形式 `mod NAME;`声明一个模组, 而不是定义模组
* Rust会查找同名文件`NAME.rs`, 并从中加载内容

例子: 以下三种文件组织都是表示同一种模组树

* 文件`src/lib.rs`

  ```rust
  mod front_of_house {
      pub mod hosting {
          pub fn add_to_waitlist() {}
      }
  }
  
  pub use crate::front_of_house::hosting;
  
  pub fn eat_at_restaurant() {
      hosting::add_to_waitlist();
      hosting::add_to_waitlist();
      hosting::add_to_waitlist();
  }
  ```

* 分成两个文件`src/lib.rs`和`src/front_to_house`

  * 将其中的`front_of_house`模组定义移到同名文件`src/front_to_house.rs`中

  * 文件`src/lib.rs`

    ```rust
    mod front_of_house;
    
    pub use crate::front_of_house::hosting;
    
    pub fn eat_at_restaurant() {
        hosting::add_to_waitlist();
        hosting::add_to_waitlist();
        hosting::add_to_waitlist();
    }
    ```

  * 文件`src/front_to_house`

    ```rust
    pub mod hosting {
        pub fn add_to_waitlist() {}
    }
    ```

  * 目录结构

    ```
    src
    ├── front_of_house.rs
    └── lib.rs
    ```

* 分成三个文件`src/lib.rs`, `src/front_to_house`和`src/front_to_house/hosting.rs`

  * 进一步从`front_of_house`文件中提取`hosting`模块的定义

  * 文件`src/lib.rs`

    ```rust
    mod front_of_house;
    
    pub use crate::front_of_house::hosting;
    
    pub fn eat_at_restaurant() {
        hosting::add_to_waitlist();
        hosting::add_to_waitlist();
        hosting::add_to_waitlist();
    }
    ```

  * 文件`src/front_to_house`

    ```rust
    pub mod hosting;
    ```

  * 文件`src/front_to_house/hosting.rs`, 注意: 该文件在目录`src/front_to_house/`中

    ```rust
    pub fn add_to_waitlist() {}
    ```

  * 目录结构

    ```
    src
    ├── front_of_house.rs
    |
    ├── lib.rs
    |
    └── front_of_house
    	└── hosting.rs	
    ```