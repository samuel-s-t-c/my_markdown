# 总结

* 关键词: 内置类型, 变量, 复合类型, 类
* 类型: 定义存储要求和所能执行的操作
  * 原始内置类型: 算术类型
  * 复合类型: `const`, 引用, 指针
  * 类
* 变量
  * 定义和声明
  * 初始化
  * 作用域

[toc]

# 1. 原始内置类型
类型，决定数据的意义和可执行的操作。
* 简单的语句，如`i = i + j`，表达的意思依赖于`i`和`j`的类型；如果类型都为`int`，这条语句执行算术的加法运算；如果类型都为`string`，则是将两条字符串接起来。

原始内置类型，有算术类型（**arithmetic type**）和空类型（**void**）；

* 空类型不对应任何的数值，仅用于一些特殊的场景，如当函数不返回数值时，空类型作为函数的返回类型.

> C++中的内置类型直接体现计算机硬件的能力, 如数字和字符.

## 1.1算术类型*
算术类型分为：整型（包括字符类型和布尔类型）和浮点型
### 1.1.1整数类型
#### 一般的整型
* `short, int, long, long long`
* 标准保证了这样的尺寸关系：`short <= int <= long <= long long`
#### 布尔类型
* `bool`类型的取值是真true或假false
#### 字符类型
* `char`类型要确保可以存放该机器的基本字符集，所以`char`类型的大小等于该机器的字节。
* 扩展字符类型：`wchar_t, char16_t, char32_t`
    * `wchar_t`类型要确保可以存放该机器的最大扩展字符集的任意字符
    * `char16_t` ，`char32_t`用于Unicode字符集
### 1.1.2浮点数类型
* 浮点型可表示为单精度，双精度和扩展精度值，对应的类型为`float, double, long double`
* 通常（1byte = 8 bit，1 word = 4 byte），`float`的大小为一个字（32位），`double`的大小为两个字，`long double`的大小为3或4个字
* 通常，`float`有7个有效位，`double`有16个有效位
* `long double`类型一般用于有特殊浮点需求的硬件，它的精度也因具体实现而变化。
### 1.1.3尺寸
C++标准规定了尺寸的最小值
![算术类型](C:\Users\c\Documents\my_markdown\c++primer\assets\算术类型.png)

### 1.1.4有符号类型和无符号类型
#### 整型
除了`bool`和扩展字符类型，其他的整数类型可以分为有符号的（**signed**）和无符号的（**unsigned**）。
* `short, int, long, long long`都是有符号的；在类型名前添加`unsigned`就能得到对应的无符号类型，如`unsigned long`，`unsigned int` 可以写成`unsigned`。
#### 字符型
* `char, unsigned char, signed char`是三种不同的字符类型；`char`不等于`signed char`，也不等于`unsigned char`。`char`实际表示为带符号或无符号的，由编译器决定。
## 1.2类型转换*
类型转换，将对象从一种给定的类型转换（**convert**）为另一种相关的类型。
* 在程序的某处，我们使用一种类型而其实对象应该为另一种类型时，将自动进行类型转换。
### 1.2.1算术类型的类型转换
* 非布尔类型=>布尔类型；值为0时，转换为`false`，其他情况转换为`true`。
* 布尔类型=>非布尔类型; 值为`true`时, 转换为1; 值为`false`时, 转换为0
* 浮点类型=>整型; 将值截断, 只保留小数点之前的部分
* 整数类型=>浮点类型; 小数点部分记为0; 如果该整数所占的位数比该浮点类型的位数多时, 精度可能有损失. 
* 超出范围的值=>无符号类型; 初始值对<u>该类型可表示数值的总数</u>取模后的余数.
* 超出范围的值=>有符号类型; 结果是未定义的

## 1.3字面值常量literals
### 1.3.1整数字面值常量Integer literals
* 整数字面值常量的三种表达形式
    * 十进制decimal notation: 20
    * 八进制octal notation, 以`0`开头的整数: 024
    * 十六进制hexadecimal notation, 以`0x`或`0X`开头的整数: 0x14 0X14

* 整数字面值常量的类型
    * 默认情况下, 十进制字面值是有符号的; 而八进制和十六进制字面值因实现而异, 可能为有符号的, 也可能为无符号的.
    * 默认情况下, 十进制字面值是`int`, `long`和`long long`中能够容纳该值且尺寸最小的类型; 八进制和十六进制字面值是`int, long, unsigned long, long long, unsigned long`中能够容纳该值且尺寸最小的类型.
        * 如果字面值比所有相关的类型中的最大值还大, 将产生错误.
        * `short`类型没有字面值
    * 可用后缀明确指定类型

* 严格来说, 整数字面值常量没有负数; 如`-24`看似负十进制字面值, 实质上那个负号不在字面值之内, 它的作用是将`24`字面值取负值.
### 1.3.2浮点数字面值Floating-point literals
* 浮点数字面值的两种表达形式
    * 十进制小数: 3.14159, 0., .001
    * 以科学计数法表示的指数, 指数部分用`E`或`e`标识: 0e0 1E2
* 浮点数字面值的类型
    * 默认为`double`
### 1.3.3字符和字符串字面值Character and Character String Literals
* 字符字面值
    * 表达形式
        * 由单引号括起的一个字符: 'a'
    * 类型
        * `char`
* 字符串字面值
    * 表达形式
        * 由双引号括起的零个或多个字符: "" "Hello, World!"
    * 类型
        * 由常量的`char`构成的数组
    * 注意
        * 编译器在每个字符串的结尾处添加一个空字符('\0'), 因此长度比它的内容多1
        * 如果两个字符串字面值位置紧邻且仅有空格, 缩进和换行符分隔, 它们实际上是一个整体.
#### 转义序列Escape Sequences
不能直接使用的字符
* 不能打印的(nonprintable)字符, 如退格backspace或其他控制字符, 是没有可视的图案.
* 有特殊含义的字符, 如引号, 双引号, 反斜线

转义序列, 用来表示这样的字符
* 反斜线+字符
    * 换行符newline: `\n`
    * 反斜线: `\\`
* 泛化的转义序列, 可表示所有字符:`\x`+十六进制数字或八进制数字
    * 换行符: `\12`
    * 空格: `\40`
    * 字符`M`: `\x4d`
### 1.3.4指定字面值的类型
![指定字面值类型](C:\Users\c\Documents\my_markdown\c++primer\assets\指定字面值类型.png)例子:

```C++
L'a'         // 宽字符型字面值, 类型是wchar_t
42ULL    // 无负号整型字面值, 类型是unsigned long long
1E-3F    // 单精度浮点型字面值, 类型是float
```
### 1.3.5布尔和指针字面值Boolean and Pointer Literals
* 布尔字面值
    * `true`和`false`
* 指针字面值
    * `nullptr`是一个指针字面值
# 2.变量variables
* 对象(Object), 有多种含义
    * 在通用情况下, 对象是指一块能存储数据并具有某种类型的内存空间(**本书使用这种含义**)
    * 在面对对象编程的角度, 对象是指某种类的实例
    * 在是否可写的角度, 对象是指能被程序修改的数据, 而值是指只读的数据
* 变量, 是指已经命名的对象
## 2.1定义变量*
变量定义的基本形式
* 类型说明符(**type specifier**)+一个或多个变量名(以逗号分隔)+分号
    * 定义时可以为变量赋初始值
    * 例子: `int sum = 0, value, units_sold = 1;`
## 2.2初始化Initialization
* 对象被初始化(initialized), 是指该对象在创建时获得一个特定的值.
```c++
int a;            
int i = 0;      // 拷贝初始化
int j = {0};	// 列表初始化
int k {0};   	// 列表初始化
int l (0);     	// 直接初始化
```
* 注意
    * 初始化(**initialization**)和赋值(**assignment**)是两回事, 虽然两者的区别在多数情况下是无关紧要.
    * 初始化是在创建变量时给其一个值, 而赋值是将变量的当前值擦除, 并替代为新值
### 2.2.1初始符/初始值Initializers
* 初始符可以为复杂的表达式, 字面值等
    * 在同一条定义语句中, 可以用前面定义的变量值作为后面定义的变量的初始值. 如, `int i = 1, j = i, k = i * j;`
### 2.2.2列表初始化List initialization(c++11)
用**花括号curly braces**进行初始化, 称为列表初始化
* 当列表初始化用于内置类型时, 如果初始符存在丢失信息的风险, 编译器将报错
```c++
long double ld = 3.1415926536;
int a{ld};                   //错误: 存在丢失信息的风险
int b = {ld};                //错误: 存在丢失信息的风险
int c(ld), d = ld;           // 无报错, 将执行类型转换, 并丢失部分值
// 使用long double的值初始化int变量时可能丢失数据;
// 一是, 小数部分丢失; 二是, int可能存不下ld的整数部分
```
### 2.2.3默认初始化Default initialization
当定义一个变量时, 没有提供初始值, 该变量会被**默认初始化**; 此时变量被赋予一个默认值, 而默认值由<u>变量类型</u>和<u>定义变量的位置</u>决定.
* 内置类型+函数体外: 变量被初始化为0 
* 内置类型+函数体内: 变量不被初始化, 且值是未定义的; 
    * 例外情况: 见6.1.1(p205)
* 类控制如何初始化对象, 包括能否默认初始化, 和初始值是多少等;
## 2.3变量声明和定义的关系*
* 背景
    * C++支持分离式编译(**separate compilation**)机制; 该机制允许将程序分隔为若干个文件, 每个文件可被独立编译.
* 声明(**declaration**)与定义(**definition**)
    * 为了支持*separate compilation* , C++明确区分了声明和定义这两个概念;
        * 声明,使得名字为程序所知/A declaration makes a name known to the program;
        * 定义,创建与名字关联的实体/ A definition creates the associated entity;
* 变量声明和定义
    * 概念
        * 变量声明: 指定变量的类型和名字
        * 变量定义: 指定变量的类型和名字, 并分配存储空间, 也可能赋予一个初始的值initial value
    * 区别
        * 变量定义是变量声明; 变量声明不一定是变量定义;
        * 任何包含显式初始符的变量声明, 就是变量定义
    * `extern`关键字
        * 要声明而不定义变量时, 在变量名前添加`extern`, 然后不要显式提供初始符,
            * 如 `extern int i; int extern j;`
        * `extern`语句包含初始符
            * 在函数体外, 变成一个定义
                * `extern int k = 0; //这是定义, 有初始符`
            * 在函数体内, 将引发错误
    * 注意
        * 变量只能被定义一次, 但是可以被声明多次
        * 在不同文件中使用同一个变量时,
            * 一是, 必须在一个文件中定义该变量;
            * 二是, 在需要使用该变量的文件中, 声明而不定义变量;
## 2.4标识符identifiers
标识符, 是指用于标识名字的字符序列
* 标识符(**identifiers**)的规则
    * 由字母, 数字和下划线组成; 
    * 长度没有限制
    * 必须以字母或下划线开头; 即, 不能以数字开头
    * 字母大小写敏感case-sensitive
    * 用户自定义的标识符中, 不能连续出现两个下划线, 不能以下划线紧接大写字母开头, 在函数体外的标识符不能以下划线开头
        * 如`a__b`, `_Ab`
* C++保留的名字, 不能用于标识符
    * 关键字Keywords
        * ![keywords](C:\Users\c\Documents\my_markdown\c++primer\assets\keywords.png)    
    * 操作符替代名
        * ![alternative_operator_names](C:\Users\c\Documents\my_markdown\c++primer\assets\alternative_operator_names.png)
* 标准库保留的名字, 建议不要用于标识符, 如`cin`, `cout`等
### 2.4.1变量的命名习惯
提高程序可读性的习惯
* 标识符应该体现实际含义
* 变量名一般用小写字母
* 类名一般以大写字母开头
* 如果标识符由多个单词构成, 则单词间应该有明显区分, 如`student_loan`, `studentLoan`
## 2.5名字的作用域
* 背景
    * 在程序的任何位置, 每个使用的名字都会指向一个特定的实体: 变量, 函数, 类型等; 然而, 同一名字如果出现在不同位置, 可能不是指向同一实体
* 作用域
    * 作用域(**scope**), 是指名字有特定含义的区域;
        * 在不同的作用域, 同一名字可以指向不同的实体; 从名字的声明语句开始, 直到所在的作用域末端, 该名字都是可见的(**visible**)/有效的;
* 分类
    * 全局作用域(**global scope**)
        * 位置: 于所有花括号之外
    * 块作用域(**block scope**)
        * 位置: 于函数体内
### 2.5.1嵌套作用域Nested scope
* 一个作用域可以包含另一个作用域;
    * 内层作用域(**inner scope**), 是被包含(被嵌套)的作用域
    * 外层作用域(**outer scope**), 是包含其他作用域的作用域
* 情况
    * 名字只声明于外层作用域: 名字在内外层作用域指向同一实体
    * 名字声明于外层作用域, 然后重定义于内层作用域: 名字指向不同实体
        * `::`作用域操作符: 左操作数为作用域, 如果左操作数为空, 则指定全局作用域
# 3.复合类型*
* 复合类型, 是指基于其他类型定义的类型.
    * C++的复合类型有引用, 指针等
* 声明语句, 是由一个基本类型(**base type**)和声明符(**declarator**)列表组成的
    * 基本类型, 是指在声明语句中, 类型说明符和(可选的)限定符(如`const, constexpr, typedef`); `(qulifier)+type specifier` 如`const std::string, const int, int, std::string`
    * 声明符: 是指在声明语句中, 由变量名, 零个或多个类型修饰符(**modifier**), 和零个或多个类型限定符(**qualifier**), 用来命名一个变量, 并指定一个与基本类型有关的某种类型; 
## 3.1引用Reference*
* 引用有两种

  * 左值引用**lvalue reference**; 从技术上来说, 术语"reference"是指"lvaule reference";
  * 右值引用**rvalue reference**, 常用于内置类(**inside class**);

* 什么是引用

  * 引用, 定义了一个对象的另一个名字; 
  * 一般来说, 在初始化一个变量时, 初始符的值被拷贝到新键的对象中; 而定义一个引用时, 程序把引用和初始符绑定(**bind**)在一起. 一旦初始化完成, 引用将一直和它初始对象绑定在一起, 无法令引用和其他对象绑定. 因此,  引用必须初始化

* 如何定义一个引用类型

  * 将声明符写成`&d`的形式;

  * ```C++
    int ival = 1024;
    int &refVal = ival;		// refVal引用了ival, refVal指向ival的实体
    int &refVal2;			// 报错: 引用必须被初始化
    ```
* 注意
  * 除了两个例外情况(s 2.4.1 p.61 和 s 15.2.3 p601), 其他所有引用的类型必须和初始符的类型严格匹配	
  * 引用只能与对象绑定, 不能与字面值或表达式的结果绑定	
### 3.1.1引用等于别名*

> 引用不是对象. 引用是一个已经存在的对象的别名

定义一个引用之后, 对引用进行的操作都是对与其绑定的对象进行的.

```C++
refVal = 2;					// refVal是ival的引用, 所以这是将2赋予ival
std::cout << ival			// 输出: 2
```

因为引用本身不是对象, 所以不能定义一个引用的引用

### 3.1.2引用的定义Reference Definitions

* 声明符以`&d`的形式来定义, 即类型修饰符`&`后接变量名
* 可以在一条语句中定义多个引用

```C++
int i = 1024, i2 = 2048, &r = i, &r2 = i2;
int &ri = r 	// r是对i的引用, 所以ri的初始值是i, ri也是对i的引用
```

## 3.2指针Pointers*

* 什么是指针
  * 指针, 是指向(point to)对象的复合类型; 指针, 是放存某个对象的地址
* 与引用的不同点
  * 指针是一个对象
  * 指针可以被赋值和拷贝
  * 指针不是和被指对象绑定在一样, 指针可以指向不同的对象;
  * 指针允许在定义时不初始化
* 与其他内置类型一样, 指针在块作用域定义时未初始化, 指针的值是未定义的

### 3.2.1 定义指针

* 获取对象的地址
  * 在对象前使用取地址符(**address-of operator**), `&`;

* 声明符以`*d`的形式, 即类型修饰符`*`后接变量名

```c++
int i = 0;				
int *ip1, *ip2 = &i;
```

* 注意
  * 因为引用不是对象, 它没有地址, 所以不能定义一个指向引用的指针
  * 除了两个例外(s2.4.2 p62, s15.2.3 p601), 其余情况下, 指针的类型和对象的类型要严格匹配

### 3.2.2 指针值

* 指针的值(即地址)有4种情况
  * 指向一个对象
  * 指向一个位置, 该位置紧邻着一个对象所占的空间
  * 空指针, 意味着指针没有指向任何对象
  * 无效指针, 也就是上述情况之外的值

### 3.2.3 使用指针访问对象

* 使用解引用符(**dereference operator**, the `*` operator), 可以获得指针指向的对象

```c++
int ival = 42;
int *p = &ival;
std::cout << *p;	//输出: 42
*p = 0;
std::cout << *P;	//输出: 0
```

### 3.2.4 空指针Null Pointers

* 空指针不指向任何对象; 在使用一个指针之前, 可以编写代码来检查它是否为空

* 获得空指针的方法

  * ```C++
    int *p1 = nullptr;			//字面值nullptr
    int *p2 = 0;				//字面值0
    // must #include cstdlib
    int *p3 = NULL;				//预处理器变量NULL
    ```

  * `nullptr`字面值是一种特殊类型的字面值, 它可以转换为任意其他的指针类型

  * `NULL`预处理器变量, 在`cstdlib`头文件中被定义为`0`;

### 3.2.5 `void*`指针

* `void*`类型是一种可以存放任意对象的地址的类型; `void*`指针没有保存指向的对象的类型
* `void*`指针可以进行的操作
  * 与其他指针比较
  * 从函数传入或返回`void*`指针
  * 将`void*`指针的值赋予另一个`void*`指针
* 不能直接操作`void*`指针所指的对象

### 3.2.6 其他指针操作

* 将指针用在条件表达式中,
  * 指针为空指针, 则将其转换为`false`
  * 指针为非空指针, 则将其转换为`true`
* 使用比较操作符比较同一类型的指针
  * `==`和`!=`: 如果两指针存放的地址值相同, 则两指针相等; 反之, 它们不相等;
    * 两指针存放的地址值相同有三种可能: 都为空, 都指向同一个对象, 或者指向同一个对象的下一地址
  * 其他的比较操作符: 结果都为`false`

## 3.3理解复合类型的声明*

* 在变量的定义语句中, 有一个基本类型和一组声明符; 每个声明符的形式都可以不同; 也就是说, 一条定义语句可以定义出不同类型的变量
  * `int i = 1024, *p = &i, &r = i;`
* **type modifiers**类型修饰符`*`和`&`; 
  * 类型修饰符只在声明符内起作用; 
    * 如`int* i, j;`, `int`为基本类型, `*i`和`j`为声明符, `*`修饰符在所在的声明符内起作用, 不影响其他声明符;
  * 在一个声明符中可以有零个或多个类型修饰符
    * Pointers to pointers指向指针的指针;  如`int **i;`
      * 指针是对象, 指针指向对象; 那么, 可以将(某个)指针的地址存放在(另一)指针中,  
    * References to pointers指向指针的引用; 如`int *&i;`
      * 指针是对象, 引用指向对象; 所以, 引用可以指向指针
      * 引用不是对象; 所以, 指针不能指向引用, 如`int &*i;`是错误的
* 理解复合类型的声明的步骤
  * 将声明语句分为两部分: `基本类型`和`声明符组`,
  * 在`声明符组`中, 逐一分析每个`声明符`
  * 在`声明符`中,  从`标识符`开始, 找到最近的`修饰符`从而确定最高层的类型, 重复如此;
    * `int **&r;`  a reference to a pointer which points to another pointer
    * `int`是基本类型, `**&r`是声明符; 在声明符中, `r`是标识符, 最近的修饰符是`&`, 那么可以确定变量`r`是一个引用类型; 下一个修饰符是`*`, 确定是一个指向指针的引用; 最后一个修饰符是`*`, 可得变量`r` 的类型是一个引用类型, 指向一个指针, 而这个指针是一个指向指针的指针; 

# 4.`const`限定符Qualifier*

* `const`限定符: 

  * `const`对象一旦被创建后其值不能再改变, 所以`const`对象必须初始化

  * ```c++
    const int bufSize = 512;	
    bufSize = 512;				//错误: 试图向const对象写值
    const int k;				//错误: k是一个未初始化的const常量
    ```

* 说明

  * `const`可译为`常量`
  * 不能对`const`对象执行修改其内容的操作, 如赋值assignment; 初始化不属于修改对象的操作;
  * 值是不考虑`const`ness(不变性)的; 即`const`对象的值不是`const`;

### 4.0.1 默认状态下, `const`对象仅在文件内有效

* 背景
  * 当以compile-time constant为初始化一个`const`对象时,  编译器将在编译过程中将用到该变量的地方都替换成对应的数值;
    * 如`const int bufSize =512;`, 编译器将所有用到`bufSize`的地方替换成`512`;

* `const`和变量定义

  * 为了支持这用法, 同时避免对同一变量的重复定义, 在默认情况下, `const`对象仅在文件内有效

  * 定义一个能在文件间共享的`const`变量, 但它的初始符不是`const expression`:

    * 在定义和声明时都使用`extern`

    * ```c++
      //file_1.cc 
      extern const int bufSize = fcn();
      //file_1.h
      extern const int bufSize;
      ```

## 4.1对`const`的引用

* 对常量的引用(**reference to const**), 是指一个引用, 它指向一个`const`类型的对象;
  * 实际上可以指向一个非`const`对象; 
* 关于术语: `const` reference is a reference  to `const`; 
  * 有时我们将"对常量的引用a reference  to `const`"缩写成"常量引用`const` reference"
  * 对常量的引用 == 对`const`的引用 == a reference to `const`  
  * 常量引用 == `const`引用 == `const` reference
* `const`引用中的`const`只限制该引用的操作, 不影响被引用的对象; 

### 4.1.1 初始化和`const`引用

* 关于初始化引用的规则

  * 除了两个例外, 引用的类型必须与其所引用对象的类型一致

* 例外之一:

  * 在初始化`const`引用时, 允许使用这样的初始符, 该初始符的类型能够转换成引用的类型;

* `const`引用绑定不同类型的对象,

  *  除顶层`const`之外都相同的类型的对象

    * 直接与底层对象绑定

  * 其他的类型不同的对象

    * 当`const`引用绑定不同类型的对象时, 编译器实际将`const`引用绑定到一个临时量(**temporary**)对象; 而该临时量是一个未命名的对象, 用来存放类型转换后的初始符的值;
    * 使用`const`引用的意图是期待不会改变该值, 所以以上做法是合理的
    * 如果`const`引用指向不同类型的对象, 这时该对象的值发生改变, `const`引用的值是不变的; 因为`const`引用实际指向一个临时量;

  * ```c++
    double dval = 3.14;
    const int &ri = dval;
    // 以上代码经过编译后的伪代码
    const int temp = dval;
    const int &ri = temp;
    ```

## 4.2指针和`const`*

* 对`const`的指针(**pointer to const**), 是指一个指向`const`类型的指针;

  * 实际上, 对`const`的指针可以指向非`const`对象;
  * 一个对`const` 的指针不能改变其所指对象的值

* "指针的类型必须和所指对象的类型匹配"的例外情况之一

  * 一个对`const`的指针可以指向非`const`对象
    * 与对`const`的引用一样, `const`只是限制了指针的操作, 不影响所指的对象

* 常量指针(**const pointer**)和对常量的指针(**pointer to const**)

  * 对`const`的指针, 是一个指针, 不能通过它来改变所指的对象;

  * `const`指针, 是指一个指针, 它的值不能改变;

    * 和其他`const`对象一样, `const`指针必须被初始化, 而且一旦初始化, 其值不能改变

    * 表示方法: 在`*`修饰符后接`const`

    * ```c++
      int errNumb = 0;
      int *const curErr = &errNumb;	// curErr will always point to errNumb
      const double pi = 3.14159;
      const double *const pip = &pi;	// pip is a const pointer to a const object
      ```

    * 简单而有效地解读声明`const double *const pip = &pi;`: **从变量名开始, 从右向左解读**; 

    * 首先遇到`*const`, 意味着`pip`是一个`const`指针; 然后, 基本类型是`const double`, 知道了`pip`指向的对象类型是`const double`

## 4.3顶层`const`*

* `const`的级别: `top-level`和`low-level`

  * 顶层`const`(**top-level const**), 是指`const`作用于类型的顶层, 表示一个对象自身是常量`const`

  * 底层`const`(**low-level const**), 用于复合类型,  相对于顶层`const`而言的, 是指`const`作用于类型的非顶层

    * ```C++
      // i是一个int, const作用于顶层"int"
      const int i;			
      // p是一个指针, 右边const作用于顶层"*", 左边const作用于底层
      const int *const p = &i;	
      ```

    * 以`const int *const p = &i;`为例: 

    * 变量`p`是一个指向`const int`的`const`指针; 在语句中, 靠右的`const`是`top-level`, 也就是说变量`p`是一个常量; 靠左的`const`是`low-level`,  也就是说`p`指向的对象是一个常量;

* 在拷贝一个对象时, 

  * 对象的类型有顶层`const`: 可以忽略该限定符`const`, 对拷贝无影响; 因为拷贝操作不会改变对象的值, 是否存在该限定符`const`不会影响该操作

    * ```C++
      int i = 0;
      const int ci = 42;
      i = ci;				// 正确: 顶层`const`可以忽略
      ```

  * 对象的类型有底层`const`: 不能忽略该限定符`const`, 拷贝操作的两个对象都必须有相同的底层`const`, 或者两者的类型能够转换; 一般来说, 非`const`可以转换为`const`, 反之不然

    * ```C++
      int i = 0;
      const int ci = 1;
      int *const p1 = &i;
      const int *p2 = &ci;
      const int *const p3 = p2;
      int *p = p3; 				// 错误: p3有底层const, p没有
      p2 = p3;					// ok: p2和p3有相同的底层const
      p2 = &i;					// ok: int* 可转换为 const int*
      int &r = ci;				// 错误: const int不能转换为 int
      const int &r2 = i;			// ok: int 可转换为 const int
      ```

## 4.4constexpr和常量表达式

* 常量表达式(**constant expression**): 其值不会改变; 其值可以在编译期间运算出来;

  * 如字面值, 初始符为常量表达式的常量对象

* 如何判断是否为常量表达式: 类型和初始符

  * 类型为`const`, 初始符的值能在编译时得出

  * ```C++
    const int max_files = 20;		// max_files是常量表达式; 类型是const, 初始符为字面值
    const int limit = max_files + 1;// limit是常量表达式
    int staff_size = 27;			// staff_size不是常量表达式
    const int sz = get_size();		// sz不是常量表达式; 初始符的值不能在编译时得到
    ```

### 4.4.1 `constexpr`变量(c++11)

* 背景
  * 在一个复杂系统中,  几乎不可能分辨一个初始符到底是不是常量表达式;  
* `constexpr`类型: 编译器会检验`constexpr`类型的变量是否为常量表达式
  * `constexpr`类型的变量的值是不能改变的, 所以必须初始化
* 注意
  * `constexpr`把它所定义的对象置为顶层`const`

#### 字面值类型

* 字面值类型(**literal type**): 能用于`constexpr`声明中的类型,  能够在编译时计算的类型
* 字面值类型有
  * 算术类型
  * 引用
  * 指针
  * 其他字面值类型(s7.5.6 p267, s 19.3 p736)

####  `constexpr`和指针

* `constexpr`指针的初始值必须是`nullptr`, `0`或者存储在固定地址的对象

  * 基于(s6.1.1 p204)的原因, 定义在函数中的变量不是存储在固定地址
  * 定义在任意函数体外的对象存储在固定地址, 且该对象的地址是常量表达式
  * (s6.1.1 p205)提到, 允许函数定义一类有效范围超出函数本身的变量, 这类变量有固定地址

* `constexpr`指针中的`constexpr`限定符仅对指针有效, 与指针所指的对象无关

  * ```C++
    const int *p = nullptr;			// p指向一个`const int`对象
    constexpr int *q = nullptr;		// q指向`int`对象, q是一个常量指针
    ```

  * ```C++
    int j = 0;
    constexpr int i = 42;
    // i and j must be defined outside any function
    constexpr const int *p = &i;	// p is a constant pointer to the const int i
    constexpr int *p0 = &i;			// Error: p0和初始符的底层const不同, 且无法转换
    constexpr int *p1 = &j;			// p1 is a constant pointer to the int j
    ```

# 5.处理类型

* 随着程序越来越复杂, 使用的类型也越来越复杂, 导致
  * 难以确定原本的含义
  * 难以确定需要什么类型

## 5.1类型别名

* 类型别名(**type alias**): 某种类型的替代名称, 使复杂的类型变得简单明了, 方便使用.

* 两种方式定义类型别名

  * `typedef`关键字

    * ```c++
      typedef double wages;	//wages是double的别名
      typedef wages base, *p;	//base是double的别名, p是double*的别名
      ```

    * `typedef`作为声明语句中基本类型的部分; 

    * 含有`typedef`的声明语句是在声明一个类型别名, 不是变量

  * 别名声明(**alias declaration**): **C++11**中的方法

    * 形式如`using alias = variable;`: 以`using`开头, 将等号左边的名字定义为右边变量的类型别名

#### 理解类型别名*

* 定义一个类型别名后, 该别名(名字)等同于类型说明符, 可以出现在类型说明符出现的地方;

* ```c++
  typedef char *pstring;
  const pstring cstr = 0;		// cstr is a constant pointer to char
  const pstring *ps;			// ps is a pointer to a constant pointer to char
  ```

  * 上述`pstring`是`char *`的别名; `pstring`出现在声明的基本类型中; 与以往一样, 基本类型中的`const`是对给定类型的修饰, 这里是对`pstring`的修饰; 那么, `const pstring`是一个指向`char` 的常量指针, 而不是指向`const char`的指针

## 5.2auto类型说明符(c++11)

* `auto`类型说明符

  * 编译器为`auto`变量从初始符推导出合适的类型, 因此`auto`变量必须有初始符;

  * 在一条定义语句中, 使用`auto`定义多个变量, 这些变量的基本类型是相同的

    * ```c++
      auto i = 0, *p = &i;	// i is int and p is a pointer to int;
      auto sz = 0, pi = 3.14; //error: inconsistent types for sz and pi
      ```

#### 复合类型和`auto`

* 编译器推断出来的类型有时候和初始符的类型并不完全一样, 因为编译器会适当地改变结果类型使其更符合初始化规则

* 引用和`auto`

  * `auto`不会推断出引用类型

    * 因为使用引用作为初始符, 实际上使用被引用的对象作为初始符; 

  * ```c++
    int i = 0, &r = i;
    auto a = r;			// a is an int, not a reference to int;
    ```

  * 可显式添加`&`, 得到引用类型, 此时不会忽略初始符的顶层`const`

  * ```c++
    auto &g = i;		// g is a int& that is bound to i
    auto &h = 42;		// error: we cant't bind a plain reference to a literal
    const auto &j = 42	// ok: we can bind a const reference to a literal
    const int ci = i;
    auto &m = ci;		// m is a const int&;
    ```

* `const`和`auto`

  *  `auto`一般不会推断出顶层`const`类型

    * 因为`auto`忽略顶层`const`, 并保留底层`const`; 

  * 可通过显式添加`const`得到顶层`const`;

  	* ```c++
    	int i = 0;
      const int ci = i, &cr = ci;
    	auto b = ci;				// b is an int
    	auto c = cr;				// c is an int
    	auto d = &i;				// d is a pointer to int
    	auto e = &ci;				// e is a pointer to const int
    	const auto f = ci;			// f is const int
    	```

  

## 5.3`decltype`类型指示符(c++11)*

* `decltype`类型的变量: 编译器根据表达式为变量推断出类型;
  * 如: `decltype(f()) sum =x; // sum has whatever type f returns`
  * 编译器不会调用`f()`, 只是分析`f`函数的返回值类型, 并作为`sum`的类型
* `decltype`中的表达式
  * 表达式是变量: `decltype`返回该变量的类型, 包括顶层`const`和引用
  * 表达式不是变量: `decltype`返回类型取决于表达式结果.
    * 表达式的结果对象是左值: `decltype`返回一个与结果类型相应的引用类型
      * `decltype((variable))`必定返回引用类型[^1]
    * 表达式的结果对象是右值: `decltype`返回一个结果的类型

# 6. 自定义数据结构*

* 什么是数据结构
  * 从最基本的层面理解, 数据结构是一种将相关的数据元素组织起来的方式, 是一种使用这些数据的策略.
* C++允许用户以类的形式来自定义数据结构.

## 如何定义结构类型(struct)

* 类[^2]以关键字`struct`开始, 紧跟类名和类体(其中类体部分可以为空). 
* 类体(**class body**)由花括号包围形成一个新的作用域.
* 类内部定义的名字必须在类内唯一; 可以和类外部定义的名字重复
* 在类体右侧表示结束的花括号后接一个分号; 

```c++
struct Sales_data {
    std::string bookNo;
    unsigned units_sold = 0;
    double revenue = 0.0;
};
```

也可以在类体后接一个或多个声明符, 从而定义相应的变量. (非必要情况下, 不建议这样做)

```c++
struct Sales_data { /* ... */ } accum, trans, *salesptr;
//equivalent, but better way to define these objects
struct Sales_data { /* ... */ };
Sales_data accum, trans, *salesptr;
```

## 类的数据成员

类体(Class body)定义类的成员(**member**); 其中, 数据成员(**data member**)定义了类的对象的具体内容

* 每个对象都有自己的一份数据成员拷贝

定义数据成员的方法: 和定义普通变量一样, 基本类型紧跟一个或多个声明符.

* 类内初始符(**in-class initializer**)(C++11): 当创建一个对象时, 类内初始符被用于初始化对象的数据成员.
  * 使用形式: `=`形式, `{}`形式; 注意: 不能使用`()`形式. 
* 没有类内初始符的数据成员会进行默认初始化.

## 头文件

类可以定义在函数内, 也可以定义在函数外. 

* 在函数内定义类时, 该类会受到一些限制; 一般情况下不会在函数内定义类.

* 在函数外定义类时, 如果需要在不同文件中使用同一个类, 那么该类的定义要保持一致.
  * 为了确保各个文件中类的定义一致, 类通常被定义在头文件中; 类所在的头文件通常与类名一样.

头文件, 通常包含那些只能被定义一次的实体, 如类的定义, `const`和`constexpr`变量.

### 预处理器概述

背景
* 头文件经常用到其他头文件的功能; 为了使用其他头文件, 该头文件必须包含所需的头文件. 这将可能出现多次包含的情况.

> 例子: 一个自定义的`myclass`类有一个`string`类型的成员, 则`myclass.h`头文件要包含`string.h`头文件; 在程序中, 如果同时需要`myclass`类和`string`类, 则文件将包含对应的头文件`myclass.h`和`string.h`; 此时, `string.h`被包含两次, 一次是直接包含的, 一次是随着包含`myclass.h`被隐式包含进来.

预处理器(**preprocessor**): 作用之一是可以确保头文件多次包含仍能安全工作

* 预处理器是在编译之前执行的程序,  它会根据命令改变文件的源代码.

预处理器功能

* 预处理器变量: 预处理器变量存在两种状态: 已定义和未定义.
  * 预处理器变量无视C++语言中 关于作用域的规则
  * 在整个程序中, 预处理器变量必须唯一; 为了避免与程序中的其他实体发生名字冲突, 一般把预处理变量的名字全部大写.

* `#include`指令:  `#include`指令指定一个头文件; 当预处理器遇到`#includ`指令时就会用指定的头文件的内容代替`#include`指令

* `#define`指令: `#define`指令指定一个名字, 把该名字定义为预处理器变量; 

* `#ifdef`指令: `#ifdef`指令接受一个名字; 当该名字已定义为预处理器变量, 则检查结果为`true`

* `#ifndef`指令: `#ifndef`指令接受一个名字; 当该名字未定义为预处理器变量, 则检查结果为`true`

* `#endif`指令: 当`#ifdef`或`ifndef`的检查结果为`true`, 将执行后续操作直至遇到`#endif`指令为止. 当结果为`false`, 则从`#endif`处开始执行后续操作.

* 头文件保护符(**header guard**): 是指用于防止重复包含的代码中的预处理器变量

  * 利用`#define`, `ifdef`, `ifndef`和`endif`等指令, 构成一段保护代码, 如下

  * ```c++
    #ifndef NAME	// NAME is a header guard
    #define NAME
    // Header content
    #endif
    ```



[^1]: 将一个变量放入括号中, 如`(i)`, 这是一个表达式; 其结果为变量本身, 而变量是左值.
[^2]: 广义的类, 是指自定义的数据结构

