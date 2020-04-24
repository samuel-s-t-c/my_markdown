# 总结

进一步认识Cargo和Crates.io

* 使用release配置来自定义构建
* 向[crates.io](https://crates.io/)发布(*publish*)库
* 用工作空间来组织大型程序
* 安装[crates.io](https://crates.io/)的二进制文件
* 使用自定义命令来扩展Cargo

* 更多功能, 请查看[documentation](https://doc.rust-lang.org/cargo/)

[toc]

# 1.使用发行配置来自定义构建

在Rust中, 发行配置(*release profile*)是预定义和可定制的, 允许程序员更灵活地控制代码编译过程

* 每个配置都是彼此相互独立的

* Cargo的两个主要配置
  * `dev`配置: 运行`cargo build`时使用该配置, 其默认配置为开发时的优秀配置
  * `release`配置: 运行`cargo build --release`时使用该配置, 其默认配置为release时的优秀配置

自定义发行配置: 在项目的*Cargo.toml*文件中

* 如果没有任何`[profile.*]`区域时, Cargo对每个配置都采用默认设置

* 有`[profile.*]`区域时, 其中的设置将覆盖默认配置中对应的设置

  * `*`可以是`dev`, `release`

  ```rust
  [profile.dev]
  opt-level = 0
  
  [profile.release]
  opt-level = 3
  ```

  * 例子中的`opt-level`设置是用于控制优化等级的, 范围是0到3

# 2.向crates.io发布库

可以向[crates.io](https://crates.io/)发布你自己的包, 与其他社区成员分享你的代码

* [crates.io](https://crates.io/)的crate registry主要用来托管开源代码, 并分发包的源代码

## 2.1编写有用的文档注释

文档注释

* 作用: 用于生成HTML文档; 这些HTML文档展示公有API文档注释的内容

* 语法:

  *  `///`
    * 为后跟的item提供文档注释
  * `//!`
    * 为该文档注释所在的item提供文档注释

* 说明: 支持Markdown标记语言

* 例子

  * 为后跟的item提高文档注释

    ```rust
    /// Adds one to the number given.
    ///
    /// # Examples
    ///
    /// ```
    /// let arg = 5;
    /// let answer = my_crate::add_one(arg);
    ///
    /// assert_eq!(6, answer);
    /// ```
    pub fn add_one(x: i32) -> i32 {
        x + 1
    }
    ```
    * 以上是函数`add_one`的文档注释

  * 为文档注释所在的item提供文档注释: 

    ```rust
    //! # My Crate
    //!
    //! `my_crate` is a collection of utilities to make performing certain
    //! calculations more convenient.
    pub fn add_one(x: i32) -> i32 {
        x + 1
    }
    ```

    * 假设文档注释在`my_crate`项目的文件`src/lib.rs`中, 则例子中是`my_crate`的文档注释
    * 不是函数`add_one`的文档注释

`cargo doc`命令

* 作用: 从文档注释中生成HTML文档
  * 生成的HTML文档在*target/doc*目录中
* 参数:
  * `--open`: 生成当前crate文档注释的HTML, 并在浏览器中打开生成的HTML

### 文档注释中常用的Markdown标题

* `Examples`: 使用例子
* `Panics`: 函数可能会`panic!`的场景
* `Erros`: 如果这个函数返回`Result`, 描述可能会出现何种错误以及什么情况会造成这些错误
* `Safety`: 如果这个函数使用`unsafe`代码, 解释函数为什么`unsafe`, 并描述该函数所期望的不变条件

### 文档注释作为测试

`cargo test`命令可以测试文档注释中的Markdown代码块的代码

## 2.2`pub use`导出公有API

`pub use`

* 作用: 将一个位于某个位置的公有item在另一个位置公开可见, 使得该item像是在新位置定义那样

  * 这被称为重导出*re-export*

* 例子: 

  * `art`库中的文件*src/lib.rs*

    ```rust
    //! # Art
    //!
    //! A library for modeling artistic concepts.
    
    pub use self::kinds::PrimaryColor;
    pub use self::kinds::SecondaryColor;
    pub use self::utils::mix;
    
    pub mod kinds {
        /// The primary colors according to the RYB color model.
        pub enum PrimaryColor {
            Red,
            Yellow,
            Blue,
        }
        
        /// The secondary colors according to the RYB color model.
        pub enum SecondaryColor {
            Orange,
            Green,
            Purple,
        }
    }
    
    pub mod utils {
        use crate::kinds::*;
        
        /// Combines two primary colors in equal amounts to create
        /// a secondary color.
        pub fn mix(c1: PrimaryColor, c2: PrimaryColor) -> SecondaryColor {
            // --snip--
        }
    }
    ```

  * 使用`art`库

    ```rust
    use art::PrimaryColor;
    use art::mix;
    
    fn main() {
        // --snip--
    }
    ```

## 2.3创建Crates.io账号

在向crates.io发布任何crate之前, 需要创建一个crates.io账号, 并获得一个API令牌(*token*)

具体步骤为

1. 访问位于 [crates.io](https://crates.io/) 的首页并使用GitHub账号登陆
   * (2020/4/23)目前只支持GitHub 账号
2. 登录后, 进入账号设置并创建/获得一个API令牌
3. 使用API令牌运行`cargo login`命令, 类似于`$ cargo login abcdefghijklmnopqrstuvwxyz012345`
   * 该命令会告知Cargo, 并将令牌存储在*~/.cargo/credentials*

注意: 

* 谨慎保存该令牌, 不要与其他人分享

## 2.4为crate添加元数据

在发布之前, 你需要在*Cargo.toml*的`[package]`区域中添加一些元数据

* `name`字段要求一个唯一的名称
  *  `crates.io`上的crate名称采用先到先得的原则
* `license`字段: 必须有的字段, 需要一个*license*标记符值;
  * Rust社区成员常用与Rust本身相同的许可: *MIT OR Apache-2.0*双许可; `OR`用来分隔多个许可
  * 可使用的许可: [Linux Foundation’s Software Package Data Exchange (SPDX)](http://spdx.org/licenses/)
  * 如果使用不在SPDX上的许可, 你需要将*license*文本放入一个文件, 将该文件包含进项目中, 接着使用`license-file`来指定文件名而不是`license`字段
* `description`字段: 必须有的字段, 对该crate进行概述

例子: *Cargo.toml*

```toml
[package]
name = "guessing_game"
version = "0.1.0"
authors = ["Your Name <you@example.com>"]
description = "A fun game where you guess what number the computer has chosen."
license = "MIT OR Apache-2.0"

[dependencies]
```

## 2.5发布到Crates.io

* 发布 crate 时请多加小心, 因为每次发布都是永久性的
  * 对应的版本不能被覆盖, 其代码不能被删除

* 命令`cargo publish`用于发布到指定网站, 默认为crates.io

* crate的版本号遵循语义化版本规则( [Semantic Versioning rules](http://semver.org/))

## 2.6从crates.io中撤销版本

`cargo yank`, 用于撤销版本

* 撤销版本, 是指阻止新项目将该版本加入到依赖中, 而不是删除crate

  * 所有带有*Cargo.lock*的项目的依赖不会被破坏, 同时任何新生成的*Cargo.lock*将不能使用被撤回的版本

* 用法: 

  * `--vers` 指定要撤销的版本
  * `--undo`恢复撤销的版本

* 例子

  ```shell
  $ cargo yank --vers 1.0.1
  $ cargo yank --vers 1.0.1 --undo
  ```

# 3.Cargo工作空间

随着项目的开发, library crate一直增大; 当希望将其进一步拆分成多个library crate时, 可以使用一个叫工作空间的功能, 有助于管理多个相关的协同开发的包

* 工作空间, 是指共享同一个*Cargo.lock*和输出目录的一组包

在工作空间中

* 只有一个*target*目录, 是在顶层目录中
  * 工作空间中的其他包没有自己的*target*目录

## 3.1创建一个工作空间

在工作空间中运行命令`cargo build`会构建所有crate

* 参数: 
  * `-p`: 指定一个crate以执行命令
* 如:
  * `cargo test -p adder`

一种常用的创建工作空间的方式

1. 创建一个目录
2. 创建一个*Cargo.toml*; 
   * 在该文件中以`[workspace]`区域开始; 不会包含`[package]`或其他元数据
3. 在`[workspace]`中的`members`指定工作空间成员的路径
   * 工作空间成员就是包
4. 使用`cargo new`命令创建对应的工作空间成员

例子:

1. 命令行

   ```shell
   $ mkdir add
   $ cd add
   ```

2. *add*目录中的文件*Cargo.toml*

   ```toml
   [workspace]
   
   members = [
       "adder",
       "add-one",
   ]
   ```

3. 命令行

   ```shell
   $ cargo new adder
   $ cargo new add-one --lib
   ```

4. 此时运行`cargo build`命令; 在*add*目录结构如下

   ```
   add
   ├── Cargo.lock
   ├── Cargo.toml
   ├── add-one
   │   ├── Cargo.toml
   │   └── src
   │       └── lib.rs
   ├── adder
   │   ├── Cargo.toml
   │   └── src
   │       └── main.rs
   └── target
   ```

## 3.2工作空间中的依赖

工作空间中的某个成员`A`可以依赖于另一个成员`B`

* 需要在`A`的*Cargo.toml*中指定成员`B`的名字与路径
* 如果只有名字, 没有路径, 则Cargo假设名字对应的crate是一个外部依赖

* 例子: `adder`包依赖于`add-one`包; 在文件*adder/Cargo.toml*中的`[dependencies]`区域

  ```toml
  [dependencies]
  
  add-one = { path = "../add-one" }
  ```

工作空间只在根目录中有一个*Cargo.lock*文件

* 目的是, 保证所有的crate都使用完全相同版本的依赖, 使得crate之间相互兼容

* 如果在不同成员中的*Cargo.toml*中有相同的外部依赖, Cargo会将其都解析为同一版本并记录到唯一的*Cargo.lock*中

## 3.3工作空间中运行测试

`cargo test`命令会运行工作空间中的所有测试

* 使用`-p`标记以指定一个crate

## 3.4发布与工作空间

工作空间中的每个crate都需要独立发布

* 需要进入某个crate目录中运行`cargo publich`, 以发布该crate

* `cargo publish`没有类似`--all`标记以发布全部crate, 也没有`-p`标记以发布一个crate

# 4.使用`cargo install`安装来自crate.io的二进制文件

`cargo install`

* 用于本地安装和使用binary crate
  * 不是用于替换系统的包, 只是便于开发者安装在[crates.io](https://crates.io/)上共享的工具
* 只能安装有二进制目标文件的包
  * 二进制目标文件(*binary target*)是一个可运行程序, 是在crate中有*src/main.rs*或者其他指定为二进制文件时所创建的
  * 库目标文件(*library target*), 自身不能执行但适合被包含在其他程序中的

* 安装目录是Rust安装根目录的*bin*文件夹中;
  * 如果使用*rustup.rs*安装Rust, 并没有自定义任何配置, 那安装目录将是*$HOME/.cargo/bin*
* 安装后, 将安装目录添加到`$PATH`环境变量中就能够运行通过`cargo install`安装的程序了。

例子: *ripgrep*

```shell
$ cargo install ripgrep
    Updating crates.io index
  Downloaded ripgrep v11.0.2
  Downloaded 1 crate (243.3 KB) in 0.88s
  Installing ripgrep v11.0.2
--snip--
   Compiling ripgrep v11.0.2
    Finished release [optimized] target(s) in 3m 10s
  Installing ~/.cargo/bin/rg
   Installed package `ripgrep v11.0.2` (executable `rg`)xxxxxxxxxx11 1$ cargo install ripgrep2    Updating crates.io index3  Downloaded ripgrep v11.0.24  Downloaded 1 crate (243.3 KB) in 0.88s5  Installing ripgrep v11.0.26--snip--7   Compiling ripgrep v11.0.28    Finished release [optimized] target(s) in 3m 10s9  Installing ~/.cargo/bin/rg10   Installed package `ripgrep v11.0.2` (executable `rg`)11$ cargo install ripgrepshell
```

* 例子的第9行中展示了安装目录和名称

# 5.自定义命令以扩展Cargo

如果`$PATH`中有类似`cargo-something`的二进制文件, 就可以通过`cargo something`来像 Cargo 子命令一样运行它

* 通过命令`cargo --list`来展示所有自定义命令