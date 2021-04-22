# 总结

容器

* 容器与其公用接口
  * 操作可能使迭代器, 引用和指针失效

* 顺序容器, 顺序容器的特有操作
* 个别容器的特有操作
* 容器适配器

[toc]

# 1.顺序容器
> 容器存放一组特定类型的对象.

顺序容器为程序员提供了控制元素存储和访问顺序的能力

* 顺序容器中元素存储和访问的顺序, 与元素加入容器时的位置对应, 不依赖于元素的值.
* 相比之下, 关联容器根据键值来存储元素.

顺序容器类型
|类型|描述|
|-|-|
|`vecotr`|可变大小的数组; 支持快速随机访问; 在尾部位置快速插入或删除元素, 其他位置慢|
|`deque`|双端队列; 支持快速随机访问; 在头尾位置快速插入/删除|
|`list`|双向链表; 只支持双向顺序访问; 在任何位置都可快速插入/删除|
|`forward_list`|单向链表; 只支持单向顺序访问; 在任何位置都可快速插入/删除|
|`array`|固定大小数组; 支持快速随机访问; 不可添加或删除元素|
|`string`|只保存字符的容器; 快速随机访问; 在尾部位置快速插入/删除|

* 顺序容器都支持快速顺序访问
* 顺序容器类型在以下方面有不同的取舍
    * 添加或删除元素
    * 非顺序访问
* `forward_list`和`array`是C++11标准增加的类型

# 2.容器库概览*

每种容器类型都定义在同名的头文件中; 容器均定义为模板类

容器类型的操作的层次
* 所有容器类型的可用的操作
* 顺序容器特有的操作, 和关联容器特有的操作
* 只适用于一小部分容器的操作

某些容器操作对元素类型有要求
* 如果某类型不符合某个操作的需求, 使用该类型定义的容器不能使用该操作

容器通用操作

|             操作              |                                   描述                                   |
| ---------------------------- | ----------------------------------------------------------------------- |
| 类型别名                      |                                                                         |
| `iterator`                   | 该容器类型的迭代器类型                                                    |
| `const_iterator`             | 迭代器类型; 可读取元素, 不可修改元素                                       |
| `size_type`                  | 无符号整数类型, 足够保存该容器类型最大可能对象的尺寸                         |
| `difference_type`            | 带符号整数类型, 足够保存两个迭代器的距离                                    |
| `value_type`                 | 元素类型                                                                 |
| `reference`                  | 元素的左值类型; 与`value_type&`含义相同                                   |
| `const_reference`            | 元素的`const`左值类型, 等于`const value_type&`                            |
| 构造函数                      |                                                                         |
| `C c;`                       | 默认构造函数, 构造空容器(`array`;见p336)                                  |
| `C c1(c2);`                  | 构造容器`c1`, `c1`是`c2`的拷贝                                            |
| `C c(b, e);`                 | 构造`c`, `c`的元素是迭代器`b`和`e`指定的范围内的元素的拷贝;(不适用于`array`) |
| `C c{a, b...};`              | 列表初始化`c`                                                            |
| 赋值与`swap`                  |                                                                         |
| `c1 = c2`                    | 把`c1`中的元素替换为`c2`中的元素                                          |
| `c1 = {a, b...}`             | 把`c1`中的元素替换为列表中的元素(不适用于`array`)                           |
| `a.swap(b)`                  | 交换`a`和`b`的元素                                                       |
| `swap(a,b)`                  | 交换`a`和`b`的元素                                                       |
| 尺寸                          |                                                                         |
| `c.size()`                   | `c`中元素的数目(不适用于`forward_list`)                                   |
| `c.max_size()`               | `c`可保存的最大元素数目                                                   |
| `c.empty()`                  | 如果`c`中有任一元素, 返回`false`, 否则返回`true`                           |
| 添加/删除元素(不适用于`array`) | 注: 在不同容器中,这些操作的接口都不同                                      |
| `c.insert(args)`             | 将`args`指定的元素拷贝到`c`中                                             |
| `c.emplace(inits)`           | 使用`inits`构造`c`中的一个元素                                            |
| `c.erase(args)`              | 删除`args`指定的元素                                                      |
| `c.clear()`                  | 删除`c`中的所有元素; 返回`void`                                           |
| 相等和关系运算符               |                                                                         |
| `==`, `!=`                   | 所有容器都支持相等性运算符                                                 |
| `<`, `<=`, `>`, `>=`         | 关系运算符(无序关联容器不支持)                                             |
| 获取迭代器                    |                                                                         |
| `c.begin()`, `c.end()`       | 返回指向`c`的首元素和尾后位置的迭代器                                      |
| `c.cbegin()`, `c.cend()`     | 返回`const_iterator`                                                     |
| 反向容器的额外成员             | (不支持`forward_list`)                                                   |
| `reverse_iterator`           | 逆序寻址的迭代器类型                                                      |
| `const_reverse_iterator`     | 不能修改元素的逆序迭代器类型                                               |
| `c.rbegin()`, `c.rend()`     | 返回指向`c`的尾元素和首前位置的迭代器                                      |
| `c.crbegin()`, `c.crend()`   | 返回`const_reverse_iterator`                                             |

## 2.1迭代器*
各种容器的迭代器有着公共的接口
* 如果迭代器提供某个操作, 那么所有提供该操作的迭代器以相同的形式支持该操作

除了一个例外, 所有容器迭代器都支持([s3.4: 标准容器迭代器支持的操作](P1-C03-Strings_Vectors_Arrays.md))所示的操作
* 例外: `forward_list`的迭代器不支持递减运算符`--`

以下容器的迭代器支持([s3.4: vector和string迭代器支持的算术运算](P1-C03-Strings_Vectors_Arrays.md))所示的操作
* `string`, `vector`, `deque`, `array`

### 迭代器范围
> 注意: 迭代器范围, 这个概念是标准库的基础之一

一个迭代器范围(**iterator range**)由一对迭代器表示, 标记某个容器中元素的范围
* 通常, 一个迭代器指向范围内的第一个元素, 一个迭代器指向范围内最后一个元素之后的位置
    * 这两个迭代器通常被称为`begin`和`end`, 或者`first`和`last`
* 迭代器范围是左闭合区间, 其标准数学描述为`[begin, end)`

对构成范围的迭代器的规范:
* 迭代器`begin`和`end`必须指向相同的容器
* 迭代器`begin`和`end`它们要么指向某个元素, 要么指向尾后位置(one past the last element);
* 反复递增`begin`可能到达`end`; 也就是说, `end`不在`begin`之前

使用左闭合范围有三个性质
* `begin`与`end`相等时, 范围为空
* `begin`与`end`不等时, 范围至少包含一个元素, 且`begin`指向该范围的第一个元素
* 可以反复递增`begin`, 直到`begin == end`
* 因为这三个性质, 可以安全使用如下的代码来处理一个元素范围
```c++
while (begin != end) {
      *begin = val;
      ++begin;
}
```

## 2.2容器类型成员
见容器通用操作表格
## 2.3返回迭代器的成员*
迭代器类型
* `iterator`
* `const_iterator`
* `reverse_iterator`
* `const_reverse_iterator`

返回迭代器的成员: 如果容器是`const`, 返回`const`迭代器, 否则返回非`const`迭代器
* `begin`: 返回指向容器内首元素的迭代器, 类型为`iterator`或`const_iterator`
* `end`: 返回指向容器内尾后位置的迭代器, 类型为`iterator`或`const_iterator`
* `rbegin`: 返回指向容器内尾元素的逆序迭代器, 类型为`reverse_iterator`或`const_reverse_iterator`
* `rend`: 返回指向容器内首前位置的逆序迭代器, 类型为`reverse_iterator`或`const_reverse_iterator`
    * 首前位置, 是指首元素前一个"元素"的位置

(C++11)返回`const`迭代器的成员函数: 与前面的成员不同的是必定返回`const`迭代器
* `cbegin`
* `cend`
* `crbegin`
* `crend`

使用`begin`和`end`返回的迭代器指示了一个包含所有元素的范围
* 类似的, `cbegin`和`cend`, `rbegin`和`rend`, `crbegin`和`crend`也是如此

## 2.4定义和初始化容器*
|               构造函数                |                 描述                  |
| ------------------------------------ | ------------------------------------ |
| `C c;`                               | 默认初始化                            |
| `C c1(c2)`, `C c1 = c2`              | `c1`是`c2`的拷贝                      |
| `C c{a, b, e...}`, `C c{a, b, e...}` | `c`是初始符列表中所有元素的拷贝         |
| `C c(b,e)`                           | `c`是`b`和`e`表示的范围内所有元素的拷贝 |
| 以下只对顺序容器(`array`除外)有效      |                                      |
| `C seq(n)`                           | `seq`有`n`个值初始化的元素             |
| `C seq(n, t)`                        | `seq`有`n`个值为`t`的元素              |

具体说明

* `C c;`
    * 构造`c`;
    * 如果`c`是`array`, `c`中的元素默认初始化
    * 其他情况,  `c`是空容器

* `C c1(c2)`, `C c1 = c2`
    * 构造`c1`; `c1`是`c2`的拷贝
    * `c1`和`c2`必须是相同的类型; (即相同的容器类型, 且保存的是相同的元素类型; 对于`array`, 还必须具有相同的大小)

* `C c(b, e)`
    * 构造`c`, `c`的每个元素是`b`和`e`指定的范围中对应元素的拷贝; `c`的尺寸是范围中元素的数量
    * `b`和`e`所指元素的类型与`c`的元素类型要匹配
    * 不适用于`array`

* `C c{a,b, e...}`, `C c = {a, b, e...}` (C++11)
    * 构造`c`, `c`的每个元素是初始符列表中对应元素的拷贝;  对于除`array`外的容器, `c`的尺寸是列表中元素的数量

* `C seq(n)`
    * 构造`seq`, 有`n`个值初始化的元素
    * 元素要么是内置类型, 要么有默认构造函数

* `C seq(n, t)`
    * 构造`seq`, 有`n`个值为`t`的元素

`array`类型的说明

* `array`的尺寸是类型的一部分

* 定义`array`时需要指明元素类型和尺寸

  ```c++
  array<int, 42> arr;
  array<string, 10> arr2;
  ```

* 默认初始化时, `array`的元素被默认初始化
* 列表初始化时, 列表中元素的数量必须小于或等于`array`的尺寸; 如果小于, 剩余的没有初始符的元素被值初始化
* `array`支持拷贝或赋值操作

## 2.5赋值和`swap`

| 操作                                | 描述                                                      |
| ----------------------------------- | --------------------------------------------------------- |
| `c1 = c2`                           | 把`c1`的元素替换为`c2`的元素的拷贝; 两者的类型必须相同    |
| `c = {a, b, e...}`                  | 把`c1`的元素替换为初始符列表中的元素的拷贝(对`array`无效) |
| `swap(c1, c2)`, `c1.swap(c2)`       | 交换`c1`和`c2`的元素; 两者的类型必须相同                  |
| `assign`操作对关联容器和`array`无效 |                                                           |
| `seq.assign(b, e)`                  | 把`seq`的元素替换为`b`和`e`所表示的范围中的元素           |
| `seq.assign(il)`                    | 把`seq`的元素替换为花括号初始符列表`il`中的元素           |
| `seq.assign(n, t)`                  | 把`seq`的元素替换为`n`个值为`t`的元素                     |

* 警告: 
  * 赋值相关操作会导致指向左边容器的迭代器, 引用和指针失效; 
  * (除了`string`和`array`) 而`swap`操作不会使其失效, 不过它们指向的容器相互交换了

具体说明

* `assign`成员函数: 
  * 要求
    * 参数所指的元素类型要和容器的元素类型匹配, 即相同或能转换成元素类型
  * 三种方式
    * `seq.assign(b, e)`: 迭代器`b`和`e`不能指向`seq`中元素
    * `seq.assign(il)`
    * `seq.assign(n, t)`

* `swap`成员函数

  * 要求: 类型必须相同
  * 两种方式
    * `swap(c1, c2)`(C++11)
    * `c1.swap(c2)`
  * (除了`string`和`array`之后)容器执行`swap`之后, 指向这两个容器的迭代器, 引用和指针不会失效, 不过它们指向的容器相互交换了

  * 说明: 
    * 除了`array`外, `swap`操作很快, 能在常数时间内完成, 因为`swap`交换的是两个容器内部的数据结构, 而不是元素本身
    * 因为`swap`并不是交换元素, 所以指向容器的迭代器, 引用和指针在`swap`操作之后都不会失效, 它们仍指向操作之前的那些元素; 但是在`swap`操作之后, 这些元素已经属于不同的容器, 所以这些迭代器, 引用和指针所属的容器相互交换

## 2.6容器`size`操作*

见容器通用操作表格

## 2.7关系运算符

`==`, `!=`, `<`, `<=`, `>`, `>=`

* 所有容器支持`==`, `!=`; 除无序关联容器之外的容器支持`<`, `<=`, `>`, `>=`

* 要求: 作为操作数的容器对象必须是类型相同的
* 比较两个容器, 实际是对元素逐对进行比较; 具体如下
  * 如果两个容器具有相同的尺寸, 且所有元素都相等, 则两容器相等; 否则, 两容器不相等
  * 如果两个容器大小不同, 的较小容器中的每个元素都与较大容器中的对应元素相等,则较小容器小于较大容器
  * 如果两个容器都不吃另一个容器的前缀子序列, 则比较结果取决于第一个不相等的元素的比较结果
* 容器的关系运算符使用元素的关系运算符完成比较
  * 所以只有当其元素类型也定义了相应的比较运算符时, 才可以使用关系运算符来比较两个容器
  * 实际上(在实现中), 容器的相等运算符`==`和`!=`是使用元素的`==`运算符实现比较; 其他关系运算符是使用元素的`<`运算符.

# 3.顺序容器操作

顺序容器与关联容器的不同在于两者组织元素的方式, 而这些差异影响到元素如何存储, 访问, 添加和删除.

## 3.1向顺序容器添加元素*

除了`array`, 所有标准库容器都提供灵活的内存管理. 在运行时可以动态添删元素, 改变容器的大小

下表是顺序容器的添加元素的操作

| 操作                    | 描述                                                         |
| ----------------------- | ------------------------------------------------------------ |
| `c.push_back(t)`        | 在`c`的尾部创建一个值为`t`的元素, 返回`void`                 |
| `c.emplace_back(args)`  | 在`c`的尾部创建一个根据参数`args`构造的元素, 返回`void`      |
| `c.push_front(t)`       | 在`c`的头部创建一个值为`t`的元素, 返回`void`                 |
| `c.emplace_front(args)` | 在`c`的头部创建一个根据参数`args`构造的元素, 返回`void`      |
| `c.insert(p, t)`        | 在迭代器`p`所表示的位置的前面创建一个值为`t`的元素; 返回指向新元素的迭代器 |
| `c.empalce(p, args)`    | 在迭代器`p`所表示的位置的前面创建一个根据参数`args`构造的元素; 返回指向新元素的迭代器 |
| `c.insert(p, n, t)`     | 在迭代器`p`所表示的位置的前面插入`n`个值为`t`的元素; 返回指向第一个新元素的迭代器 |
| `c.insert(p, b, e)`     | 在迭代器`p`所表示的位置的前面插入迭代器`b`和`e`指定范围内的元素; `b`和`e`不能指向`c`中的元素; 返回指向第一个新元素的迭代器; 若范围为空, 返回`p` |
| `c.insert(p, il)`       | 在迭代器`p`所表示的位置的前面插入花括号初始符列表`il`的元素; 返回指向第一个新元素的迭代器; 若范围为空, 返回`p` |

* 备注
  * `array`不支持这些操作;
  * `forward_list`有专有版本的`insert`和`empalce`, 不支持`push_back`和`emplace_back`
  * `vector`和`string`不支持`push_front`和`emplac_fron`
  * 向`vector`, `string`或`deque`插入元素可能会使它们的迭代器, 引用和指针失效
  * C++11之前的标准库中, 接受元素个数或范围的`insert`返回`void`
  * C++11标准引入`emplace_front`, `emplace_back`, `emplace`; `args`参数必须与元素类型的构造函数相匹配

* 性能:
  * `deque`的在头尾插入元素的操作是常数时间级
  * `vector`和`string`在尾部插入元素的操作是常数时间级

## 3.2访问元素*

下表是顺序容器的访问元素的操作

| 操作        | 描述                                                         |
| ----------- | ------------------------------------------------------------ |
| `c.back()`  | 返回指向`c`中尾元素的引用; 如果`c`为空, 行为未定义           |
| `c.front()` | 返回指向`c`中首元素的引用; 如果`c`为空, 行为未定义           |
| `c[n]`      | 返回指向`c`中下标为`n`的元素引用; 如果`n>=c.size()`, 行为未定义 |
| `c.at(n)`   | 返回指向`c`中下标为`n`的元素引用; 如果`n>=c.size()`, 抛出`out_of_range`异常 |

* 备注

  * `at`和下标操作只对`string`, `vector`, `deque`, `array`等有效
  * `back`对`forward_list`无效

  * 如果容器是`const`的, 以上操作返回`const`引用
  * `at`和下标操作的`n`的有效范围: `0 <= n < c.size()`

* 性能
  * `string`, `vector`, `deque`和`array`支持快速随机访问

## 3.3删除元素*

下表是顺序容器的删除元素的操作

| 操作            | 描述                                                         |
| --------------- | ------------------------------------------------------------ |
| `c.pop_back()`  | 删除`c`中的尾元素; 若`c`为空, 行为未定义; 返回`void`         |
| `c.pop_front()` | 删除`c`中的首元素; 若`c`为空, 行为未定义; 返回`void`         |
| `c.erase(p)`    | 删除`c`中迭代器`p`所指的元素, 返回被删元素后面的位置的迭代器; 若`p`为尾后迭代器, 行为未定义 |
| `c.erase(b, e)` | 删除`c`中迭代器`b`和`e`所指范围内的元素, 返回最后一个被删元素后面的位置的迭代器;; |
| `c.clear()`     | 删除`c`中的所有元素; 返回`void`                              |

* 备注
  * `array`不支持这些操作
  * `forward_list`有专属的`erase`成员
  * `forward_list`不支持`pop_back`
  * `pop_front`不对`vector`和`string`有效
  * 这些操作不检查其参数
* 警告
  * 对于`deque`, 删除`deque`中除首尾位置之外的任何元素都会使所有迭代器, 指针和引用失效
  * 对于`vector`和`string`, 删除某个元素导致指向该元素之后位置的迭代器, 指针和引用失效

## 3.4`forward_list`特有的操作

>  `forward_list`需要特殊版本的添加和删除操作的原因是, 它的实现是基于单向链表的

下表是`forward_list`的添加和删除元素的操作

| 操作                        | 描述                                                         |
| --------------------------- | ------------------------------------------------------------ |
| `lst.before_begin()`        | 返回指向链表首元素前面的位置的迭代器                         |
| `lst.cbefore_begin()`       | 返回指向链表首元素前面的位置的迭代器, 类型是`const_iterator` |
| `lst.insert_after(p, t)`    | 在迭代器`p`之后的位置插入值为`t`元素, 返回指向新元素的迭代器; 若`p`为尾后迭代器, 行为未定义 |
| `lst.insert_after(p, n, t)` | 在`p`之后的位置插入`n`个值为`t`元素, 返回指向最后一个新元素的迭代器; 若`p`为尾后迭代器, 行为未定义 |
| `lst.insert_after(p, b, e)` | 在`p`之后的位置插入迭代器`b`和`e`之间的元素, 返回指向最后一个新元素的迭代器; 若`p`为尾后迭代器, 行为未定义 |
| `lst.insert_after(p, il)`   | 在`p`之后的位置插入花括号初始符列表`il`中的元素, 返回指向最后一个新元素的迭代器; 若`p`为尾后迭代器, 行为未定义 |
| `emplace_after(p, args)`    | 在`p`之后的位置插入由实参`args`构造的元素, 返回指向新元素的迭代器; 若`p`为尾后迭代器, 行为未定义 |
| `lst.erase_after(p)`        | 删除`p`之后的位置上的元素, 返回指向被删元素之后的迭代器; 若`p`为尾后迭代器, 行为未定义 |
| `lst.erase_after(b, e)`     | 删除从迭代器`b`之后到`e`前面的元素, 返回指向最后一个被删元素之后的迭代器 |

## 3.5改变容器大小

| 操作             | 描述                                                         |
| ---------------- | ------------------------------------------------------------ |
| `c.resize(n)`    | 调整`c`的大小为`n`; 若`n < c.size()`, 则容器后面多出的元素被丢弃; 若`n>c.size()`, 则新元素被值初始化 |
| `c.resize(n, t)` | 调整`c`的大小为`n`, 任何新添加的元素的值为`t`                |

* 备注
  * 这些操作不适用于`array`
  * 如果`resize`缩小容器, 则指向被删元素的迭代器, 引用和指针都会失效
  * 对`vector`, `stirng`或`deque`进行`resize`操作, 可能导致迭代器, 引用和指针失效

## 3.6 容器操作可能使迭代器, 引用和指针失效*

> 失效的指针, 引用和迭代器将不再表示任何元素; 不能使用失效的指针, 引用或迭代器

在向容器添加元素后

* `vector`和`string`
  * 迭代器, 指针和引用都会失效, 如果它们的存储空间被重新分配;
  * 如果在添加元素后没有发生重新分配, 在插入位置之前的迭代器, 引用和指针仍然有效, 在插入位置后的会失效
* `deque`
  * 迭代器, 引用和指针都会失效, 如果新元素插入到除首尾位置之外的任何位置;
  * 如果在首尾位置插入新元素, 迭代器会失效, 但是引用和指针不会失效

* `list`和`forward_list`
  * 迭代器, 引用和指针仍然有效

在从容器在删除一个元素后

* 指向被删元素的迭代器, 引用和指针都会失效
* `vector`和`string`
  * 在删除点前的迭代器, 指针和引用都仍然有效
  * 在删除点后的迭代器, 指针和引用都会失效

* `deque`
  * 指向其他所有元素的迭代器, 引用和指针都会失效, 如果在除首尾位置之外的任何位置删除元素
  * 如果删除`deque`的尾元素, 尾后迭代器失效, 但是其他迭代器, 引用和指针都不受影响
  * 如果删除`deque`的首元素, 迭代器, 引用和指针都仍然有效

* `list`和`forward_list`
  * 指向其他所有元素的迭代器, 引用和指针仍然有效

# 4.`vector`对象是如何增长*

`vector`对象是如何增长; 见p355 

下表是管理容量的操作

| 操作                | 描述                                                         |
| ------------------- | ------------------------------------------------------------ |
| `c.shrink_to_fit()` | 请求将`capacity()`减少为与`size()`相同大小                   |
| `c.capacity()`      | 在不重新分配的话, `c`的当前已分配的内存空间可以保存的元素数量 |
| `c.reserve(n)`      | 分配至少能容纳`n`个元素的内存空间                            |

* 备注
  * `shrink_to_fit`只适用于`vector`, `string`, `deque`
  * `capacity`和`reserve`只适用于`vector`和`string`
* 具体说明
  * `reserve`
    * 如果请求的空间超过当前容量, 则`reserve`调用改变容量, 至少分配与需求一样大的空间
    * 如果请求的空间小于或等于当前容量, 则`reserve`什么都不做; 也就是说, 不会减少容留
  * `shrink_to_fit()`
    * 作用是请求退回不需要的内存空间
    * 具体实现可以忽略此请求, 也就是说`shrink_to_fit`并不保证一定退回内存空间

# 5.`string`的额外操作

除了顺序容器共有的操作之外, `string`类型还提供了一些额外的操作. 这些操作主要是

* 提供`string`类和C风格字符串数组之间的相互转换
* 增加用下标代替迭代器的版本的操作

## 5.1构造`string`的其他方式

| 操作                       | 描述                                                         |
| :------------------------- | ------------------------------------------------------------ |
| `string s(cp, n)`          | `s`是指针`cp`所指的字符数组中前`n`个字符的拷贝; 此数组至少应该包含`n`个字符 |
| `string s(s2, pos2)`       | `s`是`string s2`从下标`pos2`开始的字符的拷贝; 若`pos2 > s2.size()`, 抛出`out_of_range`异常 |
| `string s(s2, pos2, len2)` | `s`是`string s2`从从下标`pos2`开始`len2`个字符的拷贝; 若`pos2 > s2.size()`, 抛出`out_of_range`异常. 无论`len2`的值是多少, 至多拷贝`s2.size() - pos2`个字符 |

* 备注
  * `n`, `len2`, `pos2`都是无符号值
* 具体说明
  * `string s(cp, n)`
    * `cp`指向的数组不必以空字符结尾
    * 如果`n `大于数组大小, 行为未定义

### `substr`操作

`s.substr(pos, n)`

* 作用: 返回一个`string`对象; 该对象包含从`s`的下标`pos`开始的`n`个字符
* 说明
  * `pos`的默认实参为0; 若`pos > s.size()`, 抛出`out_of_range`异常
  *  `n`的默认实参是一个导致标准库拷贝从`pos`开始的所有字符的值`npos`
  * 如果`pos + n > s.size()`, 则从`pos`开始拷贝到`string` 的末尾
* 例子

```c++
string s("hello world");
string s2 = s.substr(0, 5); 	//s2 = hello
string s3 = s.substr(6);		//s3 = world
string s4 = s.substr();			//s4 = hello world
string s5 = s.substr(12);		//throws an out_of_range exception
```

## 5.2改变`string`的其他方式

修改`string`的操作

| 操作                      | 描述                                                         |
| ------------------------- | ------------------------------------------------------------ |
| `s.insert(pos, args)`     | 在`pos`前插入由args指定的字符;                               |
| `s.erase(index, len)`     | 删除从`index`开始的`len`个字符; 若`len`被省略, 则删除到末尾. 返回一个指向`s`的引用 |
| `s.assign(args)`          | 将`s`中的所有字符替换成由`args`指定的字符; 返回一个指向`s`的引用 |
| `s.append(args)`          | 在`s`的末尾添加由`args`指定的字符; 返回一个指向`s`的引用     |
| `s.replace(range, args)`  | 删除`s`中`rang`内的字符, 替换成由`args`指定的字符;  返回一个指向`s`的引用 |
| `args`可以是下列形式之一: | `append`和`assign`可以使用所有形式; `str`不能与`s`相同, 迭代器`b`和`e`不能指向`s` |
| `str`                     | 名为`str`的`string`对象                                      |
| `str, index, len`         | `str`中从`index`开始最多`len`个字符                          |
| `cp, len`                 | `cp`指向的字符数组中从头开始最多`len`个字符; `cp`可以不是以空字符结尾 |
| `cp`                      | `cp`指向的以空字符结尾的字符数组                             |
| `n, c`                    | `n`个字符`c`                                                 |
| `b, e`                    | 迭代器`b`和`e`指定的范围内的字符                             |
| *initializer list*        | 花括号包围的, 以逗号分隔的字符列表                           |

* `pos`可以是下标或迭代器

* `range`可以是一个下标加一个长度, 或者一对指向`s`的迭代器

* `replace`和`insert`所允许的`args`形式依赖于`range`和`pos`是如何指定的

  | `replace (index,len,args)` | `replace (b,e,args)` | `insert (index,args)` | `insert (iter,args)` | `args`可以是       |
  | -------------------------- | -------------------- | --------------------- | -------------------- | ------------------ |
  | yes                        | yes                  | yes                   | no                   | `str`              |
  | yes                        | no                   | yes                   | no                   | `str,pos,len`      |
  | yes                        | yes                  | yes                   | no                   | `cp, len`          |
  | yes                        | yes                  | no                    | no                   | `cp`               |
  | yes                        | yes                  | yes                   | yes                  | `n, c`             |
  | no                         | yes                  | no                    | yes                  | `b2, e2`           |
  | no                         | yes                  | no                    | yes                  | *initializer list* |

  * 表中, `index`是下标, `len`是长度; `b`, `e`和`iter`是迭代器

* 具体说明

  * `s.insert(pos, args)`
    * 若`pos`是下标, 则返回一个指向`s`的引用; 若`pos`是迭代器, 则返回一个指向第一个插入字符的迭代器

## 5.3`string`的搜索操作

| 操作                        | 描述                                                         |
| --------------------------- | ------------------------------------------------------------ |
| `s.find(args)`              | 在`s`中找到`args`第一次出现的位置                            |
| `s.rfind(args)`             | 在`s`中找到`args`最后一次出现的位置                          |
| `s.find_first_of(args)`     | 在`s`中找到`args`任何一个字符第一次出现的位置                |
| `s.find_last_of(args)`      | 在`s`中找到`args`任何一个字符最后一次出现的位置              |
| `s.find_first_not_of(args)` | 在`s`中找到第一次不在`args`中的位置                          |
| `s.find_last_not_of(args)`  | 在`s`中找到最后一次不在`args`中的位置                        |
| `args`必须是以下形式之一    |                                                              |
| `c, pos`                    | 从`s`的位置`pos`开始查找字符`c`; `pos`默认为0                |
| `s2, pos`                   | 从`s`的位置`pos`开始查找`string s2`; `pos`默认为0            |
| `cp, pos`                   | 从`s`的位置`pos`开始查找`cp`所指的C风格字符串; `pos`默认为0  |
| `cp, pos, n`                | 从`s`的位置`pos`开始查找`cp`所指的C风格字符串前`n`个字符; 无默认实参 |

* 所有搜索操作返回指定字符的下标, 类型是`string::size_type`;如果没找到的话, 返回静态成员`string::npos`
  
* 标准库定义`string::npos`是一个`const string::size_type`类型的值`-1`; 因为它是无符号类型, 所以它的值意味着`npos`等于`string`最大的可能尺寸
  
* 例子

  ```c++
  string name("AnnaBelle");
  auto pos1 = name.find("Anna");				//pos1 == 0
  string numbers("0123456789"), name("r2d2");
  auto pos2 = name.find_first_of(numbers);	//pos2 == 1
  string dept("03714ps");
  auto pos3 = dept.find_first_not_of(numbers);//pos3 == 5
  string river("Missisiippi");
  auto first_pos = river.first("is");			//returns 1
  auto last_pos = river.rfirst("is");			//returns 4
  ```

## 5.4`string`的`compare`函数

标准库为`string`提供了一组与`strcmp`函数类似的函数, `compare`函数

`s.compare(args)`

* 根据`s`是等于, 大于或小于`args`指定的字符串, 相应地返回0, 正数或负数

* `args`必须是以下形式之一

  | 形式                     | 描述                                                         |
  | ------------------------ | ------------------------------------------------------------ |
  | `s2`                     | 比较`s`和`s2`                                                |
  | `pos1, n1, s2`           | 将`s`中从`pos1`开始的`n1`个字符与`s2`比较                    |
  | `pos1, n1, s2, pos2, n2` | 将`s`中从`pos1`开始的`n1`个字符与`s2`中从`pos2`开始的`n2个字符比较 |
  | `cp`                     | 比较`s`和`cp`所指的空终止字符数组                            |
  | `pos1, n1, cp`           | 将`s`中从`pos1`开始的`n1`个字符与`cp`所指的空终止字符数组比较 |
  | `pos1, n1, cp, n2`       | 将`s`中从`pos1`开始的`n1`个字符与`cp`所指的地址开始的`n2`个字符比较 |

## 5.5数值转换(C++11)*

C++11引入多个函数, 可以实现数值数据与`string`之间的转换

| 函数              | 描述                                                         |
| ----------------- | ------------------------------------------------------------ |
| `to_string(val);` | 返回数值`val`的`string`表示; `val`可以是任何算术类型         |
| `stoi(s, p, b)`   | 把`s`的表示整数的起始子串转换成数值, 返回该数值 ; 返回类型是`int`; |
| `stol(s, p, b)`   | 同上; 返回类型是`long`                                       |
| `stoul(s, p, b)`  | 同上; 返回类型是`unsigned long`                              |
| `stoll(s, p, b)`  | 同上; 返回类型是`long long`                                  |
| `stoull(s, p, b)` | 同上; 返回类型是`unsigned long long`                         |
| `stof(s, p)`      | 把`s`的表示浮点数的起始子串转换成数值, 返回该数值 ; 返回类型是`float` |
| `stod(s, p)`      | 同上; 返回类型是`double`                                     |
| `stold(s, p)`     | 同上; 返回类型是`long double`                                |

* `p`是指向`size_type`的指针, 用于保存`s`中第一个非数值字符的下标; 默认为0, 即表示函数不保存下标

* `b`表示转换所用的基数, 默认为10;
* 如果`string`不能转换成一个数值, 抛出`invalid_argumetn`异常; 如果转换得到的数值无法用任何类型来表示, 抛出`out_of_range`异常

表示数值的字符

* 要转换成数组的`string`中的第一个非空白符必须是数值中可能出现的字符; 
  * 对于整数, 开头的字符有符号"+-"和数字"0123456789"; 也可能以`0x`或`0X`开头表示十六进制数
  * 对于浮点数, 开头的字符有符号"+-."和数字"0123456789"; 

* 根据基数不同, `string`可以包含字母字符, 对应大于数字9的数; 
* 在浮点数中 `e`或`E`表示指数部分

# 6.容器适配器

>  适配器(**adaptor**)是标准库中的一个通用概念; 也是一种机制, 能使某种东西的行为像另一种东西一样.
>
> * 容器, 迭代器和函数都有适配器

容器适配器接受一个已有的容器类型, 使其行为像一种不同的类型; 

* 有三种顺序容器的适配器: `stack`, `queue`, `priority_queue`
* 容器适配器基于底层容器类的操作定义了自己的特殊操作
* 下表是所有容器适配器都支持的操作和类型

| 成员                             | 描述                                                         |
| -------------------------------- | ------------------------------------------------------------ |
| `size_type`                      | 一种类型, 足以保存当前类型的最大对象的大小                   |
| `value_type`                     | 元素类型                                                     |
| `container_type`                 | 实现适配器的底层容器类型                                     |
| `A a;`                           | 创建一个名为`a`的空适配器                                    |
| `A a(c);`                        | 创建一个名为`a`的适配器, 带有容器`c`的一个拷贝               |
| `==`, `!=`, `<`, `<=`, `>`, `>=` | 适配器支持所有关系运算符; 这些运算符返回底层容器的比较结果   |
| `a.empty()`                      | 若`a`包含任何元素, 返回`false`, 否则返回`true`               |
| `a.size()`                       | 返回`a`中的元素数目                                          |
| `swap(a,b)`                      | 交换`a`和`b`的内容, `a`和`b`必须有相同的类型, 包括底层容器类型 |
| `a.swap(b)`                      | 同上                                                         |

## 定义适配器

每个适配器都定义两种构造函数: 默认构造函数, 和接受一个容器的构造函数

默认下, `stack`和`queue`是基于`deque`实现, `priority_queue`基于`vector`实现

* 例子

  ```c++
  deque<int> deq;
  stack<int> stk1;
  stack<int> stk2(deq);
  ```

在创建适配器时, 可以将一个顺序容器类型作为第二个类型参数, 来覆盖默认容器类型

* 关于容器类型的要求
  * 所有适配器都要求容器具有添加和删除元素的能力, 都需要容器有添加, 删除以及访问尾元素的能力
    * 因此, 不能基于`array`和`forward_list`来构造适配器
  * `stack`适配器只要求`push_back`, `pop_back`和`back`操作
    * 因此, 除了`array`和`forward_list`外的顺序容器都能构造`stack`
  * `queue`适配器要求`back`, `push_back`, `fron`和`push_front`
    * 因此, `list`和`deque`可以构造`queue`, 其他不能
  * `priority_queue`要求`push_back`, `pop_back`和`back`操作, 还有随机访问能力
    * 因此, `priority_queue`可以构造于`vector`或`deque`之上, 其他不能

* 例子

  ```c++
  stack<string, vector<string>> str_stk;
  vector<string> svec;
  stack<string, vector<string>> str_stk2(svec);
  ```

## `stack`适配器

`stack`类型, 定义在`stack`头文件中

* 默认基于`deque`实现, 也可以在`list`或`vector`之上实现

| 操作              | 描述                                                     |
| ----------------- | -------------------------------------------------------- |
| `s.pop()`         | 删除`stack`的栈顶元素; 返回`void`                        |
| `s.push(item)`    | 创建一个新元素并压入栈顶; 该元素通过拷贝或移动`item`而来 |
| `s.emplace(args)` | 创建一个新元素并压入栈顶; 该元素由`args`构造             |
| `s.top()`         | 返回栈顶元素, 但是不会将其删除                           |

## `queue`和`priority_queue`适配器

`queue`和`priority_queue`类型, 定义在`queue`头文件

* `queue`默认基于`deque`实现, 也可以在`list`或`vector`之上实现
* `priority_queue`默认基于`vector`实现, 也可以在`list`之上实现
* 下表是`queue`和`priority_queue`的更多操作

| 操作              | 描述                                                   |
| ----------------- | ------------------------------------------------------ |
| `q.pop()`         | 对于`queue`, 删除首元素; 返回`void`                    |
|                   | 对于`priority_queue`, 删除最高优先级的元素; 返回`void` |
| `q.front()`       | 返回首元素; (只适用于`queue`)                          |
| `q.back()`        | 返回尾元素; (只适用于`queue`)                          |
| `q.top()`         | 返回最高优先级的元素;(只适用于`priority_queue`)        |
| `q.push(item)`    | 在`queue`末尾或`priority_queue`中恰当的位置创建...     |
| `q.emplace(args)` | ...一个元素, 其值为`item`, 或者由`args`构造            |

`queue`使用一种先进先出(first-in, first-out, FIFO)的存储和访问策略

`priority_queue`允许我们为队列中的元素建立优先级.

* 新加入的元素会排在所有优先级比它低的已有元素之前
* 默认情况下, 标准库在元素类型上使用`<`运算符来确定相对优先级