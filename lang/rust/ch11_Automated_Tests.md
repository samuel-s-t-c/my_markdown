# 总结

Rust测试功能

* 适用于测试的注解和宏

* 运行测试时的默认行为和选项
* 如何将测试组织成单元测试和集成测试

[toc]

# 1.编写测试

这里所说的"测试", 是指一种函数, 能够验证非测试代码是否按预期运行; 测试通常执行三个操作

1. 设置任何所需的数据或状态
2. 运行需要测试的代码
3. 断言所期望的结果

## 概述

简单来说, Rust的测试是一个带有`test`属性的函数

* 属性是关于Rust代码段的元数据; 如`derive`属性

将一个函数变成测试函数的方式: 在该函数的`fn`所在行前添加`#[test]`

* 注意: 每个测试函数都要有一个`#[test]`

* 当用`Cargo`新建一个库项目时, 自动生成一个测试模组, 模组里有一个测试函数

运行测试: 执行`Cargo test`命令

* 执行该命令后, Rust将构建一个测试运行器; 该程序将运行项目中所有需要测试的测试函数, 并报告每个测试函数的测试情况
* 也会对API文档中的代码示例进行测试

测试运行器

* 测试函数中某处发生`panic`, 则对应的测试失败
* 每一个测试函数都在一个新线程中运行; 当主线程发现某个测试线程死亡, 则将对应的测试函数标记为失败

## 断言宏

### `assert!`宏

`assert!`宏接受一个布尔值

* 如果值为`true`, 则`assert!`不做任何事
* 如果值为`false`, `assert!`将调用`panic!`

### `assert_eq!`和`assert_ne!`

`assert_eq!`宏接受两个值, 并判断两者是否相等

* 如果两者相等, 则无事发生
* 如果两者不相等, `assert_eq!`将打印这两个值, 并调用`panic!`

`assert_ne!`宏接受两个值, 并判断两者是否不相等

* 如果两者不相等, 则无事发生
* 如果两者相等, `assert_ne!`将打印这两个值, 并调用`panic!`

在底层, `assert_eq!`和`assert_ne!`分别使用`==`和`!=`运算符, 而且在打印实参时使用调试格式

* 因此, 用于`assert_eq!`和`assert_ne!`的类型必须实现了`PartialEq`和`Debud`特征

### 自定义的失败信息

可以向 `assert!`、`assert_eq!` 和 `assert_ne!` 宏传递可选的失败信息形参

在 `assert!` 的一个必需实参和 `assert_eq!` 和 `assert_ne!` 的两个必需实参之后指定的所有参数都会传递给 `format!` 宏

* 因此, 可以传递一个包含 `{}` 占位符的格式字符串和需要放入占位符的值。

  ```rust
  #[test]
  fn greeting_contains_name() {
      let result = greeting("Carol");
      assert!(
          result.contains("Carol"),
          "Greeting did not contain name, value was `{}`", result
      );
  }
  ```

## `should_panic`属性: 检查`panic`

`should_panic`属性: 用于一个测试函数

* 如果测试函数中的代码`panic`时, 则测试通过; 如果代码没有`panic`, 则测试失败

* 可选的`expected`参数, 是一段文本; 如果有`expected`参数, 则错误信息中包含其提供的文本时测试才通过

```rust
   #[test]
    #[should_panic(expected = "Guess value must be less than or equal to 100")]
    fn greater_than_100() {
        Guess::new(200);
    }
```

* 例子中期望错误信息中包含"Guess value must be less than or equal to 100"

## 使用`Result`的测试

可以使用`Result<T, E>`编写测试; 因此, 也能使用`?`操作符来编写测试

* 如果返回`Err`变体, 则测试失败
* 如果返回`Ok`变体, 则测试通过

注意: 如果使用`Result<T, E>`编写测试函数, 则不能使用`#[should_panic]`注解

```rust
#[cfg(test)]
mod tests {
    #[test]
    fn it_works() -> Result<(), String> {
        if 2 + 2 == 4 {
            Ok(())
        } else {
            Err(String::from("two plus two does not equal four"))
        }
    }
}
```



# 2.控制测试的运行方式

`cargo test`命令: 测试模式下编译代码并运行生成的测试运行器

* 默认行为: 并行运行所有测试, 并截获测试运行过程中产生的输出, 阻止他们被显示出来, 使得阅读测试结果相关的内容变得更容易
* 一些命令行参数是传递给 `cargo test`，而另外一些是传递给生成的测试二进制文件
  * 为了分隔这两种参数, 需要先列出`cargo test`的参数, 接着使用`--`分隔, 然后是传递给测试运行器的参数
  * `cargo test [OPTIONS] [TESTNAME] [-- <args>...]`
    * `OPTIONS`是传递给`cargo new`的参数
    * `TESTNAME`是字符串
    * `args`是传递给测试运行器的参数
  * 例子: 
    * `cargo test --help`命令用于展示 `cargo test` 的有关参数
    * `cargo teset -- --help`命令用于展示在分隔符 `--` 之后可以使用的有关参数

## 并行或连续地运行测试

当测试运行器中有多个测试时, 默认使用线程以并行运行这些测试

* 因此, 如果要使用默认运行的方式, 应该确保测试不会相互依赖, 或依赖任何共享的状态, 包括共享的环境, 如当前工作目录或环境变量等

可以给测试运行器传递`--test-threads`标记和希望使用线程的数量

* 如`$ cargo test -- --test-threads=1`, 这表示不要使用任何并行机制

## 显式函数输出

默认情况下, 当测试通过时, Rust 的测试库会截获打印到标准输出的所有内容.

`--show-output`: 表示打印成功测试的标准输出

* 如`cargo test -- --show-output`

`--nocapture`:  表示不要截获所有测试的stdout/stderr, 也就是说可以直接输出

* 如`cargo test -- --nocapture`

## 运行部分测试

`cargo new NAME`

* 运行包含`NAME`字符串的测试函数

在代码中使用 `ignore` 属性来标记测试函数

* 通常使用`ignore`属性标记耗时的测试

* 默认情况下, 测试运行器不会运行有`ignore`属性的测试函数
* 如果只希望运行被忽略的测试, 可以给测试运行器传递 `--ignored`标记
  
* 如`cargo test -- --ignored`
  
* 例子

  ```rust
  #[test]
  fn it_works() {
      assert_eq!(2 + 2, 4);
  }
  
  #[test]
  #[ignore]
  fn expensive_test() {
      // code that takes an hour to run
  }
  ```

# 3.测试的组织结构

>  测试是一个复杂的概念, 而且不同的开发者也采用不同的术语和组织结构

Rust社区从两个主要分类来考虑测试

* 单元测试(**unit test**)
  * 单元测试更小而更集中
  * 单元测试在隔离的环境中测试模组 并且每次只测试一个模组
  * 可以测试私有接口
* 集成测试(**integration test**)
  * 对于你的库或代码来说, 集成测试是外部代码; 以外部代码的方式进行测试
  * 只能测试公有接口, 而且每次测试可能会测试多个模组

## 3.1单元测试

单元测试的作用

* 在与其他部分隔离的环境下测试代码的每一个单元, 从而快速准确地确定代码中符合预期的部分和不符合预期的部分

* 单元测试在要测试的代码单元所在的文件中
* 通常是, 在文件中创建一个`tests`模组, 带有`#[cfg(test)]`注解, 其中包含测试函数

### 测试模组和`#[cfg(test)]`

`#[cfg(test)]`注解: 

* 作用: 使得 Rust 只在执行 `cargo test` 时才编译和运行该测试代码, 而在运行 `cargo build` 时不这么做.
  * 这样的话, 在只希望构建库的时候可以节省编译时间, 并且减少所产生文件的大小

`cfg`属性

* `cfg`是*configuration* 的缩写
* `cfg`属性使得其后的item只应该被包含进给定的配置选项中
  * `cfg(test)`中的`test`就是一个配置选项

### 测试私有函数

例子

```rust
pub fn add_two(a: i32) -> i32 {
    internal_adder(a, 2)
}

fn internal_adder(a: i32, b: i32) -> i32 {
    a + b
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn internal() {
        assert_eq!(4, internal_adder(2, 2));
    }
}
```

* `internal_adder`是一个私有函数

## 3.2集成测试

集成测试

* 作用: 测试库的多个部分能否一起正常工作

* 为了创建集成测试, 需要先创建一个*tests*目录

### `tests`目录

在项目根目录创建一个 *tests* 目录, 与 *src* 同级.

* *tests*目录中可以随意创建任意多的测试文件, Cargo 会将每一个文件当作单独的crate来编译

* 每个集成测试文件都是完全独立的 crate，所以需要用`use`将库引入到作用域中

* 在集成测试文件中, 用`#[test]`注解测试函数

例子: 在*adder*包的*tests/integration_test.rs*文件中

```rust
use adder;

#[test]
fn it_adds_two() {
    assert_eq!(4, adder::add_two(2));
}
```

### 集成测试中的子模组

在*tests*目录中像创建一个能被其他多个集成测试文件调用的文件/子模组

* Rust有一个命名规范:
  * 在*tests*目录中创建一个子目录, 并在目录中创建一个*mod.rs*文件; 该子目录被当作一个非测试的模组
* *tests* 目录中的子目录不会被作为单独的 crate 编译或作为一个测试结果部分出现在测试输出中

### binary crate与集成测试

如果项目是binary crate, 而且只包含*src/main.rs*而没有*src/lib.rs*,  那么不可能在 *tests* 目录创建集成测试并使用 `use` 将 *src/main.rs* 中定义的函数引入到作用域中

* 只有library crate的函数才能被引入

建议: 在binary crate项目中使用这样的代码结构: *src/main.rs*文件调用*src/lib.rs*中的逻辑, 而且*src/lib.rs*`包含所有的主要功能

* 通过这种结构, 可以使用集成测试来测试library crate中的主要功能

