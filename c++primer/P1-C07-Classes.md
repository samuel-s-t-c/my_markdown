# 总结

如何定义类

[toc]

# 1.抽象数据类型

类, 是指自定义的类型

类(**class**)的基本思想是数据抽象(**data abstraction**)和封装(**encapsulation**)

* 数据抽象, 是一种编程技术和设计技术, 其基本思想是把接口(**interface**)和实现(**implementation**)分离.

  > 接口, 泛指实体与外界交互的渠道
  >
  > 实现, 是使概念成为现实的东西

  * 类的接口是指用户所能执行的与类相关的操作
  * 类的实现包括类的数据成员, 构成接口的函数体, 定义类所需的各种私有函数等

* 封装更是使得类的接口和实现能够分离;

  * 被封装的类隐藏了它的实现
  * 类的用户只能使用接口, 无法知道类的实现
  * 优点:
    * 确保用户代码不会无意间破坏被封装的对象的状态
    * 

抽象数据类型(**abstract data type**), 是指使用数据抽象和封装技术的类

* 对于抽象数据类型, 类设计者要考虑如何实现类
* 抽象数据类型的用户不需要知道该类型怎么工作的, 只需考虑类型能做什么

## 1.1设计抽象数据类型

设计的关键: 

* 应该考虑如何才能使得类易于使用
* 直观而易于使用的接口
* 足够高效的实现

以`Sales_data`类为例, 其用于表示书籍出售记录

* `isbn`成员函数, 返回对象的ISBN
* `combine`成员函数, 把一个`Sales_data`对象加到另一个中
* `add`函数(非成员), 用于两个`Sales_data`对象相加
* `read`函数, 将数据从`istream`中读入到`Sales_data`对象
* `print`函数, 将`Sales_data`对象的值写到`ostream`

## 1.2定义抽象数据类型

形式: 可使用`struct`或`class`关键字  

```c++
class name {/*...*/};
struct name {/*...*/};
```

* `struct`的默认的访问权限是`public`的
* `class`的默认的访问权限是`private`的

要求: 

* 数据成员的声明必须在类的内部
  * 可提供类内初始符: 可以使用`=`或`{}`初始化形式, 不可使用`()`初始化形式
    * 注意: 某些编译器不支持类内初始符

* 成员函数的声明和定义
  * 成员函数的声明必须在类的内部

  * 成员函数的定义可以在类的内部或外部
    * 定义在类内的成员函数隐式地被定义为`inline`

* 作为接口组成部分的非成员函数, 它的定义和声明都在类的外部

例子: `Sales_data`的定义

```c++
struct Sales_data {
    std::string isbn() const {return bookNo;}
    Sales_data& combine(const Sales_data&);
    double avg_price() const;
    //data member
    std::string bookNo;
    unsigned units_sold = 0;
    double revenue = 0.0;
};
//nonmember Sales_data interface functions
Sales_data add(const Sales_data&, const Sales_data&);
std::ostream &print(std::ostream&, const Sales_data&);
std::istream &read(std::istream&, Sales_data&);
```

### `this`形参

使用点运算符可以得到给定对象的成员, 形如`obj.mem`; 如果该成员是函数, 可以调用它, `obj.mem()`

`this`隐式形参: 一个常量指针

* 当调用成员函数时, 有一个隐式的形参`this`, 它是常量指针, 它被初始化为调用该函数的对象地址
  * 例子:`total.isbn()`; 实际上编译器将该调用重写成伪代码`Sales_data::isbn(&total)`
* 成员函数与`this`
  * 在成员函数中, 任何对类的成员的直接使用, 都被看作通过`this`的隐式引用; 也就是说, 在成员函数中, 使用类的成员`mem`, 等价于`this->mem`
    * 例子:`total.isbn()`; `isbn`中有使用`bookNo`, 等价于`this->bookNo`, 此时实际上是`total`对象的`bookNo`成员,
* 用法:
  * 与普通指针一样, 可以在成员函数内使用`this`; 如`*this`表示调用该函数的对象
* 注意: 
  * 虽然`this`是隐式定义的, 但是任何自定义名为`this`的参数或变量的行为都是非法的

### `const`成员函数

默认情况下, `this`是一个指向非常量的常量指针; 因此, 默认情况下`this`不能指向`const`对象, 也就是说我们不能在`const`对象上调用普通的成员函数. 

C++允许把`const`放在成员函数的参数列表之后, 以此声明`this`是一个指向常量的指针; 此时的成员函数被称作常量成员函数(**const member function**)

* 例子: `Sales_data`类内部中, `std::string isbn() const {return bookNo;}`

常量对象, 以及常量对象的指针或引用都只能调用常量成员函数

### 类作用域和成员函数

类本身是一个作用域; 成员函数的定义嵌套在类作用域之内.

编译器处理类的两个步骤: 首先, 编译成员声明; 然后, 处理成员函数的函数体(如果有的话)

* 因此, 不管成员声明的出现次序, 可以在成员函数中随意使用类的其他成员

### 在类的外部定义成员

在类的外部定义一个成员时, 它的名字必须包含它所属的类名

* 做法: 在成员名字前通过`::`作用域运算符指明类名
* 形式:`class_name::member_name `

在类的外部定义成员函数, 成员函数的定义必须与它的声明匹配;

* 也就是说, 返回类型, 函数名, 形参列表和形参列表后的`const`都得与类内的声明一致

例子:

```c++
double Sales_data::avg_price() const {
    if (units_sold)
        return revenue/units_sold;
    else
        return 0;
}
```

* 当编译器看到函数名时, 它知道该函数属于`Sales_data`类, 然后把剩余的代码理解成是位于类作用域内

## 1.3定义类相关的非成员函数*

类相关的非成员函数

* 这些函数定义的操作从概念上说属于类的接口的组成部分, 但是实际上并不属于类本身
* 由于这类函数是接口的一部分, 它们的声明一般与类声明放在同一头文件中.
* 定义非成员函数与定义一般的函数一样

例子: 属于`Sales_data`类的接口的非成员函数`read`

```c++
istream &read(isterea &is, Sales_data &item)
{
    double price = 0;
    is >> item.bookNo >> item.units_sold >> price;
    item.revenue = price * item.units_sold;
    return is;
}
```

## 1.4构造函数*

构造函数(**constructor**): 类的一种特殊成员函数

* 作用: 当某个类的对象被创建时, 运行构造函数以初始化类对象的数据成员; 

  * 因此, 类通过构造函数来控制对象的初始化

* 形式: 

  ```c++
  class_name(parameter_list) {/*...*/}
  ```

  * 构造函数的名字与类名相同
  * 没有返回类型
  * 形参列表可以为空
  * 函数体可以为空
  * 可以包含多个构造函数, 规则与函数重载一样.
  * 构造函数不能被声明成`const`; 因为构造期间要向对象写值

* 注意:

  * 在构造函数的函数体被执行之前, 对象的数据成员已经完成初始化

* 例子: `Sales_data`类的构造函数

  ```c++
  struct Sales_data {
      //constructors
      Sales_data = default;
      Sales_data(const std::string &s) : bookNo(s){ }
      Sales_data(const std::string &s, unsigned n, double p):
      		   bookNo(s), units_sold(n), revenue(p*n) { }
      Sales_data(std::istream &);
  	//other members as before
  };
  ```

### 默认构造函数

默认构造函数(**default constructor**)

* 默认构造函数, 是指不接受实参的构造函数, 用于控制默认初始化
* 没有为类对象提供初始符时, 执行默认初始化, 也就是执行默认构造函数

合成默认构造函数(**the synthesized default constructor**)

* 当类没有明确地定义任何构造函数时, 编译器会定义一个隐式的默认构造函数, 该默认构造函数被称作合成默认构造函数
* 合成默认构造函数按照如下规则初始化类的数据成员
  * 如果存在类内初始符, 则用它来初始化该成员
  * 否则, 默认初始化该成员
* 不应该依赖于**合成默认构造函数**的情况
  * 类内定义了构造函数
    * 编译器根本不会定义一个*合成默认构造函数*
  * 类内含有一个内置类型的成员, 该成员没有类内初始符
    * 该成员被默认初始化; 而内置类型的对象被默认初始化时, 其值是未定义的
  * 类内含有一个类类型的成员, 该类类型没有默认构造函数
    * 由于该成员没有默认构造函数, 编译器无法默认初始化该成员; 所以*合成默认构造函数* 是不可用的

### `=default`的含义(C++11)

`=default`

* 作用: 要求编译器生成构造函数, 该构造函数的作用与*合成默认构造函数*一样

* 用法: `class_name() = default;`
  * 在空的形参列表后面加上`=default`
  * `=default`可以和声明出现在类的内部; 对应的默认构造函数被隐式定义为`inline`
  * `=default`可以和定义出现在类的外部
* 例子: `Sales_data() = default;`

### 构造函数初始符列表

构造函数初始符列表(**constructor initializer list**)

* 作用: 在创建新对象过程中, 为该对象的一个或多个数据成员提供初始符

* 用法: `class_name(parameter_list) : initializer_list {statements}`

  * 在形参列表后面加上`:`和构造函数初始符列表
  * `initializer_list`是成员名字列表, 每个名字后面紧跟被括号括起的初始符(可用圆括号或花括号); 不同成员的初始化通过逗号隔开
  * 不在构造函数初始符列表的数据成员, 以与*合成默认构造函数*相同的方式初始化

* 注意:

  * 构造函数初始符列表用于数据成员的初始化中

* 例子

  ```c++
  Sales_data(const std::string &s) : bookNo(s) { }
  Sales_data(const std::string &s, unsigned n, double p) :
  		   bookNo(s), units_sold(n), revenue(p*n) { }
  ```

### 在类的外部定义构造函数

* 与其他成员函数一样, 构造函数的定义可以在类的外部

* 与以往一样, 在类的外部定义一个成员时, 它的名字必须使用`::`指定它所属的类名

* 例子

  ```c++
  //outsied the Sales_data class
  Sales_data::Sales_data(std::istream &is)
  {
      read(is, *this);
  }
  ```

## 1.5拷贝, 赋值和析构*

类不仅可以控制其对象的初始化, 还可以控制拷贝, 赋值和销毁对象时的行为.(**关于这方面的内容在Part3**)

如果类没有明确定义这些操作, 编译器会合成它们;

* 对于拷贝, 赋值和销毁操作, 编译器生成的版本是对每个成员执行拷贝, 赋值和销毁操作.

* 某些类不能依赖于合成的版本, 如管理动态内层的类

# 2.访问控制与封装*

访问说明符(**access specifier**): 

* `public`说明符
  * 用法:`public:`
  * 作用: 定义在`public`说明符后的成员在整个程序内可被访问; 用于定义类的接口
* `private`说明符
  * 用法:`private`
  * 作用: 定义在`private`说明符后的成员可以被类的成员访问, 但是不能被其他代码访问; 用于隐藏类的实现
* 说明
  * 访问说明符的数量和位置是没有限制的
  * 每个访问说明符指定了接下来的成员的访问级别, 其有效范围直到实现下一个访问说明符或者到达类的结尾处为止

## 2.1友元Friends*

友元

* 作用: 成为某个类的友元后, 该类允许友元访问它的非`public`成员

* 声明友元: 只能在类内部; 在类内的具体位置不限

  * 对于函数, 在函数声明前加上`friend`关键字:  `friend function_declaration;`

* 说明;

  * 友元声明可以在类内的任意地方; 因为友元不是类的成员, 所以友元不受所在区域的访问控制级别的约束
  * 友元声明只是在说明对某个类的访问权限, 并不是通常意义的声明;
    * 如果想要使用某个友元函数, 不仅需要友元声明, 还需要该函数本身的函数声明
    * 一些编译器允许只有友元声明也可以调用友元函数; 不过最好是两种声明都有

* 例子:

  ```c++
  //在Sales_data类内
  friend Sales_data add(const Sales_data&, const Sales_data&);
  ```

# 3.类的其他特性

## 3.1类成员再探

为展示特性, 定义一对相互关联的类`Screen`和`Window_mgr`

* `Screen`表示显示器的一个窗口
* `window_mgr`表示显示器的一组`Screen`

### 类型成员Type Member

类可以为某种类型定义一个类内部的局部名字(别名), 该名字被称为类型成员;

与定义普通的类型别名一样定义类型成员; 

* `typedef`, `using`

注意: 

* 类型成员服从于访问控制, 可以是`public`或`private`中的一种

* 类型成员必须先定义后使用; 所以, 一般把类型成员放在类开始的地方

例子: 也同时给出`Screen`类

```c++
class Screen {
public:
    typedef std::string::size_type pos;
    Screen() = default;
    Screen(pos ht, pos wd, char c) : height(ht), width(wd),
        							 contents(ht * wd, c) { }
    char get() const {return contents[cursor];}
    char get(pos ht, pot wd) const;
    Screen &move(pos r, pos c);
private:
    pos cursor = 0;
    pos height = 0, width = 0;
    std::string contents;
};
```

### 内联成员函数

显式地指明一个成员函数为`inline`

* 在类内部与成员函数的声明一起
* 在类外部与成员函数的定义一起

注意:

* 可以同时在类内外部指明某个成员函数为`inline`; 最好只在类外部定义的地方指明为`inline`

* 与以往一样, 内联函数应该放在头文件中; 特别的, 内联成员函数应该与相应的类定义在同一个头文件中

### 重载成员函数

重载成员函数的规则与普通的函数重载一样

### `mutable`数据成员

`mutable`关键字

* 作用: 使一个数据成员在任何情绪下都是可改变的

* 用法: 在类内部的数据成员声明中加入`mutable`

* 说明: 一个`mutable`数据成员, 即使它是某个`const`对象的成员, 或者在`const`成员函数中, 都能够改变其值

* 例子:

  ```c++
  class Screen() {
  public:
      void  some_member() const;
  private:
      mutable size_t access_ctr; //may change even in a const object
      //other members as before
  };
  void Screen::some_member() const
  {
      ++access_ctr;				//ok
  }
  ```

## 3.2返回`*this`的函数*

值得重看p275~p277

* 一个`const`成员函数如果以引用的形式返回`*this`, 那么它的返回类型将是常量引用
* 基于成员函数是否`const`的重载
  * 成员函数的`const`实际上是修饰`this`指针指向的对象类型, 是底层`const`;所以可以以此重载函数
  * 基于成员函数是否`const`的函数重载; 使得非`const`对象调用非`const`版本的函数, `const`对象调用`const`版本的函数

## 3.3类类型

* 每个类都定义了唯一的类型; 就算它们的成员完全一样, 它们所定义的类型也是不同的类型

* 把类名当作类型使用时, 实际上直接指向所定义的类类型

### 不完全类型

C++允许声明类而暂时不定义它; 这种声明, 称为前向声明(**forward declaration**)

* 作用: 向程序中引入一个名字, 并指明它是一个类类型
* 例子: `class Sreen;`
* 在类声明之后定义之前的区域, 该类对应的类型是一个不完全类型(**incomplete type**)
  * 因为程序只知道它是类类型, 并不知道它到底包含哪些成员

不完全类型(即类仅仅被声明)的使用场景

* 定义指向该类型的指针或引用
* 声明(不能定义)一个函数, 使用该类型作为函数的返回类型或形参

类必须被定义(不能仅仅被声明)的使用场景

* 创建类的对象
  * 因为创建对象要知道需要多少存储空间

* 用引用或指针访问类的成员.

* 除了一个例外(s7.6, p300), 数据成员被声明成该类类型
  * 因为编译器要知道存储该数据成员需要多少空间

在类的内部类本身的类型是不完全类型

* 因为一旦类的名字出现后, 该类就被认为是声明过了
* 只有类全部完成后类才算被定义

* 意味着
  * 类的成员的类型不会是该类本身
  * 类允许包含指向它自身的引用或指针

## 3.4友元再探

友元可以是函数, 也可以是其他的类, 还可以是其他类的成员函数

### 友元类

一个类指定另一个类为友元

* 形式:

  ```c++
  class A {
      friend class B;
  };
  ```

  * 类`B`是类`A`的友元; 类`A`不是类`B`的友元

* 效果

  * 友元类`B`的成员函数都可以访问类`A`中的所有成员

* 注意:

  * 友元关系不存在传递性; 每个类负责控制自己的友元函数或友元类

* 例子

  ```c++
  class Screen {
      friend class Window_mgr;
      // the rest of the Screen class
  };
  class window_mgr {
  public:
      using ScreenIndex = std::vecotr<Screen>::size_type;
      void clear(ScreenIndex);
  private:
      std::vecotr<Screen> screens{Screen(24, 80, ' ')};
  };
  void Windows_mgr::clear(ScreenIndex i)
  {
      Screen &s - screens[i];
      s.contens = string(s.hight * s.width, ' ');
  }
  ```

### 作为友元的成员函数

一个类指定一个成员函数为友元

* 形式: 

  ```c++
  class B {
     type func(parameter_list);
  };
  class A {
      friend type B::func(parameter_list);
  }
  ```

  * 类`B`的成员函数`func`是 类`A`的友元

* 效果:

  * 成员函数`func`可以访问类`A`的所有成员

* 注意: 

  * 要想声明一个成员函数为友元, 需要仔细组织程序的结构, 以满足各个类的声明和定义的彼此的依赖关系

### 作为友元的重载函数

重载函数的名字相同, 但它们仍然是不同的函数;

所以, 如果一个类想把一组重载函数都声明成它的友元, 它需要对这组重载函数中的每一个函数分别声明

### 友元声明和作用域*

要点

* 类和非成员函数可以在它们的友元声明之后才被声明
  * 非成员函数甚至可以在友元声明处被定义; 不过还是要在类外部进行函数声明

* 函数声明的作用是使名字在作用域中变得可见
* 友元声明的作用只是影响类的访问权限, 并不能使名字可见

例子

```c++
struct X {
    friend void f() {/*friend funtcion can be defined in the class body*/}
    X() {f();}	     		//error: no declaration for f
    void g();
    void h();
};
vod X::g() {return f();}	//error: no declaration for f
void f();					//a declaration for f
void X::h() {return f();}	//ok: declaration for f is now in scope
```

# 4.类作用域*

## 4.1名字寻找

# 5.构造函数

## 5.1构造函数的初始符列表

## 5.2

## 5.3默认构造函数

## 5.4隐式的类类型转换

## 5.5聚合类

## 5.6字面值类

# 6.类的静态成员