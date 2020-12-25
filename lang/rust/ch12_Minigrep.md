# 总结

项目实例*Minigrep*

* 实现一个类似*grep*的命令行工具

[toc]

# 1.接受命令行参数

创建一个项目, 名为`Minigrep`

```shell
$ cargo new minigrep
     Created binary (application) `minigrep` project
$ cd minigrep
```

## 1.1读取参数值

文件*src/main.rs*

```rust
use std::env;

fn main() {
    let args: Vec<String> = env::args().collect();
    println!("{:?}", args);
}
```

* `std::env::args`函数, 只接受有效Unicode字符的参数, 返回一个指向给定命令行参数的迭代器
  * `std::env::args_os`函数, 可接受包含无效Unicode字符的参数
* 迭代器的`collect`方法, 将迭代器转换成一个集合
* `:?`, 调试格式符

## 1.2在变量中保存参数值

文件*src/main.rs*

```rust
use std::env;

fn main() {
    let args: Vec<String> = env::args().collect();

    let query = &args[1];
    let filename = &args[2];

    println!("Searching for {}", query);
    println!("In file {}", filename);
}
```

# 2.读取文件

文件*src/main.rs*

```rust
use std::env;
use std::fs;

fn main() {
    // --snip--
    println!("In file {}", filename);

    let contents = fs::read_to_string(filename)
        .expect("Something went wrong reading the file");

    println!("With text:\n{}", contents);
}
```

* `std::fs::read_to_string`, 接受一个表示文件路径的参数; 打开文件, 返回表示文件内容的`Result<String>`

# 3.重构: 模组化和错误处理

* `main`程序尽量简洁
* 最好将配置变量放入一个结构体中
* 改善错误处理代码
* 将错误处理代码放在一起

## 3.1二进制项目的关注点分离Separation of Concerns

当`main`函数逐渐臃肿时, 维护将变得不方便; 将这个复杂系统分解成多个独立的区域, 每个区域负责一个独立的关注点, 这就是关注点分离(*SoC*)

Rust社区开发一个过程, 作为关注点分离的指导

* 将程序分成*main.rs*和*lib.rs*, 并把程序的逻辑移到*lib.rs*
* 命令行解析逻辑比较小时, 它可以保留在*main.rs*中
* 当命令行解析逻辑开始变得复杂, 将其移到*lib.rs*中
* `main`程序应该保留的责任时
  * 使用命令行参数来调用命令行解析逻辑
  * 设置任何其他配置
  * 调用*lib.rs* 中的`run`程序
  * 处理`run`可能返回的错误

总结是: 

* *main.rs* 处理程序运行, 而 *lib.rs* 处理所有其他的任务逻辑
* 而且将所有的程序逻辑移动到 *lib.rs* 的函数中, 这使得我们可以对其进行集成测试
* 保留在 *main.rs* 中的代码将足够小, 通过阅读就可以验证其正确性

### 提取参数解析器

将解析参数的功能提取到一个函数中

文件*src/main.rs*

```rust
fn main() {
    let args: Vec<String> = env::args().collect();

    let (query, filename) = parse_config(&args);

    // --snip--
}

fn parse_config(args: &[String]) -> (&str, &str) {
    let query = &args[1];
    let filename = &args[2];

    (query, filename)
}
```

### 合并配置值

用一个结构体`config`来表示配置值

文件*src/main.rs*

```rust
fn main() {
    let args: Vec<String> = env::args().collect();

    let config = parse_config(&args);

    println!("Searching for {}", config.query);
    println!("In file {}", config.filename);

    let contents = fs::read_to_string(config.filename)
        .expect("Something went wrong reading the file");

    // --snip--
}

struct Config {
    query: String,
    filename: String,
}

fn parse_config(args: &[String]) -> Config {
    let query = args[1].clone();
    let filename = args[2].clone();

    Config { query, filename }
}
```

### 创建`Config`的构造函数

此时, `parse_config`函数相当于`Config`的构造函数; 于是将其变成`Config`的`new`方法

文件*src/main.rs*

```rust
fn main() {
    let args: Vec<String> = env::args().collect();

    let config = Config::new(&args);

    // --snip--
}

// --snip--

impl Config {
    fn new(args: &[String]) -> Config {
        let query = args[1].clone();
        let filename = args[2].clone();

        Config { query, filename }
    }
}
```

## 3.2修复错误处理逻辑

如果输错, 或输少了命令行参数, 则会出现错误

### `panic`退出

文件*src/main.rs*

```rust
// --snip--
fn new(args: &[String]) -> Config {
    if args.len() < 3 {
        panic!("not enough arguments");
    }
    // --snip--
```

### 返回`Result`: 更好的方式

文件*src/main.rs*

```rust
use std::process;

fn main() {
    let args: Vec<String> = env::args().collect();

    let config = Config::new(&args).unwrap_or_else(|err| {
        println!("Problem parsing arguments: {}", err);
        process::exit(1);
    });
    // --snip--
}

impl Config {
    fn new(args: &[String]) -> Result<Config, &'static str> {
        if args.len() < 3 {
            return Err("not enough arguments");
        }

        let query = args[1].clone();
        let filename = args[2].clone();

        Ok(Config { query, filename })
    }
}
```

* `Result`的`unwrap_or_else`方法, 接受一个闭包
  * 如果`Ok`, 则返回`Ok`所包含的值
  * 如果`Err`, 则执行给定的闭包
* `process::exit`函数: 立即停止程序, 并返回一个给定的数字
  * 该数字将作为退出状态码

## 3.3从`main`提取逻辑

除了设置配置和错误处理的逻辑外,  将当前`main`中的其他所有逻辑都放入`run`函数中;

* 这样的话, `main`函数将变得简洁, 且易于检查

文件*src/main.rs*

```rust
fn main() {
    // --snip--

    println!("Searching for {}", config.query);
    println!("In file {}", config.filename);

    run(config);
}

fn run(config: Config) {
    let contents = fs::read_to_string(config.filename)
        .expect("Something went wrong reading the file");

    println!("With text:\n{}", contents);
}

// --snip--
```

### `run`函数返回错误

为了改善错误处理能力, `run`函数将返回`Result`

文件*src/main.rs*

```rust
use std::error::Error;

// --snip--

fn run(config: Config) -> Result<(), Box<dyn Error>> {
    let contents = fs::read_to_string(config.filename)?;

    println!("With text:\n{}", contents);

    Ok(())
}
```

* 特征对象`Box<dyn Error>`: 表示一个有`Error`特征的类型
  * `dyn`, 是*dynamic*的缩写
  * 这使得我们能够在不同的错误场景下返回不同类型的错误
* 注意`fs::read_to_string`方法后面的`?`运算符
* `Ok`包含值的类型是`()`, 这是一种惯用方法, 表示调用`run`是为了它的副作用

### 在`main`中处理`run`返回的错误

文件*src/main.rs*

```rust
fn main() {
    // --snip--

    println!("Searching for {}", config.query);
    println!("In file {}", config.filename);

    if let Err(e) = run(config) {
        println!("Application error: {}", e);

        process::exit(1);
    }
}
```

* 使用`if let`

## 3.4将代码放进library crate

除了`main`函数, 其余的逻辑都放入*src/lib.rs*中

文件*src/lib.rs*

```rust
use std::error::Error;
use std::fs;

pub struct Config {
    pub query: String,
    pub filename: String,
}

impl Config {
    pub fn new(args: &[String]) -> Result<Config, &'static str> {
        // --snip--
    }
}

pub fn run(config: Config) -> Result<(), Box<dyn Error>> {
    // --snip--
}
```

* 使用`pub`, 以定义公有API

文件*src/main.rs*

```rust
use std::env;
use std::process;

use minigrep::Config;

fn main() {
    // --snip--
    if let Err(e) = minigrep::run(config) {
        // --snip--
    }
}
```

# 4.测试驱动开发

测试驱动开发(*Test-driven development, TDD*): 一种编写软件

1. 编写一个会失败的测试, 并运行该测试, 以确保失败的原因是你所期望的
2. 编写或修改代码, 使得该测试通过
3. 在确保测试通过的前提下重构新增的代码
4. 重复以上步骤

使用测试驱动实现这个功能(`search`函数: 

* 在文件内容中搜索给定的查询字符串, 并返回匹配的文本行

## 4.1编写一失败的测试

文件*src/lib.rs*

```rust
pub fn search<'a>(query: &str, contents: &'a str) -> Vec<&'a str> {
    vec![]
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn one_result() {
        let query = "duct";
        let contents = "\
Rust:
safe, fast, productive.
Pick three.";

        assert_eq!(
            vec!["safe, fast, productive."],
            search(query, contents)
        );
    }
}
```

* `search`的`'a`寿命

## 4.2编写代码以通过测试

实现`search`的真正功能: 在文件内容中搜索给定的查询字符串, 并返回匹配的文本行

具体步骤是:

1. 遍历内容的每一行
2. 在每次遍历中, 检查该行是否包含查询字符串
   1. 如果有, 则将该行放入一个要返回的列表中
3. 返回该列表



文件*src/lib.rs

```rust
pub fn search<'a>(query: &str, contents: &'a str) -> Vec<&'a str> {
    let mut results = Vec::new();

    for line in contents.lines() {
        if line.contains(query) {
            results.push(line);
        }
    }

    results
}
```



## 4.3重构代码并

因为之前的SoC做得不错, 不用进行重构代码;在`run`函数中使用`search`函数

文件*src/lib.rs*

```rust
pub fn run(config: Config) -> Result<(), Box<dyn Error>> {
    let contents = fs::read_to_string(config.filename)?;

    for line in search(&config.query, &contents) {
        println!("{}", line);
    }

    Ok(())
}
```

# 5.处理环境变量

实现一个功能: 用户可以通过设置环境变量来设置搜索是否大小写敏感

* 设置环境变量时调用`search_case_insensitive`函数, 否则调用`search`函数

TDD

*  失败测试: *src/lib.rs*

  ```rust
  #[cfg(test)]
  mod tests {
      use super::*;
  
      #[test]
      fn case_sensitive() {
          let query = "duct";
          let contents = "\
  Rust:
  safe, fast, productive.
  Pick three.
  Duct tape.";
  
          assert_eq!(
              vec!["safe, fast, productive."],
              search(query, contents)
          );
      }
  
      #[test]
      fn case_insensitive() {
          let query = "rUsT";
          let contents = "\
  Rust:
  safe, fast, productive.
  Pick three.
  Trust me.";
  
          assert_eq!(
              vec!["Rust:", "Trust me."],
              search_case_insensitive(query, contents)
          );
      }
  }
  ```

* 实现功能, 并通过测试: *src/lib.rs*

  ```rust
  pub fn search_case_insensitive<'a>(query: &str, contents: &'a str) -> Vec<&'a str> {
      let query = query.to_lowercase();
      let mut results = Vec::new();
  
      for line in contents.lines() {
          if line.to_lowercase().contains(&query) {
              results.push(line);
          }
      }
  
      results
  }
  
  pub struct Config {
      pub query: String,
      pub filename: String,
      pub case_sensitive: bool,
  }
  
  use std::env;
  
  impl Config {
      pub fn new(args: &[String]) -> Result<Config, &'static str> {
          if args.len() < 3 {
              return Err("not enough arguments");
          }
  
          let query = args[1].clone();
          let filename = args[2].clone();
  
          let case_sensitive = env::var("CASE_INSENSITIVE").is_err();
  
          Ok(Config { query, filename, case_sensitive })
      }
  }
  
  pub fn run(config: Config) -> Result<(), Box<dyn Error>> {
      let contents = fs::read_to_string(config.filename)?;
  
      let results = if config.case_sensitive {
          search(&config.query, &contents)
      } else {
          search_case_insensitive(&config.query, &contents)
      };
  
      for line in results {
          println!("{}", line);
      }
  
      Ok(())
  }
  
  ```

# 6.输出信息到标准错误

`eprintln!`宏打印到标准错误流