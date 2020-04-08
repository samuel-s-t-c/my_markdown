# 总结

关联容器

* `map`, `set`, 有序, 无序, 唯一键, 重复键
* 关联容器特有的操作, 迭代器

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

* 键的类型必须定义了元素比较的方式; 
  * 默认情况下, 标准库使用`<`运算符比较元素
* 如果键的类型没有定义元素比较的方式, 必须在容器定义中显式地提供自定义的操作



可以提供自定义的操作代替`<`运算符进行比较操作: 

* 如果要提供自定义操作, 必须在容器类型中提供某种操作的类型; 具体是容器类型名的尖括号中元素类型后跟着操作的类型

  * 此时, 自定义操作的类型也是该容器类型的一部分
  
* 然后可以提供自定义操作为构造函数的实参; 该操作的类型应该与类型名内的操作类型一致

* 例子: `map<key_type, value_type, comparison_type*> obj(&comparison);` , 其中`comparison`的类型是`comparison_type`

  ```c++
  bool compareIsbn(const Sales_data &lhs, const Sales_data &rhs)
  {
      return lhs.isbn() < rhs.isbn();
  }
  multiset<Sales_data, decltype(compareIsbn)*> bookstore(compareIsbn);
  ```

  

所提供自定义的操作必须在键类型上定义了一个严格弱序(**strict weak ordering**); 也就是有以下基本性质: 

* (`k1, k2`是两个键, `op`是自定义的操作, 可以将`op`看作`<`进行理解) 

* `k1 op k2`与`k2 op k1`的结果不能都为`true`
* 如果`k1 op k2`和`k2 op k3`的结果都为`true`, 则`k1 op k3`的结果也是`true`
* 如果`k1 op k2`的结果与`k2 op k1`的结果都为`false`, 则`k1`和`k2`是等价的;
  * 如果`k1`和`k2`是等价的, `k2`和`k3`是等价的, 则`k1`和`k3`是等价的;
  * 当两个键是等价时, 容器将它们视为相等的. 如果用作`map`的键时, 这两个键与同一个元素关联

## 2.3`pair`类型

`pair`类型, 定义在`utility`头文件

| 操作                         | 描述                                                         |
| ---------------------------- | ------------------------------------------------------------ |
| `pair<T1, T2> p;`            | 默认初始化`pair`对象; `p`的两个类型分别为`T1`和`T2`的成员被值初始化 |
| `pair<T1, T2> p(v1, v2);`    | `p`的 第一个成员和第二个成员分别用`v1`和`v2`初始化           |
| `pair<T1, T2> p = {v1, v2};` | 同上                                                         |
| `make_pair(v1, v2)`          | 返回一个用`v1`和`v2`初始化的`pair`; `pair`的类型从`v1`和`v2`的类型推断 |
| `p.first`                    | 返回`p`的`first`数据成员; 该数据成员是`public`的             |
| `p.second`                   | 返回`p`的`second`数据成员; 该数据成员是`public`的            |
| `p1 relop p2`                | `pair`支持关系运算符; 使用字典顺序和元素的`<`运算符          |
| `p1 == p2`                   | 如果两个`pair`对象的`first`和`second`成员都相等, 则这两个`pair`... |
| `p1 != p2`                   | ...对象相等; 否则不相等. 使用元素的`==`运算符                |

* `pair`是类型模板, 需要提供两个类型名 

```c++
pair<string, size_t> word_count;
pair<string, vector<int>> line;
pair<string, string> author{"James", "Joyce"};
```

# 3.关联容器的操作

关联容器的类型别名

| 操作          | 描述                                                         |
| ------------- | ------------------------------------------------------------ |
| `key_type`    | 该容器类型的键类型                                           |
| `mapped_type` | 与键关联的类型; (只适用于`map`)                              |
| `value_type`  | 关联容器的元素类型; 对于`set`和`set`相关的关联容器类型, 与`key_type`一样; 对于`map`和`map`相关的关联容器类型, `pair<const key_type, mapped_type>` |

* `map`的元素是`pair`

## 3.1关联容器的迭代器

> 关联容器支持通用容器操作, 包括返回迭代器的函数
>
> 除了`forward_list`例外, 所有容器迭代器都支持([s3.4: 标准容器迭代器支持的操作](P1-C03-Strings_Vectors_Arrays.md))所示的操作

`set`的迭代器是`const`的

* `set`类型为迭代器定义了`iterator`和`const_iterator`类型, 但两种类型的迭代器都只允许读元素, 不能写元素. 
* 因为在`set`中的键是`const`

解引用关联容器的迭代器, 得到指向关联容器的`value_type`的引用;

* 对于`set`和`set`相关的关联容器类型, 得到指向`key_type` 的引用; 
* 对于`map`和`map`相关的关联容器类型, 得到指向`pair`的引用; 该`pair`的`first`成员是`const`键, `second`成员是值.

```c++
auto map_it = word_count.cbegin();
while (map_it != word_count.cend()) {
    cout << map_it -> first << " occurs "
         << map_it -> second << " times" << endl;
    ++map_it;
}
```

### 关联容器和泛型算法

通常不在关联容器上使用泛型算法. 

* 因为键是`const`, 意味着不能使用关联容器的迭代器修改或重排容器元素, 所以不能将关联容器迭代器传递给修改或重排容器元素的算法
* 关联容器可用于只读元素的算法; 但是, 关联容器可以通过键进行(快速)查找, 而只读算法通常进行顺序搜索.

在实际编程中, 对关联容器使用算法时, 要么是将它当作源序列, 要么当作目的位置

## 3.2添加元素

* 因为`map`和`set`的键是唯一的, 向`map`和`set`中插入已存在的元素对容器没有任何影响

| 操作                 | 描述                                                         |
| -------------------- | ------------------------------------------------------------ |
| `c.insert(v)`        | 向`c`中插入`v`, `v`是`valut_type`对象;                       |
| `c.emplace(args)`    | 向`c`中构造一个由实参`args`指明的元素                        |
| `c.insert(b, e)`     | 向`c`中插入`b`和`e`表示的范围内的元素                        |
| `c.insert(il)`       | 向`c`中插入花括号列表`il`中的元素                            |
| `c.insert(p, v)`     | `p`是一个迭代器; 从`p`指向的位置开始搜索新元素应该存储的位置 |
| `c.emplace(p, args)` | 同上                                                         |

具体说明

`c.insert(v)`, `c.insert(args)`

* `v`是一个`value_type`对象, `args`用于构造一个元素
* 对于`map`和`set`, 当`c`中没有给定的键时, 插入该元素. 返回一个`pair`对象
  * `first`成员是一个指向该元素的迭代器
  *  `second`成员是一个指示插入是否成功的`bool`值; 如果`c`有给定的元素, 该值为`false`, 否则为`true`
* 对于`multimap`和`multiset`, 直接插入(或构造)给定的元素, 返回一个指向该元素的迭代器

`c.insert(b, e)`, `c.insert(il)`

* `b`和`e`是一对迭代器, 表示一个`c::value_typ`类型值的范围; `il`是`c::value_type`值的花括号列表
* 对于`map`和`set`, 只插入键不在`c`中的元素. 返回`void`
* 对于`multimap`和`multiset`, 插入所有元素. 返回`void`

`c.insert(p, v)`, `c.emplace(p, args)`

* `p`是一个迭代器, 指出从哪里开始搜索新元素存储的位置
* 类似`insert(v)`, `emplace(args)`;

注意: 

* `map`的元素类型是`pair`类型

```c++
map<string, size_t> m;
m.insert({word, 1});
m.insert(make_pair(word, 1));
m.insert(pair<string, size_t>(word, 1));
m.insert(map<string, size_t>::value_type(word, 1));
multimap<string, string> authors;
authors.insert({"Barth, John", "Sot-Weed Factor"});
authors.insert({"Barth, John", "Lost in the Funhouse"});
```

```c++
map<string, size_t> word_count;
string word;
wihile (cin >> word) {
    //ret的类型是pair<map<string, size_t>::iterator, bool>
    auto ret = word_count.insert({word, 1});
    if (!ret.second)
        ++ret.first->second;
}
```

## 3.3删除元素

| 操作            | 描述                                                         |
| --------------- | ------------------------------------------------------------ |
| `c.erase(k)`    | 从`c`中删除键为`k`的所有元素; 返回一个`size_type`值, 表示删除了多少元素 |
| `c.erase(p)`    | 从`c`中删除迭代器`p`指向的元素; 返回一个指向被删元素后面位置的迭代, 或者`c.end()`如果`p`指向尾元素的话 |
| `c.erase(b, e)` | 删除`b`和`e`所示范围内的所有元素; 返回`e`                    |

具体说明

`c.erase(p)`

* `p`必须指向`c`中的某个元素, 不能与`c.end()`相同

注意: 在C++之前的版本, 这些函数返回`void`

## 3.4`map`的下标操作*

`map`和`unordered_map`提供了下标运算符和一个`at`成员函数

| 操作      | 描述                                                         |
| --------- | ------------------------------------------------------------ |
| `c[k]`    | 返回键`k`对应的`mapped_type`对象; 如果键`k`不在`c`中, 添加一个值初始化的键为`k`的新元素 |
| `c.at(k)` | 访问键`k`对应的`mapped_type`对象, 带参数检查; 如果键`k`不在`c`中, 抛出`out_of_range`异常 |

* 下标运算符只适用于非`const`的`map`或`unordered_map`; 因为下标运算符会插入新元素

注意:

* 如果只是想知道一个元素是否在`map`中, 且该元素不存在时不想添加元素, 不应该使用下标操作

* `map`的下标运算符返回对应的`mapped_type`对象, 而不是`value_type`对象
  * 也可以说, 返回键值对的值, 而不是键值对
  * 通常情况下, 解引用一个迭代器返回的类型与下标运算符返回的类型是一样的; 但`map`是例外

```c++
map<string, size_t> word_count;
word_count["Anna"] = 1;
```

1. 在`word_count`查找键为`"Anna"`的元素; 结果是没有
2. 向`word_count`中插入一个新键值对;  该`pair`的键是一个保存`"Anna"`的`string`对象, 值被值初始化为0
3. 得到新插入的元素, 并将值`1`赋予它

## 3.5访问元素

| 操作               | 描述                                                         |
| ------------------ | ------------------------------------------------------------ |
| `c.find(k)`        | 返回一个指向第一个键为`k`的元素的迭代器, 或者尾后迭代器如果`k`不在容器中 |
| `c.count(k)`       | 返回键为`k`的元素的数目                                      |
| `c.lower_bound(k)` | 返回一个指向第一个键不小于`k`的元素的迭代器                  |
| `c.upper_bound(k)` | 返回一个指向第一个键大于`k`的元素的迭代器                    |
| `c.equal_range(k)` | 返回一个`pair`对象, 包含一对迭代器; 迭代器所表示的范围内的元素是键为`k`的元素; 如果`k`不在容器中, 两个迭代器都是尾后迭代器 |

* `lower_bound`和`upper_bound`不适用于无序容器

当`mutlimap`和`multiset`有多个键相同的元素, 标准准保证这些元素在容器中是相邻的

* 使用`find`和`count`

```c++
string search_item("Alain de Botton");
auto entries = authors.count(search_item);
auto iter = authors.find(search_item);
while (entries) {
    cout << iter->second << endl;
    ++iter;
    --entries;
}
```

* 使用`lower_bound`和`upper_bound`

```c++
for (auto beg = authors.lower_bound(search_item),
    	  end = authors.upper_bound(search_item);
     beg != end; ++beg)
    cout << beg->second << endl;
```

* 使用`equal_rang`

```c++
for (auto pos = authors.equal_range(search_item);
     pos.first != pos.second; ++pos.first)
    cout << pos.first->second << ednl;
```

## 3.6例子: 单词转换(p440)

# 4.无序容器 (C++11)

C++11定义了四种无序关联容器: `unordered_map`, `unordered_multimap`, `unordered_set`, `unordered_multiset`

* 无序容器使用哈希函数(**hash function**)和`==`运算符来组织元素
* 无序容器适合用于键之间没有明显的顺序关系的情况, 也会用于维护元素的顺序的代价非常高的情况

哈希技术理论上能获得很好的平均性能, 但实际上想要达到那样的效果需要进行一些性能测试和调优工作.

## 使用无序容器

除了哈希管理操作, 无序容器提供与有序容器一样的操作; 所以通常可以用无序容器替换对应的有序容器, 反之亦然.

* 因为无序容器的元素不是按照顺序存储, 所以使用无序容器的输出通常会与有序容器的不同

## 管理桶

无序容器的存储结构是一组桶(**bucket**), 每个桶保存零个或多个元素. 

* 容器使用哈希函数将元素映射到桶上.
  * 容器首先计算一个元素的哈希值, 该哈希值指出元素应该存储在哪个桶中.  
  * 容器将具有一个特定哈希值的所有元素都保存在相同的桶中. 如果容器允许重复键, 具有相同键的元素都在同一个桶中
* 无序容器的性能取决于哈希函数, 桶的数量和桶的大小
  * 哈希函数接受相同的实参, 总是产生相同的结果. 哈希函数可以将每个特定的值映射到唯一的桶上, 也允许将多个哈希值映射到相同的桶中.
  * 当一个桶中保存多个元素时, 需要在桶中顺序搜索元素来查找想要的元素
  * 计算某个元素的哈希值然后找到对应的桶, 这通常是很快的操作; 如果桶中保存很多元素, 那么需要大量比较操作来查找元素

| 操作                     | 描述                                                         |
| ------------------------ | ------------------------------------------------------------ |
| 桶接口                   |                                                              |
| `c.bucket_count()`       | 正在使用的桶的数目                                           |
| `c.max_bucket_count()`   | 该容器能容纳的最多的桶的数量                                 |
| `c.bucket_size(n)`       | 第`n`个桶中有多少元素                                        |
| `c.bucket(k)`            | 键`k`的元素在哪个桶                                          |
| 桶迭代                   |                                                              |
| `local_iterator`         | 指向桶中元素的迭代器类型                                     |
| `const_local_iterator`   | `const`桶迭代器                                              |
| `c.begin(n), c.end(n)`   | 桶`n`的首迭代器和尾后迭代器                                  |
| `c.cbegin(n), c.cend(n)` | 返回`const_local_iterator`的首迭代器和尾后迭代器             |
| 哈希策略                 |                                                              |
| `c.load_factor()`        | 当前每个桶的平均元素数量, 类型是`float`                      |
| `c.max_load_factor()`    | `c`试图维护的平均桶大小; `c`会在需要时添加新桶, 使得`load_factor<=max_load_factor` |
| `c.rehash(n)`            | 重组存储空间, 使得`bucket_count>=n && bucket_count > size/max_load_factor` |
| `c.reserve(n)`           | 重组存储空间; 在不进行`rehash`的前提下, `c`可以存放`n`个元素 |

## 无序容器的键类型的要求

默认情况下, 无序容器使用键类型的`==`运算符来比较元素, 还使用`hash<key_type>`类型的对象来生成哈希值

类型必须有自己的`hash`模板, 或者在定义容器时提供自定义操作

* 标准库为内置类型提供了`hash`模板, 也为了一部分库类型定义了`hash`, 如`string`, 智能指针类型

* 类类型定义`hash`模板(见s16.5 p709)

* 在定义容器时提供自定义操作: 

  * 可以提供函数来替代`==`运算符和哈希值计算函数

  ```c++
  //定义替换的函数
  size_t hasher(const Sales_data &sd)
  {
      return hash<string>()(sd.isbn());
  }
  bool eqOp(const Sales_data &lhs, const Sales_date &rhs)
  {
      return lhs.isbn() == rhs.isbn();
  }
  //提供自定义操作
  using SD_multiset = unordered_multiset<Sales_data,
  					decltype(hasher)*, decltype(eqOp)*>;
  SD_multiset bookstore(42, hasher, eqOp);
  //Foo类型必须有==运算符
  unordered_set<Foo, decltype(FooHash)*> fooSet(10, FooHash);
  ```

