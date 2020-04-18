# 安装与卸载

[the Rust installation page](https://www.rust-lang.org/tools/install)

## 通过`rustup`安装`rust`

`rustup`命令行工具, 用于管理Rust版本和相关工具

### 在Linux or macOS上安装`rustup`

打开终端并输入下面命令

```shell
$ curl https://sh.rustup.rs -sSf | sh                                         
```

* 下载一个脚本, 用于安装`rustup`; 然后`rustup`会安装`rust`

## 通过`rustup`更新和卸载

更新到最新版本

```shell
$ rustup update
```

卸载Rust和`rustup`

```shell
$ rustup self uninstall
```

## 本地文档

```shell
$ rustup doc
```

# Hello, World!

## 创建一个项目目录(建议)

对Rust来说, 源文件如何存放是不怎么重要的; 但是, 建议为每个项目创建目录

## 编写和运行一个Rust程序

* 创建一个新的源文件, 名为`main.rs`

* 在文件中输入并保存

  ```rust
  fn main() {
      println!("Hello, world!");
  }
  ```

* 在终端输入

  ```rust
  $ rustc main.rs
  $ ./main
  ```

# Hello, Cargo!

Cargo是Rust的构建系统和包管理器

* 可以`build`代码
* 可以下载代码所依赖的库并构建这些库

### `Cargo`创建一个项目

创建一个名为`hello_cargo`的项目

```shell
$ cargo new hello_cargo
$ cd hello_cargo
```

### 构建和运行一个`Cargo`项目

* 在`hello_cargo`目录中

  ```shell
  $ cargo build
  ```
  * 该命令在`target/debug`中创建一个可执行文件`hello_crate`
  * 在`hello_cargo`目录中创建一个文件`Cargo.lock`, 用于记录依赖的准确版本

* 运行

  ```shell
  $ ./target/debug/hello_cargo # or .\target\debug\hello_cargo.exe on Windows
  ```

也可以执行以下命令, 完成上面的两个步骤

```shell
$ cargo run
```

### 构建一个发行版本

```shell
$ cargo build --release
```

* 该命令在`target/release`中创建一个可执行文件`hello_crate`

* 该命令为代码执行优化