# 总结

面向对象编程(**Object-oriented programming, OOP**)

* 数据抽象
* 继承
* 动态绑定

[toc]

# 1.OOP概述*

面对对象编程的核心思想

* 数据抽象
* 继承
* 动态绑定

## 继承Inheritance

因继承而关联的类, 构成一个分层次的体系:

* 基类(**base class**): 被体系中的其他类直接或间接地继承的类
  * 基类定义在该体系中公有的成员
* 派生类(**derived class**): 直接或间接地继承基类的类
  * 派生类各自定义自己特有的成员
  * 派生类必须通过使用类派生列表(**class derivation list**), 明确指出它的基类;
    * 定义派生类时, 在派生类名后接一个冒号, 紧跟以逗号分隔的基类列表
    * 形式如`class DerivedClass : BaseClassList`

在C++中, 基类可以将它的成员函数分为两类

* 派生类直接继承而不做改变的函数
* 类型相关而需要改变的函数; 

基类希望其派生类为继承的函数定义自身版本, 可以将这些函数声明为虚拟的; 这些函数被称为虚函数(**virtual function**)

* 派生类必须在其内部对所有要定义的虚函数进行声明; 
  * 可以在这些函数声明中包含`virtual`关键字, 但不要求这样做
  * 在C++11下, 可以将这些函数声明为`override`: 在形参列表后加`override`关键字
* 对于不打算定义的虚函数, 可以直接忽略这些函数的声明

**例子**

书店中不同书籍有不同的定价策略, 如原价销售, 打折销售等

* 层次体系
  * 定义一个`Quote`类为基类, 表示按原价销售的书籍
  * 继承`Quote`的`Bulk_quote`类, 表示打折销售的书籍
* 成员函数
  * `isbn()`, 返回书籍的ISBN号; 该操作是公有的, 因此定义在基类
  * `net_price(size_t)`, 接受购买数量, 返回本次销售的实际总额; 该操作显然是类型相关的

```c++
class Quote {
public:
    std::string isbn() const;
    virtual double net_price(std::size_t n) const;
};

class Bulk_quote : public Quote {
public:
    double net_price(std::size_t) const override;
};
```

* `Bulk_quote`的类派生列表是`public Quote`, 表示继承了`Quote`的`public`成员
  * 因为`Bulk_quote`包含了`Quote`的所有接口, 这让我们能够把`Bluk_quote`对象当作`Quote`对象使用

## 动态绑定Dynamia Binding

动态绑定, 也称为运行时绑定(**run-time binding**), 使我们能用同一段代码来处理基类和派生类

* 当我们使用基类的引用(或指针)调用一个虚函数时将发生动态绑定

例子

```c++
double print_total (ostream &os,
                    const Quote &item, size_t n)
{
    double ret = item.net_price(n);
    os << "ISBN: " << item.isbn()
       << " # sold: " << n << " total due: " << ret << endl;
    return ret;
}
```

* 例子中的函数有一个`Quote`引用`item`; 而无论是`Quote`对象还是`Bulk_quote`对象都能被形参`item`接受
* 在运行时将根据`item`的实际类型来决定要执行`net_price`的哪个版本

# 2.定义基类和派生类

## 2.1定义基类*

* 作为继承体系的根部的基类, 通常会定义一个虚析构函数

### 成员函数与继承

派生类继承其基类的所有成员

* 对于继承的虚函数成员, 派生类可以覆盖(**override**)其从基类继承的定义; 也就是说, 对其提供它自己的新定义

在C++中, 基类的成员函数区分为: 一种是基类希望其派生类进行覆盖的函数, 一种是基类希望派生类不要覆盖的函数;

* 对于前者, 基类通常将其定义为虚函数(**virtual function**); 当使用指针或引用调用虚函数时, 该调用将会被动态绑定; 根据引用或指针所指向的对象类型不同, 该调用可能执行基类的版本或某个派生类的版本
  * 任何非静态成员函数, 除了构造函数, 都可以是虚拟的(**virtual**)
  * 关键字`virtual`只能出现在类内部的声明语句之前而不能用于类外部的函数定义
  * 如果基类把一个函数声明为虚函数, 则该函数在派生类中隐式地被声明为虚函数
* 如果成员函数没被声明为虚函数, 则其解析过程发生在编译时而非运行时

### 访问控制与继承

派生类继承其基类的成员, 但是派生类的成员函数不一定有权访问从基类继承而来的成员

* 与其他使用基类的代码一样, 派生类能访问`public`成员, 而不能访问`private`成员
* 此外还有一种访问说明符`protected`: 
  * 派生类有权访问基类的`protected`成员
  * 其他用户禁止访问`protected`成员.

### 例子

```c++
class Quote {
public:
    Quote() = default;
    Quote(const std::string &book, double sales_price):
    			bookNo(book), price(sales_price) { }
    std::string isbn() const {return bookNo;}
    virtual double net_price(std::size_t n) const
    			{return n * price;}
    virtual ~Quote() = default;
private:
    std::string bookNo;
protected:
    double price = 0.0;
};
```

## 2.2定义派生类*

### 例子

本节中以`Blukc_quote`为类

```c++
class Bulk_quote : public Quote {
    Bulk_quote() = default;
    Bulk_quote(const std::string &book, double p,
           std::size_t qty, double disc) :
		   Quote(book, p), min_qty(qty), discount(disc) { }
    double net_price(std::size_t) const override;
private:
    std::size_t min_qty = 0;
    double discount = 0.0;
};
```

### 类派生列表

派生类必须在类派生列表(**class derivation list**)中指明它从哪个基类继承而来的

* 类派生列表的形式: 一个冒号后面紧跟以逗号分隔的基类列表
* 基类列表中, 每个基类名前是一个可选的访问说明符, 是 `public`, `protected`或`private`之一
  * 访问说明符的作用是控制派生类从基类继承而来的成员是否对派生类的用户可见
  * 如果派生是`public`的, 基类的`public`成员也是派生类接口的组成部分; 此外, 能将公有派生类的对象绑定到基类的引用或指针上

只继承一个类的继承形式, 被称作单继承(**single inheritance**)

### 派生类中的虚函数

> 派生类中的虚函数分为两种: 从基类继承的虚函数, 派生类自己声明且并非从基类继承的虚函数

派生类可以覆盖它继承的虚函数, 也可以不覆盖

* 如果派生类不覆盖其基类中的某个虚函数, 则与其他继承成员一样, 派生类继承了其定义在基类的版本
* 如果派生类覆盖其基类中的某个虚函数, 则派生类必须声明该虚函数
  * 可以在其覆盖的虚函数前使用`virtual`关键字, 但不是非得这么做.
  * C++11标准允许派生类显式地说明它打算使用某个成员函数覆盖它继承的虚函数; 
    * 具体做法是: 在形参列表后面添加`override`; 
      * 如果是`const`成员函数, 则在`const`后面添加`override`
      * 如果是引用成员函数, 则在引用限定符后面添加`override`

派生类自己声明的虚函数, 与继承体系的根部基类声明的虚函数一样

### 派生类对象及派生类到基类的类型转换

一个派生对象包含多个组成部分

* 一个含有派生类自己定义的非静态成员的子对象
* 派生类继承的每个基类都有对应的子对象; 这些子对象都包含对应基类定义的非静态成员

由于一个派生对象包含与其基类对应的子对象, 我们能把派生类的对象当成基类对象来使用; 特别是能将基类的指针或引用绑定到派生类对象的基类部分中

* 这种能将基类的指针或引用绑定到派生对象上的类型转换通常被称为, 派生类到基类的类型转换(**derived-to-base conversion**). 与其他类型转换一样, 编译器会隐式地执行派生类到基类的类型转换
* 具体情况是: 
  * 在需要基类引用的地方可以使用派生类对象或派生类对象的引用
  * 在需要指向基类的指针的地方可以使用指向派生类对象的指针

### 派生类的构造函数

>  每个类控制它自己的成员初始化过程

派生类的构造函数

* 派生类不能直接初始化它的基类部分, 必须使用基类的构造函数来初始化.
  * 可以在构造函数体内对基类的`public`和`protected`成员赋值, 但不建议这么做

* 派生类对象的基类部分与派生类对象自己的数据成员都是在派生类构造函数的初始化阶段中执行初始化操作
  * 初始化阶段具体是: 
    * 首先通过基类构造函数来初始化基类部分
    * 然后按照声明的顺序依次初始化派生类的成员

在派生类构造函数的初始化列表中

* 不使用基类构造函数作为初始符时, 如果基类有默认构造函数的话, 该派生类的基类部分被默认初始化
* 使用基类构造函数作为初始化, 将根据基类构造函数的实参来选择匹配的基类构造函数来初始化基类部分

### 派生类使用基类的成员

派生类可以访问基类的`public`和`protected`成员.

派生类的作用域是嵌套在基类的作用域之内

* 派生类作用域的名字会覆盖基类作用域的同名的定义
* 查找名字的顺序是: 先在派生类作用域中查找, 再到基类作用域中查找

```c++
double Bulk_quote::net_price(size_t cnt) const
{
    if (cnt >= min_qty)
        return cnt * (1 - discount) * price;
    else
        return cnt * price;
}
```

* 其中`price`是`Bulk_quote`的基类`Quote`的`protected`成员

### 静态成员与继承

如果基类定义了一个静态成员, 则在整个继承体系中只存在一个这样的静态成员

* 不管从基类中派生出来多少个派生类, 每个静态成员都只存在唯一的实例

静态成员遵循通用的访问控制规则

* 如果静态成员是可访问的, 即`public`或`protected`,  可通过基类或派生类来使用它

### 派生类的声明

派生类的声明与其他类一样, 也就是说派生类的声明不要包含类派生列表

```c++
class Bulk_quote : public Quote;	//error
class Bulk_quote;					//ok
```

### 被用作基类的类

如果想将某个类用作基类, 则该类必须已经定义而非仅仅声明

* 原因: 派生类中包含并且可以使用它从基类继承的成员, 为了使用这些成员, 派生类必须要知道它们是什么
* 也可以知道, 一个类不能派生它本身

基类本身可以是派生类

* 派生类的直接基类(**direct base**), 是指出现在类派生列表中的类
* 间接基类(**indirect base**), 是指直接基类的基类

```c++
class Base {/*...*/};
class D1 : public Base {/*...*/};
class D2 : public D1 {/*...*/};
```

* 例子中, `Base`是`D1`的直接基类, 是`D2`的间接基类

每个派生类都会继承直接基类的所有成员; 而直接基类也包含了其基类的成员, 依次类推. 因此, 派生类包含它的直接基类的子对象以及所有间接基类的子对象

### 阻止继承(C++11)

在类名后跟关键字`final`

```c++
class NoDerived final {/*...*/};
class Base {/*...*/};
class Last final : Base {/*...*/};
class Bad : NoDerived {/*...*/};		//error
class Bad2 : Last {/*...*/};		//error
```

## 2.3类型转换和继承*

可以将基类的指针或引用绑定到派生对象

* 也就是说, 当使用一个指向基类的引用或指针时, 我们并不能知道该引用或指针指向对象的实际类型是什么, 因为该对象可能是基类对象, 也可能是派生类对象

### 静态类型与动态类型*

当使用存在继承关系的类型时, 必须将表达式/变量的静态类型和动态类型区分开来

* 静态类型是变量声明时或表达式生成的类型; 在编译时总是已知的

* 动态类型是变量或表达式所表示的内存中对象的类型; 直到运行时才可知

  ```c++
  double ret = item.net_price(n);
  ```

  * `item`的静态类型是`Quote&`, 动态类型则依赖于`item`绑定的实参, 要到运行时调用该函数时才知道其动态类型;

* 不是引用或指针的表达式的动态类型总是与静态类型相同; 如果表达式是引用或指针时, 则动态类型可能与静态类型不相同

### 派生类到基类的类型转换的说明

有派生类到基类的类型转换, 但是没有隐式的基类到派生类的类型转换

* 即使一个基类指针或引用绑定在一个派生类对象上, 也不能执行从基类向派生类的转换
  * 编译器无法在编译时确定该转换是否安全, 因为在编译时只能得知其静态类型
* 如果在基类中含有一个或多个虚函数, 可以使用`dynamic_cast`请求一个类型转换, 该转换的安全检查将在运行时执行.
* 如果已知某个基类向派生类的转换是安全的 , 也可以使用`static_cast`来强制覆盖掉编译器的检查工作

派生类到基类的类型转换只应用于引用或指针类型, 在派生类对象和基类对象之间没有这样的类型转换.

* 虽然经常可能将派生类对象转换成它的基类类型, 但是这种转换的实际发生过程不是我们所期望的那样.
  * 与其他类一样, 基类的初始化或赋值操作其实是在调用对应的函数; 如果基类的初始化或赋值操作函数的形参是引用或指针时, 则可以通过派生类到基类的类型转换使用派生类对象作为实参; 
  * 由于调用函数的对象不是引用或指针类型, 所以没有动态绑定; 
  * 对于基类的这些函数来说, 它们不可能知道派生类的成员, 只知道基类的成员; 所以, 把派生类对象作为实参时, 这些函数只会对派生类的基类部分进行操作
  * 当我们可以用一个派生类对象为一个基类对象初始化或赋值时, 只有该派生类对象中的基类部分会被拷贝,移动或赋值, 它的派生类部分将被忽略掉

派生类到基类的类型转换可能会由于访问受限而变得不可行. s15.5 p613

# 3.虚函数*

> 在C++中, 当通过基类的引用或指针调用虚函数时会执行动态绑定

所有虚函数都必须有定义

* 因为直到运行时才能知道要调用的虚函数版本, 编译器无法确定到底会使用哪个虚函数

当通过基类的引用或指针调用虚函数时会执行动态绑定,

* 当通过基类的引用或指针调用虚函数时, 编译器生成的代码在运行时才确定应该调用哪个版本的函数; 被调用的函数是与绑定到引用或指针上的对象的动态类型相匹配的那个版本
* 需要注意的两点: 一是基类的引用或指针, 二是虚函数
  * 如果通过普通类型的表达式调用虚函数时, 或者调用普通成员函数时, 在编译时就会将调用的版本确定下来

## 派生类中的虚函数

一旦某个函数被声明成虚函数, 则在所有派生类中它都是虚函数

* 因此, 在派生类中覆盖某个虚函数时, 可以不使用`virtual`关键字指出该函数的性质

派生类覆盖从基类继承的虚函数, 那么

*  该虚函数的形参类型必须与被它覆盖的基类函数完全一样
  * 如果不一样, 则隐藏了基类的同名函数
* 除了一个例外, 该虚函数的返回类型必须与被覆盖的基类函数完全一样
  * 例外: 当类的虚函数返回类型是类本身的指针或引用时, 上述规则无效
  * 也就是说, 如果`D`类由`B`类派生得到, 则基类的虚函数可以返回`B*`而派生类的对应函数可以返回`D*`, 不过这样的返回类型要求从`D`到`B`的类型转换时可访问的

## `final`和`override`说明符(C++11)

派生类可以定义了一个函数与基类中虚函数的名字相同但是形参列表不同; 编译器将认为新定义的函数与基类中的同名函数是相互独立的. 此时, 派生类的函数并没有覆盖基类中的版本, 而是隐藏了基类中的版本.

在实际的编程中, 这种声明经常是一种错误; 类作者是打算覆盖基类的虚函数, 而不小心把形参列表弄错了

可以在派生类的虚函数上使用`override`关键字, 表示打算用该函数覆盖对应的虚函数

* 如果使用`override`标记某个函数, 而该函数并没有覆盖已存在的虚函数, 则编译器报错

可以把某个函数指定为`final`, 表示不希望该函数被覆盖

* 任何试图覆盖`final`函数的操作都会引发错误

`final`和`override`说明符出现在形参列表(包括任何`const`或引用限定符)以及尾置返回类型之后

## 虚函数和默认实参

虚函数可以有默认实参. 如果某次函数调用使用默认实参, 则该实参是由本次调用的静态类型决定的

* 如果通过基类的引用或指针调用函数, 则使用基类中定义的默认实参

## 回避虚函数机制

在某些情况下, 我们希望对虚函数的调用不要进行动态绑定, 而是强迫其执行虚函数的某个特定版本.

* 使用作用域运算符指定虚函数的特定版本

  ```c++
  double undiscounted = bassP -> Quote::net_price(42);
  ```

  * 无论`bassP`是基类还是派生类都会执行`net_price`的`Quote`基类版本

# 4.抽象基类

纯虚函数, 是一种可以没有定义的虚函数

* 方式: 将其声明为`=0`, 形如`virtual return_type func(parametre_list) = 0;`
  * `=0`只能出现在类内部的虚函数声明处
* 如果类有一个非继承的纯虚函数, 可以在类外部为其提供定义; 也就是说, 不能在类的内部为一个`=0`的函数提供函数体
  * 该纯虚函数的定义可以被后续的派生类所使用

抽象基类(**abstract base class**), 是含有(或未经覆盖直接继承)纯虚函数的类

* 作用: 抽象基类负责定义接口, 而后续的派生类可以覆盖接口
* 注意
  * 不能直接创建一个抽象基类的对象;
  * 继承抽象基类的类如果不把所有纯虚函数覆盖, 那它也是一个抽象类
  * 继承抽象基类的类覆盖了所有纯虚函数, 那么可以创建该类的对象

# 5.访问控制和继承*

## `protected`成员

一个类使用`protected`关键字来说明那些希望与派生类分享但是不想被用户访问的成员

* `protected`成员对于类的用户来说是不可访问的
* `protected`成员对于派生类的成员和友元来说是可访问的; 不过派生类的友元和成员只有通过派生类对象才能访问基类的`protected`成员; 
  * 也就是说, 派生类的成员和友元只能访问派生类对象中的基类部分的`protected`成员, 而不能访问普通的基类成员

## `public`, `private`和`protected`继承

某个类对其继承而来的成员的访问权限受到两个因素影响

* 基类中该成员的访问说明符
* 在派生类的派生列表中的访问说明符

派生访问说明符

* 不影响派生类成员对基类的访问权限

* 用于控制派生类的用户(包括派生类的派生类)对于基类成员的访问权限; 
  * 当继承是`pulibc`时, 基类成员将遵循原因的访问说明符
  * 当继承是`protected`时, 对于用户和后续的派生类来说, 基类的`public`成员的访问权限变为`protected`
    * 因此后续的派生类可以访问基类的`public`和`protected`成员, 用户不能访问基类的任何成员
  * 当继承是`private`时, 对于用户和后续的派生类来说, 基类的所有成员的访问权限变为`private`
    * 因此用户和后续的派生类都不能访问基类的成员

## 派生类到基类的类型转换的可访问性

使用派生类到基类的类型转换的代码决定该类型转换是否可访问, 同时派生类的派生访问说明符也会有影响.

假设`D`继承自`B`

* 用户代码: 只有当`D`公有地继承`B`时, 用户代码才能使用派生类到基类的类型转换; 如果`D`继承`B`的方式是`protected`或`private`, 则用户代码不能使用该转换
* `D`的成员函数和友元: 无论`D`以什么方式继承`B`, `D`成员函数和友元都能使用派生类到基类的类型转换
* `D`的派生类的成员函数和友元: 如果`D`继承`B`的方式是`protected`或`public`, 则`D`的派生类的成员函数和友元能使用该转换; 如果`D`继承`B`的方式是`private`, 则不能使用该转换

## 友元和继承

友元关系既不能传递, 也不能继承; 每个类负责控制各自成员的访问权限

假设`D`继承自`B`, 而`B`有一个友元`F`, `D`和`F`没有友元关系
* `F`可以访问`B`, 也可以访问`D`中的`B`子对象

## 改变个别成员的可访问性

通过使用`using`声明, 可以改变派生类继承的某个名字的访问级别

* 在类内部的`using`声明可以指定该类的直接或间接基类中的任何可访问成员
* `using`声明中名字的访问权限由该`using`声明之前的访问说明符来决定

```c++
class Base {
public:
    std::size_t size() const {return n;}
protected:
    std::size_t n;
};
class Derived : private Base {
public:
    using Base::size;
private:
    using Base::n;
};
```

* `Derived`使用`private`继承, 因此继承而来的成员`size`和`n`在默认情况下是`Derived`的`private`成员.
* 在使用`using`声明改变成员的可访问性后,  `size`是`public`成员, `n`是`private`成员

## 默认的继承保护级别

默认派生访问说明符由定义派生类所用的关键字来决定

* 使用`class`定义的派生类默认情况下是私有继承的
* 使用`struct`定义的派生类默认情况下是公有继承的

# 6.继承中的类作用域*

每个类都定义自己的作用域; 类所定义的成员都在该类的作用域中

当存在继承关系时, 派生类的作用域嵌套在其基类的作用域之内. 

* 当一个名字未能在派生类的作用域中解析, 则编译器将继续在外层的基类作用域中寻找该名字的定义
* 因为类作用域有这种继承嵌套的关系, 所以派生类才能像使用自己的成员一样使用基类的成员

## 在编译时进行名字寻找

一个对象, 引用或指针的静态类型决定了该对象的哪些成员时可见的.

* 即使静态类型与动态类型可能不一致, 静态类型决定了该对象能够使用的成员
  * 

## 名字冲突与继承

与其他作用域一样, 派生类也能重用定义在其直接基类或间接基类中的名字, 此时定义在内层作用域(即派生类)的名字将隐藏定义在外层作用域(即基类)的名字

可以通过使用作用域运算符来使用被隐藏的基类成员

* 作用域运算符覆盖原有的查找规则, 并指示编译器从给定的作用域中查找名字

声明在内层作用域的函数并不会重载声明在外层作用域的函数; 

* 因此, 定义派生类中的函数也不会重载其基类中的成员.
* 和其他作用域一样, 如果派生类的成员与基类的某个成员同名, 则派生类将在其作用域内隐藏该基类成员; 即使派生类成员和基类成员的形参列表不一致, 基类成员也会被隐藏
  * 这也就是为什么基类与派生类中的虚函数必须有相同的形参列表

## 重载函数与继承

无论是否虚函数, 成员函数都能被重载; 派生类可以重定义重载函数中的零个或多个实例

* 如果派生类重定义重载函数中的一部分实例, 那么未重定义的重载版本对于派生类来说是不可见的

* 如果派生类仅需重定义重载函数中的一部分, 也希望所有的重载版本对于它都是可见的
  * 可以为重载的成员提供一条`using`声明语句; 而后, 定义要重定义的重载版本即可
  * `using`声明语句指定一个名字而不指定形参列表, 就可以将该名字的所有版本添加到所在的派生类作用域中; 不过在基类中该名字的每个实例在派生类中都必须是可访问的

# 7.构造函数与拷贝控制

与其他类一样, 在继承体系中的类控制对象的创建, 拷贝, 移动, 赋值和销毁的行为. 如果一个类(基类或派生类)没有定义拷贝控制操作, 则编译器将为它合成一个版本; 当然, 合成的版本可能被定义为删除的

## 7.1虚析构函数*

基类通常应该定义一个虚析构函数

* 析构函数需要被声明为虚函数以允许继承体系中的对象被动态分配
  * 原因: 如果某个指针指向继承体系中的某个类型, 那么它的静态类型与动态类型可能不一样; 如果要`delete`该指针, 编译器应该执行动态类型的析构函数, 这就需要动态绑定以执行正确的析构函数版本, 所以要定义虚析构函数
* 派生类会继承基类的虚函数, 包括虚析构函数; 因此, 只要基类的析构函数是虚函数, 就能确保`delete`指向基类指针时将运行正确的析构函数版本

三五原则的例外就是基类的析构函数

* 基类通常为了析构函数的虚属性而定义析构函数, 所以无法由此推断该基类是否需要其他拷贝成员

## 7.2合成拷贝控制与继承*

基类或派生类的合成拷贝控制成员的行为

* 对类本身的成员逐个初始化, 赋值或销毁
* 此外, 会使用直接基类中对应的操作对其对象中的直接基类部分进行初始化, 赋值或销毁操作

派生类的默认构造函数, 拷贝构造函数, 拷贝赋值运算符, 移动构造函数, 移动赋值运算符或析构函数被定义为删除的情况

* 与其他任何类一样的情况, s13.1.6 p508, s13.6.2 p537

* 直接基类的对应成员是被删除或不可访问
* 如果直接基类的析构函数是被删除或不可访问, 派生类的默认和拷贝构造函数也会被定义为删除
* 和其他任何类一样, 编译器不会合成一个删除的移动操作; 当使用`=default`请求一个移动操作时, 如果基类对对应的移动操作是被删除或不可访问, 则派生类的对应移动操作被定义为删除

由于大多数基类都定义一个虚析构函数, 在默认情况下, 基类通常不含有合成的移动操作, 因此它的派生类也没有合成的移动操作;

* 当需要执行移动操作时应该首先在基类中定义移动操作

## 7.3派生类拷贝控制成员*

派生类的构造函数在其初始化阶段中首先调用其直接基类的构造函数, 然后初始化它自己的成员

* 因此, 基类的成员首先被构建,然后是派生类的成员

派生类的拷贝和移动构造函数也是如此, 不仅要拷贝和移动自有成员, 还要拷贝和移动基类部分的成员

派生类的赋值运算符也必须为其基类部分的成员赋值

派生类的析构函数只需负责销毁派生类自己分配的资源; 

* 在析构函数的隐式销毁阶段中会自动调用直接基类的析构函数, 从而自动销毁基类部分的成员
* 对象销毁的顺序正好与其创建的顺序相反: 首先执行派生类的析构函数, 然后执行直接基类的析构函数, 以此类推, 沿着继承体系的反方向直至最后

## 定义派生类的拷贝/移动构造函数

通常在构造函数的初始化阶段中使用对应的基类构造函数初始化对象的基类部分

```c++
class Base {/*...*/};
class D : public Base {
publibc:
    D(const D& d) : Base(d) /*D成员的初始符*/  {/*...*/}
    D(D&& d) : Base(std::move(d)) /*D成员的初始符*/  {/*...*/}
}
```

## 定义派生类赋值运算符

派生类赋值运算符也必须显式地为其基类部分赋值

```c++
D &D::operator=(const D& rhr)
{
    Base::operator=(rhs);
    /*为派生类的成员赋值的代码*/
    return *this;
}
```

## 定义派生类析构函数

派生类的析构函数只负责销毁由派生类自己分配的资源

```c++
class D : public Base {
publibc:
    ~D() {/*释放派生类分配的资源*/}
}
```

## 在构造函数和析构函数中调用虚函数

> 如果构造函数或析构函数调用了某个虚函数, 则执行与构造函数或析构函数所属类型相对应的虚函数版本

如之前所提到的, 在创建派生类对象时, 首先构建基类部分的成员; 所以当正在执行基类的构造函数时, 该对象的派生类部分是未被初始化的. 类似的, 对于销毁派生类对象, 当执行基类的析构函数时, 该对象的派生类部分已经被销毁. 

* 因此, 当执行上述基类成员时, 该对象处于未完成的状态

如果在执行上述基类成员时调用虚函数, 那么会如何处理这种未完成的状态

* 当正在构建一个对象时, 编译器把对象的类看作为正在执行的构造函数所属的类
* 类似地, 当正在销毁一个对象时, 编译器把对象的类看作为正在执行的析构函数所属的类
* 因此, 在构造函数或析构函数中调用虚函数时, 虚函数调用与构造函数/析构函数所属的类绑定, 那么会调用该类的虚函数版本

## 7.4继承构造函数(C++11)

在C++11标准中, 派生类可以重用其直接基类的构造函数, 也就是"继承"构造函数

* 这里的"继承"不是常规的继承

继承构造函数

* 方式: 使用一条指定直接基类名的`using`声明, 形如`using base::base`
  * 第一个`base`表示作用域, 第二个`base`表示构造函数
* 当`using`作用于构造函数时, 对于直接基类的每个构造函数, 编译器都生成一个与之对应的派生类构造函数
  * 生成的形式是`derived(parms) : base(args) {}`
    * `derived`是该派生类的名字, `base`是直接基类的名字
    * `parms`是基类构造函数的形参列表, `args`将该构造函数的形参传递给基类构造函数
    * 函数体为空
* 如果派生类含有自己的数据成员, 则这些成员被默认初始化

* 注意

  * 不能继承默认, 拷贝和移动构造函数
  * 只能继承直接基类的构造函数

  * 与普通成员的`using`声明不同的是 , 构造函数的`using`声明不会改变该构造函数的访问级别
    * 即不管构造函数的`using`声明出现在哪儿, 基类的私有构造函数在派生类中还是私有的, 受保护的构造函数还是受保护的, 公有的还是公有的
  * 构造函数的`using`声明不能指定`explicit`或`constexpr`
    * 如果基类的构造函数是`explicit`或`constexpr`, 则继承的构造函数也拥有相同的属性; 没有的话, 继承构造函数也没有
  * 如果基类构造函数含有默认实参, 不会继承默认实参, 而是派生类生成多个继承的构造函数, 其中每个构造函数依次省略掉一个含有默认实参的形参
    * 如果基类有一个接受两个形参的构造函数, 其中第二个形参含有默认实参, 则派生类将获得两个构造函数: 一个构造函数接受两个形参, 另一个构造函数接受一个形参, 它对应于基类中最左侧的没有默认值的那个形参
  * 如果派生类定义的构造函数与基类的构造函数具有相同的形参列表, 则不继承该基类的构造函数
  * 继承的构造函数在派生类中不被当作用户定义的构造函数; 即如果一个类只含有继承的构造函数, 则该类会合成一个默认构造函数

# 8.容器与继承*

当打算使用容器存放继承体系中的对象时, 通常必须采取间接存储的方式

* 在容器中放置基类的指针, 最好是智能指针

## 8.1例子:`Basket`类p631

在C++中, 无法直接使用对象进行面向对象编程, 而必须使用指针和引用; 而指针会增加程序的复杂性, 因此通常定义一些辅助的类来处理这种复杂情况

# 9.例子: 文本查询程序再探p634

一个支持复杂查询操作的文本查询程序