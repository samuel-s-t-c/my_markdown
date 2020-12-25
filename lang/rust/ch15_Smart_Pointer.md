# 总结

指针(这里是指一个通用概念, 而不是类型), 一个包含内存地址的变量, 而地址指向其他数据

* Rust中最常见的指针: 引用, 以`&`符号为标记, 没有任何额外的开销
* 智能指针, 一种行为像指针的数据结构
  * 属于智能指针的类型
    * `String`, `Vec<T>`
    * `Box<T>`: 用于在堆上分配值
    * `Rc<T>`: 允许多个拥有者的引用计数类型
    * `Ref<T>`和`RefMut<T>`: 一种在运行时而不是编译时指向借用规则的类型, 通过`RefCell<T>`访问  
  * 特征: `Deref`和`Drop`

* 内部可变性(*interior mutability*)模式: 一个不可变的类型有一个能改变其内部值的API
* 引用循环(*reference cycle*)如何泄漏内存, 以及如何避免发生引用循环

[toc]

# 1.`Box<T>`类型

`Box<T>`类型

* 一个指向储存在堆上的数据的指针; 除此之外, 没有其他特殊的功能

* 方法: 
  * `new`
* 特征
  * `Deref`: 使其拥有引用的行为; 即当`Box<T>`离开作用域, 释放其所占用的资源, 包括堆.

## 利用`Box<T>`创建递归类型

递归类型(*recursive type*), 是一种无法在编译时知道大小的类型

* 其值的一部分是相同类型的另一个值, 因此理论上可以无限嵌套

cons list, 一个函数式编程语言中的常见类型, 是一种递归类型

* 来源于 Lisp 编程语言及其方言的数据结构
* 在 Lisp 中, `cons` 函数(*construct function*的缩写)用两个参数来构造一个新的pair
  * 这两个形参通常是一个单独的值和另一个pair, 而包含pair的pair形成了一个列表
* cons list中用`Nil`值以表示终止

## 计算枚举类型的大小

Rust检查每个变体以找到需要最多空间的变体

# 2.`Deref`特征

`Deref`特征, 路径为`std::ops::Deref`

* 重载不可变引用的解引用运算符`*`

`DerefMut`特征, 路径为`std::ops::DerefMut`

* 重载了可变引用的解引用运算符`*`

## 2.1解引用运算符

普通引用, 是指针的一种类型

解引用运算符, 可以得到指针中的值

## 2.2实现`Deref`特征

实现`Deref`特征的要求

* `type Target: ?Sized`

* `fn deref(&self) -> &Self::Target`

实现`Deref`特征的类型使用`*`运算符时, 实际上是调用`deref`方法然后对返回的引用解引用

* 假设`y`是一个实现`Deref`特征的类型变量, 则有`*y`代码时, Rust实际在底层运行`*(y.deref())`代码

## 2.3deref coercion隐式转换

在传递实参时, 实参是某个实现`Deref`特征的类型的引用, 但形参与实参的类型不相同, 自动发生*deref coercion*

* 此时Rust自动在实参上调用一系列的`deref`方法, 直到与形参类型匹配

deref coercion的几种情况

* 当`T: Deref<Target=U>`时从`&T`转换为`&U`
* 当`T: DerefMut<Target=U>`时从`&mut T`转换为`&mut U`
* 当`T: Deref<Target=U>`时从`&mut T`转换为`&U`

# 3.`Drop`特征

`Drop`特征, 路径为`std::ops::Drop`, 已在prelude中

* 用于定义某个值离开作用域时的行为

实现要求:

* `fn drop(&mut self)`: 值离开作用域时隐式调用该方法, 而且在任何时候都不能显式调用

## `std::mem::drop`

如果想要在作用域中提早丢弃某个值, Rust禁止显式地调用`drop`; 此时, 可以使用`std::mem::drop`函数

* `std::mem::drop`函数已在prelude中
* 用例 : `drop(VALUE)`

# 4.`Rc<T>`类型

`Rc<T>`类型, 路径为`std::rc::Rc`

* `Rc<T>`是引用计算类型, 即允许多个拥有者

* 仅用于单线程情况
* 只允许不可变引用
* 方法
  * `new`方法: 接受一个值, 返回一个实例
  * `Rc::clone`函数: 接受一个`Rc`引用, 返回一个共享同一个值的`Rc`

# 5.`RefCell<T>`与内部可变性模式

内部可变性模式, 是一种Rust的设计模式, 以允许改变不可变引用中的数据

* 因为借用规则, 这通常是不允许的

* 为了改变数据, 该模式使用`unsafe`代码以忽视Rust正常的可变形和借用规则
  * 这些`unsafe`代码被封装在安全的API中, 而且外部类型仍然是不可变的
* 当可以确保代码在运行时会遵守借用规则, 即使编译器不能保证的情况, 可以选择使用那些运用内部可变性模式的类型

## 5.1`RefCell<T>`: 在运行时检查借用规则

`RefCell<T>`类型, 表示数据的唯一拥有者

* 仅用于单线程情况
* `RefCell<T>`在运行时检查借用规则, 具体是在运行时违反借用规则时程序panic
  * 不在编译时检查借用规则
* 常用场景: 你确信代码遵守借用规则, 而编译器不能理解代码并保证代码是安全的

* 方法
  * `borrow`: 返回一个智能指针`Ref<T>`, 表示不可变的引用
  * `borrow_mut`: 返回一个智能指针`RefMut<T>`, 表示可变的引用

## 5.2内部可变性: 对不可变值的可变借用

特定情况下有用的模式:  在值的方法内部能够修改自身, 但对于其他代码而言, 值仍然是不可变的

### 内部可变性的用例: mock对象

测试替身(*test double*), 是一个通用编程概念, 表示在测试过程中替代某个类型的类型

* mock对象是测试替身中的特定类型, 用于记录测试过程中发生了什么以便可以断言操作是正确的

```rust
pub trait Messenger {
    fn send(&self, msg: &str);
}

pub struct LimitTracker<'a, T: Messenger> {
    messenger: &'a T,
    value: usize,
    max: usize,
}

impl<'a, T> LimitTracker<'a, T>
where
    T: Messenger,
{
    pub fn new(messenger: &T, max: usize) -> LimitTracker<T> {
        LimitTracker {
            messenger,
            value: 0,
            max,
        }
    }

    pub fn set_value(&mut self, value: usize) {
        self.value = value;

        let percentage_of_max = self.value as f64 / self.max as f64;

        if percentage_of_max >= 1.0 {
            self.messenger.send("Error: You are over your quota!");
        } else if percentage_of_max >= 0.9 {
            self.messenger
                .send("Urgent warning: You've used up over 90% of your quota!");
        } else if percentage_of_max >= 0.75 {
            self.messenger
                .send("Warning: You've used up over 75% of your quota!");
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use std::cell::RefCell;

    struct MockMessenger {
        sent_messages: RefCell<Vec<String>>,
    }

    impl MockMessenger {
        fn new() -> MockMessenger {
            MockMessenger {
                sent_messages: RefCell::new(vec![]),
            }
        }
    }

    impl Messenger for MockMessenger {
        fn send(&self, message: &str) {
            self.sent_messages.borrow_mut().push(String::from(message));
        }
    }

    #[test]
    fn it_sends_an_over_75_percent_warning_message() {
        // --snip--
        let mock_messenger = MockMessenger::new();
        let mut limit_tracker = LimitTracker::new(&mock_messenger, 100);

        limit_tracker.set_value(80);

        assert_eq!(mock_messenger.sent_messages.borrow().len(), 1);
    }
}

fn main() {}

```

# 6.引用循环

`Weak<T>`

* `upgrade`方法: 返回一个关联的`Rc<T>`

`Rc<T>`

* `downgrade`方法: 返回一个关联的`Weak<T>`