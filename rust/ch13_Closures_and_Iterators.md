# 总结

* 闭包
* 迭代器

[toc]

# 1.闭包

Rust的闭包是指能够存放在变量中的匿名函数

* 也可以作为实参传递给函数
* 与普通函数不同的是, 闭包可以捕获所在作用域中的值

定义闭包

* 形式: `|parameter_list| -> return_type {/*body*/}`

  * `parameter_list`是逗号分隔的
  * 闭包的函数体可以是一个`{}`块, 如果只有一个表达式, 可以省略花括号
  * 在合适情况下, 可以省略`-> return_type`和形参的类型注解, 让编译器推断

* 每个闭包实例都会有一个具体的独一无二的匿名闭包类型

  * 即使两个闭包的签名相同, 它们的类型也是不同的
  * 对于省略注解的闭包, 编译器将根据它的实参和返回值来推断出一个具体的闭包类型

* 例子: 

  ```rust
  let expensive_closure = |num| {
      println!("calculating slowly...");
      thread::sleep(Duration::from_secs(2));
      num
  };
  //等价于
  let expensive_closure = |num: u32| -> u32 {
      println!("calculating slowly...");
      thread::sleep(Duration::from_secs(2));
      num
  };
  ```

  ```rust
  fn  add_one_v1   (x: u32) -> u32 { x + 1 }	//函数定义
  let add_one_v2 = |x: u32| -> u32 { x + 1 };	//完全注解的闭包定义
  let add_one_v3 = |x|             { x + 1 };	//省略类型注解的闭包定义
  let add_one_v4 = |x|               x + 1  ;	//省略类型注解和花括号的闭包定义
  ```

调用闭包

* 像调用函数那样调用闭包

* 例子

  ```rust
  let add_one_v4 = |x| x + 1  ;
  let ret = add_one_v4(2);
  ```

## 存放闭包的结构

创建一个存放闭包和其调用后的结果的结构; 且该结构体有以下行动

* 在第一次需要调用结果时才执行结构中的闭包, 并将结果存放在结构中
* 之后需要调用结果时, 返回在结构中的结果
  * 这种模式, 被称为*momeization*或*lazy evaluation*

由于每个闭包都有自己的类型, 所以定义使用闭包的结构, 枚举或函数时需要使用泛型和特征绑定

* 所有闭包至少实现了以下特征中的一个: (函数也是如此)
  * `Fn`, `FnMut`, `FnOnce`

例子

```rust
struct Cacher<T>
    where T: Fn(u32) -> u32
{
    calculation: T,
    value: Option<u32>,
}
impl<T> Cacher<T>
    where T: Fn(u32) -> u32
{
    fn new(calculation: T) -> Cacher<T> {
        Cacher {
            calculation,
            value: None,
        }
    }

    fn value(&mut self, arg: u32) -> u32 {
        match self.value {
            Some(v) => v,
            None => {
                let v = (self.calculation)(arg);
                self.value = Some(v);
                v
            },
        }
    }
}
```

* `T`类型是任意实现`Fn(u32) -> u32`特征的类型
  * `Fn(u32) -> u32`, 表示接受一个`u32`且返回一个`u32`

## 闭包会捕获其环境

闭包可以捕获所在的环境, 并访问其定义所在的作用域中的变量; 当闭包从环境中捕获一个值时, 使用额外的内存以存放该值

```rust
fn main() {
    let x = 4;

    let equal_to_x = |z| z == x;

    let y = 4;

    assert!(equal_to_x(y));
}
```

* 例子中的闭包捕获了作用域中的变量`x`

闭包可以通过三种方式捕获其环境, 与函数的三种获取参数的方式一样: 获取所有权, 可变借用和不可变借用. 这三种方式被编码为三种`Fn`特征

* `FnOnce`占用捕获的变量; 在定义闭包时, 闭包必须获取其拥有权并将其移动进闭包
  * 由于不能多次获取相同变量的拥有权, 该类闭包只能被调用一次
* `FnMut`可以改变其环境, 因为可变借用
* `Fn`从环境中不可变借用某些值

在闭包定义前添加`move`关键字

* 显式要求闭包占用所有使用的变量

在需要指定一个`Fn`特征的时候, 经常可以从指定`Fn`开始, 而编译器会根据闭包体中的情况告诉你是否需要`FnMut`或 `FnOnce`.

# 2.迭代器

Rust的迭代器是惰性(*lazy*)的

* 也就是说, 在调用方法以使用迭代器之前迭代器其实并没有做任何事情

## `Iterator`特征

`Iterator`特征, 定义在标准库

* 所有迭代器都实现了`Iterator`特征

* 定义

  ```rust
  pub trait Iterator {
      type Item;
  
      fn next(&mut self) -> Option<Self::Item>;
  
      // methods with default implementations elided
  }
  ```

  * 实现该特征时必须为`next`方法和`Item`关联类型提高定义

`next`方法会消耗迭代器

* 消耗迭代器, 是指改变迭代器中用于记录序列中的元素位置的内部状态

返回迭代器的方法

* `iter`: 返回的迭代器不能改变指向的元素
* `into_iter`: 返回的迭代器占有了整个集合
* `iter_mut`: 返回的迭代器能改变指向的元素

## `Iterator`特征定义的方法

在`Iterator`特征中有很多不同的由标准库提高默认实现的方法; 其中, 

* 有些是调用`next`的方法, 被称为消耗适配器

* 有些是会改变迭代器类型的方法, 被称为迭代器适配器
  * 这些方法返回一个迭代器