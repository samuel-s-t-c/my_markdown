# 总结

Rust标准库的集合(**collections**)

* 集合指向的数据被存放在堆中
* 集合的尺寸在编译时未知的, 可以在运行时增长或减小
* 分类
  * 向量`Vec<T>`: 值是连续存储的
  * 字符串`String`: 字符的集合; 字符是连续存储的
  * 哈希映射`HashMap<K, V>`: 数据结构`map`的特定实现; 允许将一个值关联一个特定的键

* [更多集合](https://doc.rust-lang.org/std/collections/index.html)

[toc]

# 1.向量`Vec<T>`

创建新向量

* `Vec::new()`函数
  * 例子: `let v: Vec<i32> = Vec::new();`
* `vec!`宏
  * 例子: `let v = vec![1, 2, 3];`

更新向量

* `push`方法
  * 例子: `let mut v = Vec::new(); v.push(5);`

销毁向量

* 与`struct`一样, 当一个向量离开作用域时, 它及其元素都被释放

访问元素

* 索引值是非负整数, 从0开始计数

* 两种方式
  * 使用`&`和`[]`
  * `get`方法: 返回一个`Option<&T>`

* 例子

  ```rust
  let v = vec![1, 2, 3, 4, 5];
  
  let third: &i32 = &v[2];
  println!("The third element is {}", third);
  
  match v.get(2) {
      Some(third) => println!("The third element is {}", third),
      None => println!("There is no third element."),
  }
  ```

在向量上迭代

* `for`循环

  ```rust
  let mut v = vec![100, 32, 57];
  for i in &mut v {
      *i += 50;
  }
  ```

  * 注意: 每次得到的是指向元素的迭代器; 因此需要解引用`i`以获得引用

枚举作为向量的元素类型

* 例子

  ```rust
  enum SpreadsheetCell {
      Int(i32),
      Float(f64),
      Text(String),
  }
  
  let row = vec![
      SpreadsheetCell::Int(3),
      SpreadsheetCell::Text(String::from("blue")),
      SpreadsheetCell::Float(10.12),
  ];
  ```

# 2.字符串`String`

`String`和字符串切片`str`

* `String`是可变长, 可变值, UTF-8编码的字符串类型
* `str`是UTF-8编码的字符串类型

## 创建`String`

* `String::new`函数

  * 例子: `let mut s = String::new();`

* `to_string`方法: 任何实现了`Dsiplay`特征的类型都有该方法, 包括字符串字面量

  * 例子

    ```rust
    let data = "initial contents";
    
    let s = data.to_string();
    
    // the method also works on a literal directly:
    let s = "initial contents".to_string();
    ```

* `String::from`函数: 接受一个字符串字面量
  
  * 例子: `let s = String::from("initial contents");`

## 更新`String`

* 附加
  * `push`方法: 接受一个字符
    * 例子: `let mut s = String::from("lo"); s.push('l');`
  * `push_str`方法: 接受一个字符串切片, 不会占有实参
    * 例子: `let mut s = String::from("foo"); s.push_str("bar");`

* 并接
  * `+`运算符/`add`方法: `fn add(self, s: &str) -> String`

    * `+`运算符的第一个操作数必须是`String`值; 第二个操作数必须是引用, 可以接受`&String`, 原因是`coerce`和`deref coerce`
    * `add`方法本身是一个泛型方式; 上面的签名是类型形参为`String`的情况

    ```rust
    let s1 = String::from("Hello, ");
    let s2 = String::from("world!");
    let s3 = s1 + &s2; // note s1 has been moved here and can no longer be used
    ```

  * `format!`宏: 

    ```rust
    let s1 = String::from("tic");
    let s2 = String::from("tac");
    let s3 = String::from("toe");
    
    let s = format!("{}-{}-{}", s1, s2, s3);
    ```

## `String`不支持`[]`索引操作

`String`不支持索引的原因: 

* 如果按字节来进行索引, `String`的索引值并不是总能与一个有效的Unicode标量值对应
  * `String`其实是对`Vec<u8>`的包装; 
  * UTF-8字符所占的字节可能大于1个字节
* 索引操作通常被认为使用常数时间`O(1)`; 如果按字素簇来进行索引, 无法保证性能

Rust的`String`可以有三种表现形式

* 字节
* 标量值
* 字素簇(**grapheme clusters**)
* 例子: 用梵文书写的印度语单词 `नमस्ते`
  * 字节: `[224, 164, 168, 224, 164, 174, 224, 164, 184, 224, 165, 141, 224, 164, 164, 224, 165, 135]`
  * Unicode标量值: `['न', 'म', 'स', '्', 'त', 'े']`
  * 字素簇: `["न", "म", "स्", "ते"]`

`String`的切片

* 注意: 索引值必须能处于字符的边界, 不能处于多字节字符的内部, 不然发生`panic`

* 例子: `let hello = "Здравствуйте"; let s = &hello[0..4];`

在`String`上进行迭代: 

* 为每个Unicode标量值进行迭代: `chars`方法返回指向标量值的迭代器

```rust
for c in "नमस्ते".chars() {
    println!("{}", c);
}

//结果
न
म
स
्
त
े
```

* 为每个字节进行迭代: `bytes`方法返回指向原始字节的迭代器

```rust
for b in "नमस्ते".bytes() {
    println!("{}", b);
}
//结果
224
164
// --snip--
165
135
```

# 3.`HashMap<K, V>`

`HashMap<K, V>`类型, 存储一种映射: `K`类型的键->`V`类型的值

* 映射是通过哈希函数来实现的
* `HashMap<K, V>`没有被prelude自动引入
* 路径: `std::collections::HashMap`

## 创建一个`HashMap`

* `HashMap::new()`函数

  ```rust
  use std::collections::HashMap;
  
  let mut scores = HashMap::new();	//为什么不需要类型注解?
  
  scores.insert(String::from("Blue"), 10);
  scores.insert(String::from("Yellow"), 50);
  ```

* 在元组向量(*a vector of tuples*)上使用`collect`方法: 

  * 需要类型注解`HashMap<_, _>`, 以指定`collect`方法返回的类型;
  * 使用`_`的原因是Rust可以推断出`HashMap`所包含的类型

  ```rust
  use std::collections::HashMap;
  
  let teams  = vec![String::from("Blue"), String::from("Yellow")];
  let initial_scores = vec![10, 50];
  
  let scores: HashMap<_, _> = teams.iter().zip(initial_scores.iter()).collect();
  ```

  * 例子中使用`zip`方法

## 元素的拥有权

* 对于实现`Copy`特征的类型, 其值被拷贝进`HashMap`, 如`i32`

* 对于被拥有的值(*owned values*), 它被移动进`HashMap`, 如`String`

* 如果元素是引用类型, 则要保证被引用的值必须在`HashMap`有效时也保持有效

  ```rust
  use std::collections::HashMap;
  
  let field_name = String::from("Favorite color");
  let field_value = String::from("Blue");
  
  let mut map = HashMap::new();
  map.insert(field_name, field_value);
  // field_name and field_value are invalid at this point, try using them and
  // see what compiler error you get!
  ```

## 访问`HashMap`中的值

* `get`方法: 接受一个键, 返回一个`Option<&V>`

  ```rust
  use std::collections::HashMap;
  
  let mut scores = HashMap::new();
  
  scores.insert(String::from("Blue"), 10);
  scores.insert(String::from("Yellow"), 50);
  
  let team_name = String::from("Blue");
  let score = scores.get(&team_name);
  ```

* 使用`for`迭代: 注意每次迭代一个键值对

  ```rust
  use std::collections::HashMap;
  
  let mut scores = HashMap::new();
  
  scores.insert(String::from("Blue"), 10);
  scores.insert(String::from("Yellow"), 50);
  
  for (key, value) in &scores {
      println!("{}: {}", key, value);
  }
  ```

## 更新`HashMap`

* 注意: 在任何时刻, 一个键只能关联一个值

* `insert`方法: 接受一个键和一个值

  * 给定的键存在时将其旧值覆盖
  * 给定的键不存在时插入新的键值对

  ```rust
  use std::collections::HashMap;
  
  let mut scores = HashMap::new();
  
  scores.insert(String::from("Blue"), 10);
  scores.insert(String::from("Blue"), 25);
  
  println!("{:?}", scores);
  ```

* `entry`方法: 检查给定的键是否存在; 接受一个键, 返回一个`Entry`枚举类

  * `Entry`的`or_insert`方法: 接受一个值
  * 如果给定的键存在则返回一个指向值的可变引用
    * 如果给定的键不存在则插入给定的键值对, 然后返回一个指向值的可变引用
  
  ```rust
  use std::collections::HashMap;
  
  let mut scores = HashMap::new();
  scores.insert(String::from("Blue"), 10);
  
  scores.entry(String::from("Yellow")).or_insert(50);
  scores.entry(String::from("Blue")).or_insert(50);
  
println!("{:?}", scores);
  ```

## 哈希函数

默认情况下, `HashMap`使用一个加密性强的哈希函数, 以抵抗DoS攻击

* 该哈希函数不是最快的哈希算法, 但是为了更好的安全性可以牺牲一些性能

可以显式地指定其他的`hasher`

* `hasher`是实现了`BuildHasher`特征的类型