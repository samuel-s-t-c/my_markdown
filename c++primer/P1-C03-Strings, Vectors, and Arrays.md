# 总结

* 两个重要的标准库类型: `vector`和`string`

* 异同:  `vector`与数组; `string`与字符数组/指向数组元素的指针

* 用于间接访问容器中的对象的类型: 迭代器

  

[toc]
---

内置类型, 是体现了多数计算机硬件本身具备的能力; 

* 如`int`表示整数, `char`表示字符, 可直接从硬件实现. 还有内置数组类型.

标准库定义了表示更高层次抽象的类型, 而这些抽象尚未能直接实现到计算机硬件中.

* 本章将介绍两种最重要的库类型: `string`和`vector`. `string`表示变长(variable-length)的字符序列, `vector`表示变长的对象序列(需指定类型). 

---

# 1.命名空间的`using`声明*

标准库的名字都在`std`命名空间里. 要使用命名空间里的名字的方式有多个:

* 作用域操作符`::`
  
* 如`std::cin`, 表示从左操作数指定的作用域(`std`命名空间作用域)中寻找右操作数指定的名字(`cin`)
  
* `using`指示语句: (s18.2.2 p793)

* `using`声明语句: 使指定命名空间中的指定名字在当前作用域可见

  * 形式: `using namespace::name`

  * ```c++
    #include <iostream>
    // using declaration; when we use the name cin, we get the one from the namespace std
    using std::cin;
    int main() 
    {
        int i = 0;		
        cin >> i;		//ok: cin is a synonym for std::cin
        cout << i;		//error: no using declaration; we must use the full name
        std::cout << i;	//ok: explicitly use cout from namespace std
        return 0;
    }
    ```

  * `using`声明的注意事项

    * 每个`using`声明只引入一个名字; 
    * 头文件中不应该包含`using`声明;

# 2.标准库类型`string`*

`string`类型表示变长的字符序列, 被定义在`std`命名空间, 被声明在`string`头文件.  本节描述常用的`string`操作, 更多操作在(s9.5 p360).

```c++
#include <string>
using std::string;
```

注意: 字符串字面值的类型不是`string`类型

## 2.1定义和初始化`string`*

>  类定义了如何初始化对象

`string`常用的初始化方式

| 方式                  | 描述                                                      |
| --------------------- | --------------------------------------------------------- |
| `string s1`           | 默认初始化; `s1`是一个空字符串.                           |
| `string s2(s1)`       | `s2`是`s1`的拷贝                                          |
| `string s2 = s1`      | 等于`s2(s1)`, `s2`是`s1`的拷贝                            |
| `string s3("value")`  | `s3`是字符串字面值"value"的拷贝, 除了字面值最后的空字符外 |
| `string s3 = "value"` | 等于`s3 ("value")`, `s3`是字符串字面值"value"的拷贝       |
| `string s4(n, 'c')`   | 把`s4`初始化为由连续n个字符'c'组成的字符串                |

直接初始化和拷贝初始化

* 拷贝初始化: 使用`=`初始化变量, 如`string s2 = s1`
* 直接初始化: 使用`()`初始化变量, 一般有多个初始符时使用, 如`string s4 (n, 'c')`

## 2.2`string`的操作

> 类定义了在对象上所能执行的操作; 操作包括: 通过函数名调用的操作, 大多数运算符.

`string`的常用操作

| 操作             | 描述                                                         |
| ---------------- | ------------------------------------------------------------ |
| `os << s`        | 将`s`写到输出流`os`中, 返回`os`                              |
| `is >> s`        | 从输入流`is`中读取字符串赋给`s`, 字符串以空白符[^ws]分隔, 返回`is` |
| `getline(is, s)` | 从`is`中读取一行字符赋给`s`, 返回`is`                        |
| `s.empty()`      | `s`为空返回`true`, 否则返回`false`                           |
| `s.size()`       | 返回`s`中字符的个数; 返回值类型为`string::size`, 是无符号类型 |
| `s [n]`          | 返回`s`中在位置`n`的字符的引用, 位置`n`从0开始               |
| `s1 + s2`        | 返回`s1`和`s2`连接后的`string`临时值                         |
| `s1 = s2`        | 把`s2`所有的字符复制并替代`s1`所有的字符                     |
| `s1 == s2`       | 如果`s1`和`s2`所包含的字符完全一样, 则它们相等, 返回`true`; 相等性是大小写敏感 |
| `s1 != s2`       | 与`s1 == s2`相反, `s1`和`s2`不相等, 返回`true`               |
| `<, <=, >, >=`   | 比较操作是使用字典顺序(**dictionary ordering**), 且大小写敏感 |

`string`的读写操作

* `is >> s`: 执行读操作时, `string`对象会自动忽然开头的空白符, 并从第一个非空白字符开始读取, 直到遇到下一个空白符为止(注意: 不读取空白符).
* `os << s`: 如表格所示.
* `getline(is, s)`: `getline`函数从输入流`is`中读取任意字符, 直到读取第一个换行符为止(注意: 读取了换行符); 然后, 除了换行符外, 其他的字符存储在`string`对象`s`中. (`getline`不是成员函数)

`string`关于尺寸的操作

* `s.empty()`: 如表格所示. (`empty`是成员函数, 所以需要使用`.`操作符指定对象)

* `s.size()`: 返回`string`对象`s`的长度(字符的个数); 该值的类型是`string::size_type`类型

  * > 为什么`s.size( )`的返回值类型是`string::size_type`类型?
    >
    > *  `string::size_type`类型是`string`类中定义的配套类型(**companion type**); 大多数库类型都定义了几种配套类型. 而使用配套类型的目的是, 让使用库类型的代码**不依赖于机器**.
    > *  因为, 内置类型的大小因实现的不同而变化; 定义一个`size_type`类型, 该类型在不同的实现使用不同的底层(内置类型); 这样的话, 代码具有移植性, 即不依赖于机器.
> * 标准库标准保证了`string::size_type`类型是一个无符号整型类型, 且足够存放任何`string`对象的尺寸值. 

  * 注意: 用于存放`string`的`size`函数返回值的变量, 都应该是`string::size`类型.

`string`的连接操作: `string`类赋予`+`加法运算符一个新含义

* `s1 + s2`: 将两个`string`相加得到一个新的`string`对象, 其内容是左边对象的字符的拷贝*串接*(concatenate)右边对象的字符的拷贝.
  * 可以将字符字面值(或字符串字面值)和`string`相加, 理由如下: 
    1. 我们在需要类型`a`的地方使用类型`b`, 只要类型`b`可以自动转换成类型`a`
    2. `string`类允许字符字面值和字符串字面值转换成`string`对象
* `s1 += s2`: 等于`s1 = s1 + s2`; 首先计算得到`s1 + s2`返回的对象, 然后将该对象赋值给`s1`.

`string`的其他操作

* 比较操作`==, !=, <, <=, >, >=`: 使用大小写敏感的字典顺序.

  * > 1. 两个`string`中某些对应位置的字符不同, 则结果是第一对相异字符的比较结果; 如有字符串`abc`和`aacd`, 第一对相异字符在位置1处, 分别为`b`和`a`, 而`b > a`, 所以`abc` > `aacd`;
    > 2. 两个`string`中对应位置的字符都相同, 但长度不同, 则`短string`小于`长string`; 如有字符串`abc`和`abcd`, 对应位置的字符都相同, 为`abc`, 但字符串`abcd`更长, 所以`abc` < `abcd`;
    > 3. 两个`string`的长度和字符都相同, 则两者相等

* 赋值操作: 如表格所示; 为了让库类型像内置类型那样方便使用, 大多数库类型都支持赋值操作

## 2.3处理`string`中的字符*

处理`string`中的字符时的关键问题

* 如何访问字符本身: 访问单个字符, 访问所有字符
* 如何知道/改变字符的特性

`cctype`头文件中定义了一组函数, 是关于知道/改变字符的特性

| 函数          | 描述                                                      |
| ------------- | --------------------------------------------------------- |
| `isalnum(c)`  | `c`是字母或数字时返回`true`                               |
| `isalpha(c)`  | `c`是字母时返回`true`                                     |
| `isdigit(c)`  | `c`是数字时返回`true`                                     |
| `isxdigit(c)` | `c`是十六进制数字时返回`true`                             |
| `isprint(c)`  | `c`是可打印字符时返回`true`; (即`c`是空格或c具有可视形式  |
| `isgraph(c)`  | `c`不是空格但可打印时返回`true`                           |
| `iscntrl(c)`  | `c`是控制字符时返回`true`                                 |
| `ispunct(c)`  | `c`是标点符号时返回`true`                                 |
| `isspace(c)`  | `c`是空白符[^ws]时返回`true`                              |
| `islower(c)`  | `c`是小写字母时返回`true`                                 |
| `isupper(c)`  | `c`是大写字母时返回`true`                                 |
| `tolower(c)`  | 如果`c`是大写字母, 输出对应的小写字母; 否则,  原样输出`c` |
| `toupper(c)`  | 如果`c`是小写字母, 输出对应的大写字母; 否则,  原样输出`c` |

访问`string`中的字符

* 使用`范围for`语句(**range for**)(c++11): 该语句遍历给定序列的每个元素并可对序列的每个值执行给定操作. 

  * ```c++
    // The syntacitc form of range-for statement
    for (declaraction : expression)
        statement
    ```

  * 其中, `expression`部分是一个对象, 用于表示一个序列; `declaraction`部分定义一个变量, 该变量被用于访问序列中的底层元素. 每次迭代中, `declaraction`中的变量的初始符为下一个元素的值. 

  * `declaration`中的变量可声明为引用类型, 以改变`expression`中的对象的元素

* 使用下标(**subscript**): 访问指定的字符
  * `[ ]`下标运算符(**subscript operator**)接受一个`string::size_type`类型的值(被称为下标或索引); 下标, 用于表示要访问的字符的位置; 返回值是该位置上字符的引用.
  * 下标从0计起, 且必须`>=0`和`< size( )`.
  * 任何表达式只要它的值是一个整型值就能作为下标; 该整型值将自动转换为`string::size_type`类型;
  * 注意检查下标的合法性: C++标准并不要求标准库检测下标是否合. 一旦使用一个超出范围的下标, 其行为是未定义的.
* 使用迭代器(**iterator**): 见(s3.4 p106)

[^ws]: whitespace/空白/空白符: 是指空格, 进纸符, 换行符, 回车符, 横向制表符, 纵向制表符等字符 

# 3.标准库类型`vector`*

`vector`用于表示同类型对象的集合, 被定义在`std`命名空间, 被声明在`vector`头文件中; 因为`vector`'容纳'对象, `vector`也称作容器(**container**).

```c++
#include <vector>
using std::vector;
```

`vector`是类模板(**class template**). 

> C++有类模板和函数模板.  
>
> 模板本身不是类或函数; 模板是一份说明, 用于指示编译器生成类或函数. 编译器根据模板创建类或函数的过程, 被称为实例化(**instantiation**). 当使用模板时, 需要指出编译器应实例化成何种类或函数.

 对于类模板, 我们通过提供额外的信息来指定实例化成何种类; 至于需要提供哪些信息由模板决定. 提供信息的方式: 在模板后面跟一对尖括号, 在括号内放上信息. 以`vector`为例, 需提供的是`vector`内所存放对象的类型:

```c++
// ivec holds obejcts of int
vector<int> ivec;				
// C++11: a vector whose elements are vector
vector<vector<string>> file1;	
// Earlier version of C++: in order ot define a vector whose elements are themselves 
// vector, we had to supply a space between the closing angle bracket of the outer 
// vector and its element type
vector<vector<string> > file2;
```

## 3.1定义和初始化`vector`*

`vector`常用的初始化方式

| 初始化方式                      | 描述                                                         |
| ------------------------------- | ------------------------------------------------------------ |
| `vector<T> v1`                  | `vector`存放`T`类型的对象. 默认初始化; `v1`是空的            |
| `vector<T> v2(v1)`              | `v2`包含v1`所有元素的拷贝                                    |
| `vector<T> v2 = v1`             | 等于`v2(v1)`;  `v2`中包含v1`所有元素的拷贝                   |
| `vector<T> v3(n, val)`          | `v3`包含了n个元素, 每个元素的值都是`val`                     |
| `vector<T> v3{n, val}`          | 同上                                                         |
| `vector<T> v4(n)`               | `v4`包含了n个元素, 每个元素都是一个已值初始化的对象的拷贝    |
| `vector<T> v4{n}`               | 同上                                                         |
| `vector<T> v5{a, b, c, ...}`    | 有多少个初始符, `v5`就包含多少个元素; 元素被相应的初始符初始化 |
| `vector<T> v5 = {a, b, c, ...}` | 等于`v5{a, b, c, ...}`                                       |

C++初始化的限制之一

* 如果想用**元素的初始符列表**初始化该变量, 只能使用列表初始化: 把元素初始符列表放在花括号内进行列表初始化.

  > 原文: we can supply a list of element values only by using list initialization in which the initializers are enclosed in curly braces.(p98)

* 注意: 是用**元素的初始符列表**初始化;  后面是个人猜测: 元素是指构成某个对象的底层对象, 如`string`的元素是`char`对象;

值初始化(**value initialization**)

* 当只提供`vector`对象容纳的元素数量, 不提供初始符, 库会创建一个**被值初始化的**(**value-initialized**)元素初始符, 并把它的值赋给初始化`vector`中的每个元素. 这个初始符的值取决于元素的类型:
  * 元素的类型为内置类型, 元素初始符的值为0; 元素的类型为类类型: 先进行零初始化, 再执行默认构造函数.
* 限制: 
  * 元素的类型是不支持默认初始化的类型时, 不能使用值初始化, 必须提供显式的元素初始符进行初始化.
  * 当只提供元素数量, 不提供初始值时, 必须使用直接初始化(即, 小括号形式)

初始化中小括号和花括号的含义

* 小括号: 使用构造函数
* 花括号: 若可以, 进行列表初始化; 否则使用构造函数

## 3.2`vector`的操作*

`vector`支持的常用操作

| 操作                  | 描述                                              |
| --------------------- | ------------------------------------------------- |
| `v.empty()`           | `v`为空时返回`true`; 否则返回`false`              |
| `v.siez()`            | 返回`v`中元素的数量, 类型为`vector<T>::size_type` |
| `v.push_back(t)`      | 向`v`的尾端添加一个值为`t`的元素                  |
| `v[n]`                | 返回`v`中在位置`n`的元素的引用                    |
| `v1 = v2`             | 用`v2`中元素的拷贝替换`v1`中的元素                |
| `v1 = {a, b, c, ...}` | 用列表中元素的拷贝替换`v1`中的元素                |
| `v1 == v2`            | `v1`和`v2`相等的条件是:                           |
| `v1 != v2`            | 它们的元素数量相同且对应位置的元素值都相同        |
| `<, <=, >, >=`        | 以字典顺序进行比较                                |

表格说明

* 对`vector`添加操作的编程假定(之一) : 不能在`范围for`循环中使用`vector`的添加操作, 因为`vector`的动态性质.
* `vector`比较操作: 只有在元素可比较情况下, `vector`对象才能被比较;(注意: 存在不可比较的类型)
* `vector`下标操作: 下标操作不能添加元素; `vector`的下标运算符只能用于访问已存在的元素.

`vector`的使用建议: 

* 对于绝大多数情况, 有效的方法是: 先定义一个空的`vector`对象, 然后在运行时向其添加元素.
* 有一种例外情况, 就是所有元素的值都一样. 这时. 指定大小和值会让`vector`的性能更好.
* 原因: C++标准要求`vector`的实现能够在运行时高效快速地添加元素; 而且, 定义指定大小的`vetor`比直接定义一个空`vecotr`更慢; 因此在多数情况下定义`vector`对象时没必要指定其大小 .

`vector`对象能高效地动态增长也意味着

* 不能在`范围for`循环中向`vector`对象添加元素;
* 任何一种可能改变`vector`对象的操作都会使其迭代器失效;(原因: s9.3.6 p353)

# 4.迭代器介绍*

迭代器(**iterator**), 一种通用的机制, 用于间接访问对象, 该对象为容器对象的元素或`string`对象中的字符. 

有效(valid)的迭代器, 要么指向某个元素, 要么指向容器中尾元素的下一位置; 其余情况的迭代器属于无效.

## 4.1获取迭代器

迭代器类型: `iterator`和`const_iterator`

* 支持迭代器的库类型都定义相应的配套类型: `iterator`和`const_iterator`类型
* `iterator`类型: 读写所指的元素值;
* `const_iterator`类型: 只读(不写)所指的元素值; 库类型的`const`对象只能使用`const_iterator`类型的迭代器

获得迭代器的方式(仅部分库类型含有以下成员函数)

* `begin`: 返回指向第一个元素的迭代器; 若容器没有元素, 返回尾后迭代器(见下); 
* `end`: 返回指向容器"尾元素的下一位置(the one past the end)"的迭代器, 称为尾后(**off-the-end**)迭代器或尾迭代器(**the end iterator**).
  * 注意1: 尾元素,是指最后一个元素; 尾后位置没有元素, 它用于表示容器没有元素的位置.
  * 注意2: `const`对象的`begin`和`end`返回迭代器的类型是`const_iterator`, 非`const`对象返回`iterator`类型
* `cbegin`(C++11): 与`begin`的区别是返回的迭代器类型必定是`const_iterator`
* `cend`(C++11): 与`end`的区别是返回的迭代器类型必定是`const_iterator`

## 4.2迭代器操作

> 指针也支持以下操作: 含义相同, 返回类型不同. [指向数组元素的指针与迭代器](#指向数组元素的指针是迭代器)

### 标准容器迭代器支持的操作

| 操作             | 描述                                                         |
| ---------------- | ------------------------------------------------------------ |
| `*iter`          | 返回迭代器`iter`所指元素的引用; 解引用无效或尾后迭代器是未定义的 |
| `iter->men`      | 等于`(*iter).men`; 返回`iter`所指元素的成员`men`, 适用于元素属于类类型 |
| `++iter`         | 增加`iter`, 是指使其指向容器的下一个元素                     |
| `--iter`         | 减少`iter`, 是指使其指向容器的上一个元素                     |
| `iter1 == iter2` | 判断两者是否相等; 若两者指向同一个对象或者同一个容器的尾后, 则相等 |
| `iter1 != iter2` | 与上相反; 不指向同一对象或尾后, 则不相等                     |

### `vector`和`string`迭代器支持的操作

`vector`和`string`迭代器支持的额外操作, 分为两部分

* 可使迭代器每次移动跨过多个元素
* 关系运算

| 操作            | 描述                                                         |
| --------------- | ------------------------------------------------------------ |
| `iter + n`      | 返回一个迭代器, 表示前进n个元素的位置                        |
| `iter - n`      | 返回一个迭代器, 表示后退n个元素的位置                        |
| `iter1 += n`    | 等于`iter1 = iter1 + n`                                      |
| `iter1 -= n`    | 等于`iter1 = iter1 - n`                                      |
| `iter1 - iter2` | 返回它们之间的距离, 也就是说返回值加上`iter2`, 得到`iter1`; 两迭代器属于同一容器 |
| `>, >=, <, <=`  | 若`iter1 - iter2`的值为负值, 则`iter1 < iter2`, 以此类推; 两迭代器属于同一容器 |

`iter1 - iter2`的返回类型是`difference_type`, 属于带符号整数类型.

# 5.数组

数组是一种固定大小的容器; 数组是内置的复合类型. 

## 5.1定义和初始化内置数组

数组的声明符形式是: `a[d]`; 其中, `a`是被定义的名字, `d`是数组的尺寸(**dimension**/**size**);

* 数组尺寸指定了数组有多少个元素, 因此**尺寸必须大于0**.
* 数组尺寸是数组类型的一部分, 因此尺寸必须在编译时是已知的, 即**尺寸必须是一个常量表达式**.

```c++
unsigned cnt = 42;			//not a constant expression
constexpr unsigned sz = 42;	//constant expression
int arr[10];				//array of ten ints
int *par[sz];				//array of 42 pointers to int
string strs[get_size()];	//ok if get_size is constexpr,error otherwise
string bad[cnt];			//error: cnt is not a constant expression
```

> 一些编译器允许用非常量表达式作为数组的尺寸, 这是属于编译器扩展(**compiler extension**); 
>
> 一般来说, 最好避免使用非标准特性, 因为含有非标准特性的程序可能在其他编译器上无法正常工作.

注意

* 默认情况(即无初始符)下, 数组元素被默认初始化
* 不允许使用`auto`推导数组元素的类型

### 数组与列表初始化

可以对数组的元素进行列表初始化; 此时, 允许忽略数组的尺寸; 如果要指定尺寸, 数组的尺寸必须大于等于列表中初始符的数目; 若指定的尺寸大于初始符的数量, 则多余的元素将被值值初始化.

```c++
const unsigned sz = 3;
int ial[sz] = {0, 1, 2};	//array of three ints
int a2[] = {0, 1, 2};		//array of three ints
int a3[5] = {0, 1, 2};		//array of five ints with value 0, 1, 2, 0, 0
int a4[2] = {0, 1, 2};		//error: too many initializers
```

### 特殊的字符数组

可以用字符串字面值对字符数组初始化; 注意: 字符串字面值的结尾处还有一个空字符`\0`, 它也会用于初始化.

```c++
//The dimension of a1 is 3; the dimension of a2 and a3 are both 4.
char a1[] = {'C', '+', '+'};		//list initialization
char a2[] = {'C', '+', '+', '\0'};	//list initialization
char a3[] = "C++";					//null terminator added automatically
char a4[3] = "C++";					//error: no space for the null!
char a5[3] = {};					//array of three char with value 0, 0, 0
```

### 数组间不允许拷贝或赋值

不允许将一个数组作为另一个数组的初始符, 也不允许将一个数组赋值给另一数组

```c++
int a[] = {0, 1, 2};	//array of three ints
int a2[] = a;			//error: cannot initialize one array with other
a2 = a;					//error: cannot assign one array to another
```

> 一些编译器允许数组的赋值; 这是编译器扩展(**compiler extension**);

### 如何理解复杂的数组声明

修饰符: 用于修饰名字的类型

* 默认情况下 `[ ]`修饰符比 ` * `修饰符, `&`修饰符先绑定
* 默认情况下类型修饰符从右向左依次绑定
* 可以使用小括号改变顺序.

```c++
int *ptrs[10]; 			 //ptrs is an array of ten pointers to int
int &refs[10] = /* ? */; //error: no arrays of references
int (*Parray)[10] = &arr;//Parray points to an array of ten ints
int (&arrRef)[10] = arr; //arrRef refers to an array of ten ints
```

## 5.2访问数组的元素

两种方式: `范围for`语句和下标运算符

使用下标运算符的注意事项

* 如果把一个变量作为数组下标, 该变量的类型通常被定义为`size_t`类型; 
  * 被定义在`cstddef`头文件中的 `size_t`类型是一个机器相关的无符号整数类型, 它被设计为有足够大的范围来表示内存中任意对象的大小.
* 程序员应该检查并确保下标值在范围之内, 即要满足这样的条件: 大于等于0, 同时小于数组尺寸

## 5.3数组和指针

### 数组可能转换成指针

数组的特性: 在很多使用数组的场合, 编译器自动将数组替换指针, 该指针指向数组的第一个元素.

* 在大多数表达式中, 数组对象被替换为指向首元素的指针; 

```c++
string nums[] = {"one", "two"};
string *p1 = &nums[0];			//p1 points to the first element in nums
string *p2 = nums;				//equivalent p2 = &nums[0]
auto var(nums);					// var is an string* that point to the first element
```

不会发生上述转换的场合:

* 使用`decltype`时: 假设`arr`是数组类型, `decltype(arr)`返回数组类型.
* 使用`sizeof`运算符时: 返回数组的字节数
* 更多

### 指向数组元素的指针是迭代器

>  [迭代器操作](#4.2迭代器操作)

指向数组元素的指针是数组的(概念上的)迭代器; 该指针支持的操作, 与`vector`迭代器支持的操作一样.

* 实际上, 所有指针支持的操作, 与`vector`迭代器支持的操作一样

获得指向首元素或尾后元素的指针:

1. `pb = arr`, 指向首元素的指针; `pe = &arr[size]`, 指向尾后元素的指针, `size`为数组的尺寸.
   * 数组名会自动转换成指向首元素的指针; 可以对数组尾元素后的"元素"取地址, 因为编译器不会检查数组下标. 而编译器不会检查数组下标的原因是, 编译器在一般情况下无法发现该下标是否有效.
   * 不应该解引用或递增指向尾后元素的指针
2. (C++11)定义在`iterator`头文件中的`begin`和`end`函数: `begin`返回指向首元素的指针, `end`返回指向尾后元素的指针.

指针运算

* `ptr + n`, `ptr - n`: 返回的指针相比与`ptr`相比前进/后退`n`个元素的位置
* `ptr1 - ptr2`: 返回两者指向的元素的距离(索引差); 返回类型为`ptrdiff_t`, 是机器相关的带符号整数类型, 定义在`cstddef`头文件.
* 关系运算符`>, >=, <, <=`: 指向同一数组的指针比较才有意义.

### 数组与指针的下标操作

指针支持下标操作

* `p[1]`等于`*(p+1)`

当我们对数组进行下标操作时, 实际上编译器将数组转换成指针, 再对指针进行下标操作.

```c++
int ia[2] = {};
int i = ia[0];
//实际上的行为如上
int *p = ia;		//首先将数组替换为指针
i = *(p + 2);		//然后对指针进行下标操作
```

数组(指针)的下标操作与`vector`/`string`的下标操作的区别: 

* 数组/指针的下标操作是内置操作, 下标类型是带符号的.
* `vector`和`string`的下标类型是类定义的操作, 下标类型是无符号的.

## 5.4 C风格字符串

> 尽管C++支持C风格字符串, 不建议在C++程序中使用C风格字符串. C风格字符串不仅使用不方便, 而且极易引发漏洞, 同时是许多安全问题的根本原因

C风格字符串(**C-style character strings**)是C++从C继承而来的通用结构; 字符串字面值是该结构的实例.

C风格字符串, 存放在字符数组中并空终止(**null terminated**). 空终止是指在字符串最后一个字符后面跟着一个空字符(`\0`), 使空字符代表字符串的结束.

通常使用指针来操作C风格字符串.

注意:

* C风格字符串不是类型, 是用于表示字符串的结构, 是表达和使用字符串的规范(convention).

### C库字符串函数(C Library String Functions)

C标准库定义了一组用于操作C风格字符串的函数; 这些函数定义在`cstring`头文件中.

> 注意: 以下所列的函数不检查其字符串参数

C风格字符串函数

| 函数             | 描述                                                         |
| ---------------- | ------------------------------------------------------------ |
| `strlen(p)`      | 返回`p`的长度, 空字符不计算在内                              |
| `strcmp(p1, p2)` | 比较`p1`和`p2`的相等性.                                      |
|                  | 如果`p1==p2`, 返回0; 如果`p1>p2`, 返回一个正值; 如果`p1<p2`, 返回一个负值 |
| `strcat(p1, p2)` | 将`p2`附加在`p1`之后, 返回`p1`                               |
| `strcpy(p1, p2)` | 将`p2`拷贝到`p1`, 返回`p1`                                   |

`strcat(p1, p2)`和`strcpy(p1, p2)`

* 调用这两函数时, 调用者需要确定`p1`是有足够空间来存放生成的C风格字符串.

## 5.5与旧代码的接口

现代的C++程序有时候要与那些使用数组和C风格字符串的代码衔接. 为了简化这些工作, C++专门提供了一组功能

* `string`和C风格字符串
* `vector`和数组

### `string`和C风格字符串*

以下功能使得: 在需要`string`的地方, 可以使用C风格字符串.

* 允许使用空终止的字符数组, 初始化`string`对象或给`string`对象赋值;
* 允许使用空终止的字符数组, 在`string`对象的加法运算中作为其中一个操作数;
* 允许使用空终止的字符数组, 在`string`对象的复合赋值运算(`+=`)中作为右操作数;

当需要一个C风格字符串, 而有一个`string`对象时, 使用`string`对象的`c_str`函数成员

* `s.c_str()`返回一个`const char*`类型的指针, 指向一个空终止的字符数组; 该字符数组表示的字符串与`s`表示的字符串相同.

```c++
string s ("Hello, World!");
char *str = s;			//error: can't initialize a char* from a string
const char *str = s.c_str(); //ok
```

* 如果程序需要一直使用`c_str()`返回的数组, 程序最好将该数组拷贝一份.

### `vector`和数组

可以使用数组来初始化`vector`, 只要

* 初始符为**拷贝区域**的首元素地址和尾后地址.

```c++
int int_arr[] = {0, 1, 2, 3, 4, 5};
vector<int> ivec(begin(int_arr), end(int_arr));
vector<int> subVec(int_arr + 1, int_arr + 4);
```

# 6.多维数组*

严格来说, C++中没有多维数组. 通常所说的"多维数组"其实是数组的数组(arrays of  arrays).

要定义一个这样的数组---数组的元素是数组, 需要提供两个维度来定义它: 数组本身的维度, 还有它元素的维度. (维度, 是指数组的尺寸/长度/size/length)

> 日常所说的一维, 二维, 三维等等中的数字, 是指维度的个数, 即有多少个维度.

```c++
//array of size 3; each element is an array of size4
int ia[3][4];
//array of size 10;
//each element is a 20-element array whose elements are array of 30 ints
int arr[10][20][30];
```

"[如何理解复杂的数组声明](#如何理解复杂的数组声明)"中有说明如何理解声明;

## 多维数组的初始化

列表初始化: 使用`{ }`形式提供初始符

```c++
//嵌套花括号, 便于查看
int ia[3][4] = {
    {0, 1, 2, 3},
    {4, 5, 6, 7},
    {8, 9, 10, 11}
};
//没有嵌套的花括号, 等价于上面的初始化语句
int ia[3][4] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
```

类似于一维数组, 可以只提供部分元素的初始符, 其他未列出的元素执行值初始化. 如果忽略嵌套花括号, 结果可能会不一样.

```c++
int ia[3][4] = {{0}, {4}, {8}};		//显式初始化各行的首元素, 其他元素值初始化
int ix[3][4] = {0, 1, 4, 8};		//显式初始化第一行, 其他元素值初始化
```

## 多维数组的下标操作

要使用下标操作来访问多维数组的底层元素, 每个维度需要一个下标运算符

多维数组, 其实就是数组的数组; 也就是说, 多维数组使用下标操作得到的元素也是数组, 得到的元素可以再次使用下标操作,直到得到的对象不支持下标操作为止. 所以, 多维数组的每个维度需要一个下标运算符, 才能得到底层的非数组类型的对象.

## 多维数组与范围for*

只使用`auto`, 初始符为数组时, 数组将转换为指向首元素的指针; 若使用`auto &`推导, 可得到一个指向数组的引用.

```c++
int ia[3] = {1, 2, 3};
auto var1 = ia;			// var1 is a int*
auto &var2 = ia;		// var2 is a reference to ia
```

使用`范围for`语句遍历多维数组

* 除了最内层循环的控制变量外, 其他循环的控制变量必须声明成引用类型

```c++
int ia[3][4] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
for (const auto &row : ia)
    for (auto col : row)
        cout << col << endl;
```

* 如果外层的控制变量不声明为引用类型, 在初始化控制变量时, 因为初始符为数组类型(外层的元素都是数组类型), 作为初始符的数组对象将转换为指针, 控制变量也成为指针类型; 显然后面的循环就不合法了.

## 多维数组与指针

与数组一样, 当我们使用多维数组的名字时, 也会自动将其转换成指向数组首元素的指针; 具体来说, 转换而来的指针是指向第一内层数组的指针.

```c++
int ia[3][4];
//p1 points to an array of four ints; the parenthses are essential
int (*p1)[4] = ia;	
auto p2 = ia; 		//ok: p2 == p1; p2 points to an array of four ints

```
