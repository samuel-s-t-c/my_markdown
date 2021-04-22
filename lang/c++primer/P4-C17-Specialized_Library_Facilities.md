# 总结

* 四种特殊用途的标准库设施
  * `bitset`
  * `tuples`
  * 正规表达式
  * 随机数

* IO库的不常用的部分

[toc]

# 1.`tuple`类型

`tuple`类, 及其伴随类型和函数, 定义在`tuple`头文件中

| 操作                                          | 描述                                                         |
| --------------------------------------------- | ------------------------------------------------------------ |
| `tuple <T1, T2, ..., Tn> t;`                  | `t`有`n`个值初始化的成员, 类型分别为`T1,T2,...,Tn`           |
| `tuple <T1, T2, ..., Tn> t(v1, v2, ..., vn);` | `t`的`n`个成员分别被`v1,v2,...vn`初始化;此构造函数是`explicit`的 |
| `make_tuple(v1, v2, ..., vn)`                 | 返回一个用给定初始符初始化的`tuple`                          |
| `t1 == t2`, `t1 != t2`                        | 若两个`tuple`有相同数量的成员, 且对应的成员相等, 则两个`tupe`相等; 使用成员的`==`运算符; 一旦发现某成员不相等, 则不比较后面的成员 |
| `t1 relop t2`                                 | `tuple`的关系运算符使用字典序. 两个`tuple`必须具有相同数量的成员; 使用成员的`<`运算符 |
| `get<i>(t)`                                   | 返回指向`t`的第`i`个成员的引用; 如果`t`是左值, 则结果是左值引用, 否则是右值引用; `tuple`的所有成员都是`public`的 |
| `tuple_size<tupleType>::value`                | 被`tuple`类型实例化的类模板; 有一个`public constexpr static`成员, 名为`value`, 类型为`size_t`, 表示给定的`tuple`类型中成员数量 |
| `tuple_element<i, tupleType>::type`           | 一个类模板, 可以通过一个整型常量和一个`tuple`类型实例化. 有一个`public`成员, 名为`type`, 表示给定`tuple`类型中指定成员的类型 |

## 1.1定义和初始化`tuple`

## 1.2使用`tuple`返回多个值

# 2.`bitset`类型

## 2.1定义和初始化`bitset`

## 2.2`bitset`的操作

# 3.正则表达式

## 3.1使用正则表达式库

## 3.2匹配与Regex迭代器类

## 3.3使用子表达式

## 3.4使用`regex_replace`

# 4.随机数

## 4.1随机数引擎和分布

## 4.2其他随机数分布

# 5.IO库再探

## 5.1格式化输入与输出

## 5.2未格式化的输入输出操作

## 5.3流随机访问