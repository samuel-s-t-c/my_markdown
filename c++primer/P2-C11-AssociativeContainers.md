# 总结

关联容器

[toc]

# 1.介绍关联容器

关联容器支持高效的键查找和访问; 标准库提供了8个关联容器

| 关联容器             | 描述                        |
| -------------------- | --------------------------- |
| 元素按"key"排序      |                             |
| `map`                | 关联数组; 保存key-value对   |
| `set`                | 在该容器中, 键同时也是值    |
| `multimap`           | 键可以重复出现的`map`       |
| `multiset`           | 键可以重复出现的`set`       |
| 无序集合             |                             |
| `unordered_map`      | 用哈希函数组织的`map`       |
| `unordered_set`      | 用哈希函数组织的`set`       |
| `unordered_multimap` | 哈希`map`, 其键可以重复出现 |
| `unordered_multiset` | 哈希`set`, 其键可以重复出现 |

* 容器的不同体现在三个方面
  * 是`set`还是`map`
  * 键要唯一的, 还是可以重复的
  * 有序保存元素, 还是无序保存元素
* `map`和`multimap`定义在`map`头文件
*  `set`和`multiset`定义在`set`头文件
* `unordered_map`和`unordered_multimap`定义在`unordered_map`头文件
* `unordered_set`和`unordered_multiset`定义在`unordered_set`头文件

## 简单使用`map`

`map`是key-value对的集合, 通常被称为关联数组(**associative array**)

* 关联数组基本与内置数组一样, 除了关联数组的下标可以不是整数.
  * 也就是说, 在`map`中的值是通过键来查找的, 不是通过位置

例子: 单词统计程序

```c++
map<string, size_t> word_count;
string word;
while (cin >> word)
    ++word_count[word];
for (const auto &w : word_count)
    cout << w.first << " occurs " << w.second
    	 << ((w.second > 1) ? " times" : " time") << endl;
```

* 定义`map`需要提供键和值的类型; 这里键类型是`string`, 值类型是`size_t`
* 使用`word`作为下标访问`word_count`时, 如果`word_count`没有`word`键, 则创建一个键是`word`的新元素, 其值为0; 如果有`word`值, 返回对应的值对象的引用

# 简单使用`set`

`set`就是键的集合. 

```c++
map<string, size_t> word_count;
set<string> exclude = {"The", "But", "And", "Or", "An", "A",
                       "the", "but", "and", "or", "an", "a"};
string word;
while (cin >> word)
    if (exclude.find(word) == exclude.end())
        ++word_count[word];
```

* 定义`set`需要提供元素的类型, 或者说键的类型. 这里是`string`
* 我们可以列表初始化关联容器中的元素
* `set`容器的`find`函数查找给定的值; 如果`set`中有该值的元素, 返回指向该元素的迭代器; 如果没有, 返回尾后迭代器

# 2.关联容器的概述

关联容器支持通用容器操作(见[s9.2容器库概述](P2-C09-SequentialContainers.md))

* 关联容器还定义了一些特有的操作和类型别名; 无序关联容器提供了一个调整哈希性能的操作
* 关联容器的迭代器都是双向的

## 2.1定义关联容器*

### 定义和初始化`map`和`set`

定义`map`时, 必须指明键类型和值类型; 定义`set`时, 只需指明键类型

* 键必须是唯一的

容器的四种通用构造函数

* `C c;`
* `C c1(c2);`
* `C c(b, e);`
* `C c{a, b, d...};`

为`map`提供初始符时, 键和值的初始符都要提供, 因为在`map`中一个键值对是一个元素

* 要使用花括号将每个键值对包围起来, `{key , value}`

```c++
map<string, size_t> word_count;
//列表初始化
set<string> exclude = {"The", "But", "And", "Or", "An", "A",
                       "the", "but", "and", "or", "an", "a"};
//有三个元素
map<string, string> authors = { {"Joyce", "James"},
                                {"Austen", "Jane"},
                                {"Dickens", "Charles"}};
```

### 定义和初始化`multimap`和`multiset`

* 键可以重复出现

```c++
vector<int> ivec;
for (vector<int>::size_type i = 0; i != 10; ++i) {
    ivec.push_back(i);
    ivec.push_back(i);
}

set<int> iset(ivec.cbegin(), ivec.cend());
multiset<int> miset(ivec.cbegin(), ivec.cend());
cout << ivec.size() << endl;	//print 20
cout << iset.size() << endl;	//print 10
cout << miset.size() << endl;	//print 20
```

## 2.2键类型的要求*

有序关联容器的键类型的要求

* 键的类型必须定义了元素比较的方式; 默认情况下, 标准库使用`<`运算符比较元素

可以提供自定义的操作代替`<`运算符进行比较操作

* 如果要提供自定义操作, 必须在容器类型中提供该操作的类型; 具体是在尖括号中元素类型后跟着操作的类型

  ```c++
  
  ```

  

所提供自定义的操作必须在键类型上定义了一个严格弱序(**strict weak ordering**); 也就是有以下基本性质: 

* (`k1, k2`是两个键, `op`是自定义的操作, 可以将`op`看作`<`进行理解) 

* `k1 op k2`与`k2 op k1`的结果不能都为`true`
* 如果`k1 op k2`和`k2 op k3`的结果都为`true`, 则`k1 op k3`的结果也是`true`
* 如果`k1 op k2`的结果与`k2 op k1`的结果都为`false`, 则`k1`和`k2`是等价的;
  * 如果`k1`和`k2`是等价的, `k2`和`k3`是等价的, 则`k1`和`k3`是等价的;
  * 当两个键是等价时, 容器将它们视为相等的. 如果用作`map`的键时, 这两个键与同一个元素关联

## 2.3`pair`类型

# 3.关联容器的操作

# 4.无序容器