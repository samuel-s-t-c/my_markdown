# 总结

模板

* 类模板和函数模板
* 显式模板实参
* 模板特例化
* 可变模板, 包扩展

[toc]

# 1.定义模板

> 模板(**template**)是C++中泛型编程的基础

一个模板是一个用于创建类或函数的蓝图/公式; 

* 当使用泛型类型(如`vector`)或泛型函数(如`find`)时, 提供其所需的信息以将模板转变为特定的类或函数; 这种转变是发生在编译时

## 1.1函数模板*

函数模板(**function template**), 用于生成类型特定的函数版本

定义函数模板: 以`template`关键字开始, 后跟一个模板形参列表

* 模板形参列表(**template parameter list**)是由一个或多个模板形参组成的, 形参以逗号分隔; 列表被一对尖括号`<>`包围
  * 模板形参列表不能为空
* 模板形参(**template parameter**)用来表示在类或函数定义中用到的类型或值; 每当使用模板时都会隐式或显式地指定模板实参(**template argument**)以绑定到对应的模板形参上

```c++
template <typename T>
int compare(const T &v1, const T &v2)
{
    if (v1 < v2) return -1;
    if (v2 < v1) return 1;
    return 0;
}
```

* 例子中声明了一个模板类型形参`T`, 可以用名字`T`表示一个类型
* 而`T`表示的实际类型则在编译时根据如何使用`compare`来确定

实例化(**instantiate**)函数模板

* 当调用函数模板时, 编译器可以根据调用时的函数实参来推断出模板实参

* 编译器用模板实参根据函数模板实例化一个特定版本的函数; 当编译器实例化一个模板时, 它使用模板实参代替模板中的模板形参来创建出一个实例(**instantiation**), 即编写并编译一个特定的函数版本

  ```c++
  cout << compare(1, 0) << endl;
  ```
  * 这次调用中实参类型是`int`; 编译器推断出模板实参为`int`, 并实例化一个如下的`compare`版本

  ```c++
  int compare(const int &v1, const int &v2)
  {
      if (v1 < v2) return -1;
      if (v2 < v1) return 1;
      return 0;
  }
  ```

### 模板类型形参

模板类型形参(**template type parameter**): 有关键字`class`或`typename`的模板形参, 表示一个类型

* 关键字`class`和`typename`的含义相同, 可以互换使用
* 当模板实例化后, 模板类型形参被一个类型所代替, 该类型是由用户提供或编译器推断得到.

* 在模板形参列表中, 每个类型形参都必须带有`class`或`typename`关键字

* 在模板定义中, 可以像内置类型或类类型说明符一样使用模板类型形参
  * 特别地说, 在函数模板中, 类型形参可以用作返回类型, 函数的形参类型, 以及在函数体中用于变量声明或类型转换

### 模板非类型形参

非类型模板形参(**nontype template parameter**): 有具体类型名的模板形参, 表示一个值

* 当模板实例化后, 非类型模板形参被一个值所代替, 该值是由用户提供或编译器推断得到
* 该值必须是常量表达式, 必须能在编译时得到, 从而允许编译器在编译时完成实例化
* 在模板定义内, 模板非类型形参是一个常量值; 在需要常量表达式的地方, 可以使用非类型参数

### `inline`和`constexpr`函数模板

`inline`和`constexpr`说明符出现在模板形参列表后和返回类型前

### 编写类型无关的代码

编写泛型代码的两个重要原则

* 模板中的函数形参是`const`引用
  * 这能保证函数模板可以用于不能拷贝的类型
* 函数体中的条件判断仅使用`<`比较运算
  * 这能降低对类型的要求

### 模板编译

当编译器遇到一个模板定义时, 它并不生成代码. 在编译器遇到模板使用时, 编译器进行实参检查, 而后进行模板实例化; 只有实例化模板的一个特定版本时, 编译器才会生成代码;  因此: 

* 为了生成一个模板实例, 编译器需要掌握有函数模板或类模板成员函数的定义; 
  * 所以模板的声明和定义通常放在头文件中.
* 大多数编译错误在实例化期间报告

模板的名字

* 模板的作者要保证, 当使用模板时, 模板中所有不依赖于模板形参的名字都必须是可见的, 而且模板的定义, 包括类模板的成员定义, 也必须是可见的

* 模板的用户要保证, 当使用模板时, 用来实例化模板的名字的声明必须是可见的, 并且保证传递给模板的实参支持模板所要求的操作, 以及这些操作在模板中能正确工作

## 1.2类模板*

类模板(**class template**), 用于生成类

* 编译器不能为了类模板推断模板形参; 为了使用类模板, 必须在模板名后的尖括号中提供额外信息, 也就是模板实参
  * 一个例外: 在一个类模板的作用域中, 可以直接使用模板自己的名字而不必指定模板实参
* 类模板的名字不是类型名; 类模板用来实例化类型, 而一个实例化的类型总是包含模板实参的

定义类模板: 以`template`关键字开始, 后跟一个模板形参列表

* 在定义中, 使用模板形参以代替使用模板时用户需要提供的类型或值

  ```c++
  template <typename T> class Blob {
  public:
      typedef T value_type;
      typedef typename std::vector<T>::size_type size_type;
      //constructors
      Blob();
      Blob(std::initializer_list<T> il);
      //number of element in the Blob
      size_type size() const { return data->size();}
      bool empty() cosnt {return data->empty();}
      //add and remove elements
      void push_back(const T &t) { data->push_back(t);}
      //move version
      void push_back(T &&t) {data->push_back(std::move(t));}
      void pop_back();
      //elements access
      T &back();
      T &operator[](size_type i);
  private:
      std::shared_ptr<std::vector<T>> data;
      void check(size_type i, const std::string &msg) const;
  };
  ```

实例化类模板

* 当使用类模板时, 必须提供模板实参
* 编译器为不同的模板实参生成不同的类

### 类模板的成员函数

可以在类模板的内外部定义其成员函数

* 与以往一样, 定义在类模板内部的成员函数被隐式声明为内联函数

虽然类模板的成员函数本身是一个普通函数, 类模板的每个实例都有其自己版本的成员函数

* 因此, 类模板的成员函数具有和类模板的实例相同的模板形参

* 当类模板的成员函数定义在类模板外部时, 定义以`template`开始, 后接类模板形参列表; 形式如下

  ```c++
  template <typename T>
  ret-type Class<T>:member-name(parm-list)
  ```

默认情况下, 类模板的成员函数只有程序用到它时才进行实例化; 如果某个成员函数没有被使用, 则它不会被实例化; 因此,  有以下特性

* 即使某种类型不能完全符合模板操作的要求, 仍然能用该类型进行实例化

### 类模板与友元

当一个类包含一个友元声明时, 类可以是模板, 也可以不是模板; 友元也是如此.

* 当一个类模板包含一个友元时, 
  * 如果友元不是模板, 则该友元得到所有模板实例的访问权限
  * 如果友元是模板, 则类可以授权给所有友元模板实例, 也可以只授权给友元的特定实例

#### 一对一友谊

* 类模板与另一个模板之间最常见的友谊形式是在两者的对应实例之间建立友谊, 即一对一友谊(**one-to-one friendship**)

* 具体是: 假设有类模板`C`和友元模板`F`; 在类模板`C`内部, 友元的声明用类模板`C`的模板形参作为模板实参; 这样的话, 友元模板`F`中用相同模板实参实例化的版本才有访问权限

  * 注意: 必须首先声明友元模板`F`

  ```c++
  template <typename> class BlobPtr;				// 前置声明, 在Blob中声明友元所需要的
  template <typename> class Blob;					// 前置声明, 运算符=中的参数所需要的
  template <typename T>
  	bool operator=(const Blob<T>&, const Blob<T>&);
  template <typename T> class Blob {
      friend class BlobPtr<T>;  
      friend bool operator==<T>(const Blob<T>&, const Blob<T>&);
  };
  ```

#### 通用和特定模板友谊

* 一个类将另一个模板的所有实例都声明为友元, 这是通用模板友元

  * 无须前置声明
  * 例子: `template <typename T> class F;`

* 一个类将另一个模板的特定实例声明为友元, 这是特定模板友元

  * 需要前置声明
  * 例子: `friend class F<T>;`

  ```c++
  template <typename T> class Pal;			//前置声明, 在C和C2中声明友元所需要的
  class C {									//C是一个普通的非模板类
      friend class Pal<C>;					//用C实例化的Pal是友元, 需要前置声明
      template <typename T> friend class Pal2;//Pal2的所有实例都是友元, 不需要前置声明
  };	
  template <typenamt T> class C2 {			//C2是一个类模板
      friend class Pal<T>;					//一对一友元, 需要前置声明
      template <typename X> friend clas Pal2;	//Pal2的所有实例都是友元, 不需要前置声明
      friedn class Pal3;						//普通友元
  };
  ```

#### 令模板类型形参成为友元(C++11)

可以将模板类型形参声明为友元

* 为了可以用内置类型实例化, 在这种情况下允许将内置类型声明为友元
  * 友元通常必须是一个类或一个函数

```c++
template <typename Type> class Bar {
    friend Type;
};
```

### 类模板与类型别名

类模板的一个实例定义了一个类类型; 可以使用`typedef`或`using`为类模板实例定义一个类型别名

```c++
typedef Blob<string> StrBlob;
using StrBlob = Blob<string>;
```

C++11中, 允许为类模板定义一个类型别名; 

* 当定义一个模板类型别名时, 可以为该类模板固定一个或多个模板形参
* 当使用模板类型别名时, 要提供该别名所需的模板实参, 与别名定义时的模板形参列表匹配

```c++
template<typename T> using twin = pair<T, T>;
template<typename T> using partNo = pair<T, unsigned>;	//固定了一个模板形参为unsigned
twin<stirng> authors;	//authors的类型是pair<string, string>, 只指定一个模板实参
```

### 类模板的静态成员

* 与任何其他类相同, 类模板可以声明`static`成员

* 类模板的每个实例都有自己的`static`成员实例; 所以在类模板外部定义`static`成员时, `static`成员也定义成模板

* 与任何其他`static`数据成员一样, 类模板的`static`数据成员必须有且仅有一个定义

* 与非模板类的静态成员一样, 可以通过类类型对象来访问`static`成员, 也可以使用作用域运算符直接访问

  ```c++
  template <typename T> class Foo {
  public:
      static std::size_t count() {return ctr;}
  private:
      static std::size_t ctr;
  };
  ```
  * `Foo`的每个实例都有自己的`static`成员; 即, 对任意给定类型`X`, `Foo<X>`实例都有一个`ctr`成员和一个`count`成员, 而`Foo<X>`类型的对象共享静态成员.

## 1.3模板形参*

模板形参的名字: 可以使用任何名字, 通常将类型参数命名为`T`

### 模板形参与作用域

模板形参遵循普通的作用域规则. 一个模板形参名的作用域是在其声明之后, 直到模板声明或定义结束之前

* 模板形参会隐藏外层作用域中声明的相同名字

* 在模板内不能重用模板形参名

### 模板声明

一个模板声明必须包含模板形参

* 声明中的模板形参的名字不必与定义中相同
* 模板声明和定义必须有相同数量和种类(即, 类型或非类型)的形参

建议: 

* 通常将某个文件所需的所有模板声明放在文件开头, 这是为了让声明出现在任何使用模板的代码之前
  * 原因见s16.3, p698

### 模板形参与访问类型成员

默认情况下, C++假定模板类型形参通过作用域运算符访问的名字不是类型

* 假设有模板类型形参`T`, `T::mem`在默认情况下被认为是`static`成员, 而不是类型成员

如果希望使用模板类型形参的类型成员, 必须显式告诉编译器该名字是一个类型, 即使用关键字`typename`

* 如`typename T::mem`, 表示`T`的类型成员

### 默认模板实参

C++11允许在函数模板和类模板中提供默认模板实参

* 早期标准只允许在类模板中提供默认模板实参

注意

* 对于一个模板形参, 只有当它右侧的所有形参都有默认实参时, 它才可以有默认实参
* 在使用类模板时必须提供模板实参列表; 如果一个类模板为其所用模板形参都提供了默认实参, 且我们希望使用这些默认实参, 就必须在模板名后跟一对空尖括号, 表示空的模板实参列表
* 在使用函数模板时可以不提供模板实参列表, 这样的话模板形参的类型从对应的函数实参推断而来;

例子

```c++
template <typename T, typename F = less<T>>
int compare(cons T &v1, const T &v2, F f = F())
{
    if (f(v1, v2)) return -1;
    if (f(v2, v2)) return 1;
    return 0;
}
//在某个函数中
bool i = compare(0, 42);
Sales_data item1(cin), item2(cin);
bool j = compare(item1, item2. compareIsbn);
```

* 模板形参`F`有模板默认实参, 是`less<T>`; 函数形参`f`的默认实参是`F()`

```c++
template <class T = int> class Numbers {
public:
    Numbers(T v = 0):val(v){}
private:
    T val;
};
Numbers<long double> lots_of_precision;
Numbers<> average_precision;
```

* 模板形参`T`有模板默认实参, 是`int`

## 1.4成员模板

一个类, 无论是普通类还是类模板, 可以包含本身是模板的成员函数. 这种成员函数被称为成员模板(**member template**)

* 成员模板不能是虚函数

### 普通类的成员模板

与任意其他模板一样, 一个成员模板以`template`关键字和模板形参列表作为开头

* 与任意其他模板一样, 当使用成员模板时, 才会实例化一个函数版本
* 在普通类外定义一个类模板时,  必须为成员模板提供模板形参列表, 同时也要指出它所属的类

例子

```c++
class DebugDelete {
public:
    DebugDelete(std::ostream &s = std::cerr) : os(s) { }
    template <typename T> void operator()(T *p) const;
private:
    std::ostream &os;
};

template <typename T>
void DebugDelete::operator()(T *p) const
{
    os << "deleting unique_ptr" << std::endl; delete p;
}
```

### 类模板的成员模板

可以为类模板定义成员模板

* 此时, 类模板和成员模板各自有属于自己的, 独立的模板形参
* 在类模板外定义一个成员模板时, 必须同时为类模板和成员模板提供模板形参列表
  * 具体是, 类模板的形参列表在前, 成员模板的形参列表在后

在实例化一个类模板的成员模板时, 需要类模板和函数模板的模板实参

* 类模板的模板实参: 编译器根据调用成员模板的对象的类型推断
  * 该对象的类型是类模板的一个实例
* 函数模板的模板实参: 编译器根据传递给成员模板的函数实参推断

```c++
template <typename T>
class Blob {
	template <typename It> Blob(It b, It e);
    //...
};
template<typename T>
template<typename It>
	Blob<T>::Blob(It b, Ite):
		data(std::make_shared<std::vector<T>>(b, e)) { }
```

```c++
int ia[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
vector<long> vi = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
list<const char*> w ={"how", "is", "the", "time"};
Blob<int> a1(begin(ia), end(ia));
Blob<int> a2(vi.begin(), vi.end());
Blob<string> a3(w.begin(), w.end());
```

* 定义`a1`时, 显式提供`int`模板实参以实例化一个`Blob<int>`类型; 然后编译器根据`begin(ia)`的类型推断出构造函数的模板实参为`int*`, 因此实例化了一个`Blob<int>::Blob(int*, int*)`的构造函数版本

## 1.5控制实例化

当模板被使用时才会进行实例化; 这意味着相同的实例可能出现在多个对象文件中.

* 当两个或多个独立编译的源文件使用相同的模板, 并提供相同的模板实参时, 每个文件都会有该模板的一个相同实例

在大型系统中, 在多个文件中实例化相同模板的额外开销可能不容小觑. 在C++11中, 可以通过显式实例化(**explicit instantiation**)来避免这种开销

* 形式: 

  ```c++
  extern template declaration;	//实例化声明
  template declaration;			//实例化定义
  ```

  * `declaration`是类或函数的声明, 其中所有模板形参都被模板实参代替
  * 当编译器遇到`extern`模板声明时, 它不会在该文件中生成对应版本的实例化代码
    * 由于在使用模板时编译器会自动对其实例化, 因此`extern`声明必须出现在任何使用此实例化版本的代码之前, 才能使得编译不在该文件中生成实例化代码
  * 当编译器遇到定义(非`extern`声明)时, 它会为其生成给定版本的实例化代码

  ```c++
  extern template class Blob<string>;				//实例化声明
  template int compare(const int&, const int&);	//实例化定义
  ```

* 说明
  * 将一个实例化声明为`extern`, 表示在程序其他位置有该实例化的定义(非`extern`声明)
  * 对于一个给定的实例化版本, 可以有多个`extern`声明, 但必须只有一个定义(非`extern`声明)

* 注意
  * 模板定义和声明必须出现在实例化声明和定义之前
  * 对于每个实例化`extern`声明, 在程序中某个位置必须有对应的实例; 通常使用显式实例化定义以生成实例
  * 类模板的实例化定义将该模板的所有成员实例化; 因此, 用于显式实例化一个类模板的类型, 必须满足类模板中所有成员的要求

## 1.6效率与灵活p676

以`shared_ptr`和`unique_ptr`为例, 介绍模板设计时的选择

# 2.模板实参推断

对于函数模板, 编译器利用调用中的函数实参来推断其模板实参, 这一过程被称为模板实参推断(**template argument deduction**)

* 编译器使用函数调用中的实参类型来寻找一个这样的模板实参, 该模板实参所生成的函数版本是给定的函数调用的最佳匹配

## 2.1类型转换与模板类型形参*

在函数模板中, 用模板类型形参定义的函数形参有特殊的初始化规则, 只有有限的几种类型转换可以使用

* 能在函数模板的调用中应用于函数实参的类型转换
  * 顶层`const`无论是在函数形参中还是函数实参中可以忽略
  * `const`转换: 无论是顶层`const`还是底层`const`,可以将 非`const`转换为`const`, 反之不然
  * 数组或函数指针转换: 当函数形参不是引用类型时, 则可以对数组或函数类型的实参应用正常的指针转换
* 其他类型转换, 如算术转换, 派生类到基类的转换以及用户定义的转换都不能应用于函数模板

多个函数形参可以同时使用同一个模板类型形参作为类型; 

* 由于允许有限的几种类型转换, 传递给这些形参的函数实参必须具有相同的类型

* 如果由这些函数实参推断出的模板类型实参不一致, 则调用是错误的

注意:

* 在函数模板中, 用普通类型定义的函数形参遵循正常的初始化规则, 可以使用正常的类型转换
* 对于用模板类型形参定义的函数形参`param`, 如果`param`的模板类型形参被显式指定, 与`param`对应的函数实参可以进行正常的类型转换

## 2.2函数模板的显式实参*

显式模板实参(**explicit template argument**), 就是在使用模板时显式提供的模板实参

* 在函数模板中提供显式模板实参的方式与定义类模板实例的方式相同

显式模板实参按由左至右的顺序与对应的模板形参匹配;

* 只可以忽略尾部形参的显式模板实参, 前提是它们可以从函数形参推断出来

注意: 

* 只有出现在函数形参列表中的模板类型形参才能够被编译器推断
  * 因此, 如果模板类型形参只被返回类型使用, 编译器无法推断该模板类型形参

## 2.3尾置返回类型与类型转变

可以在函数模板中使用尾置返回类型

### 标准库的类型转变模板类

类型转变模板(**type transformation template**), 定义在`type_traits`头文件中

* `type_traits`头文件中的类通常用于模板元程序设计(**template metaprogramming**)
* 类型转换模板在普通编程中也很有用

标准类型转换模板

* 这些模板有一个模板类型形参和一个名为`type`的公有类型成员

| For `Mod<T>`, where `Mod` is | If `T` is                    | Then `Mod<T>::type` is |
| ---------------------------- | ---------------------------- | ---------------------- |
| `remove_reference`           | `X&` or `X&&`                | `X`                    |
|                              | otherwise                    | `T`                    |
| `add_const`                  | `X&`, `const X`, or function | `T`                    |
|                              | otherwise                    | `const T`              |
| `add_lvalue_reference`       | `X&`                         | `T`                    |
|                              | `X&&`                        | `X&`                   |
|                              | otherwise                    | `T&`                   |
| `add_rvalue_reference`       | `X&` of `X&&`                | `T`                    |
|                              | otherwise                    | `T&&`                  |
| `remove_pointer`             | `X*`                         | `X`                    |
|                              | otherwise                    | `T`                    |
| `add_pointer`                | `X&` or `X&&`                | `X*`                   |
|                              | otherwise                    | `T*`                   |
| `make_signed`                | `unsigned X`                 | `X`                    |
|                              | otherwise                    | `T`                    |
| `make_unsigned`              | signed type                  | `unsigned type`        |
|                              | otherwise                    | `T`                    |
| `remove_extent`              | `X[n]`                       | `X`                    |
|                              | otherwise                    | `T`                    |
| `remove_all_extents`         | `X[n1][n2]`                  | `X`                    |
|                              | otherwise                    | `T`                    |

## 2.4函数指针与实参推断

当我们将一个函数模板初始化/赋值于一个函数指针, 编译器使用指针的类型来推断模板实参

* 对每个模板形参, 能唯一确定其类型或值

* 如果不能从函数指针类型确定模板实参, 则产生错误

```c++
template <typeaname T> int compare(const T&, const T&);
void func(int (*)(const string&, const string&));
void func(int (*)(const int&, const int&));
func(compare);	//error
```

## 2.5模板实参推断与引用

总结

* 编译器应用正常的引用绑定规则
* `const`引用的`const`是底层的

### 左值引用函数形参

在函数模板中, 当一个函数形参是一个指向模板类型形参的左值引用(`T&`)时, 根据正常的引用绑定规则, 该函数形参只绑定左值

* 当函数实参是`const`时, `T`被推断为`const`类型; 实参不是`const`, `T`也不是

在函数模板中, 当一个函数形参是一个指向模板类型形参的`const`左值引用(`cosnt T&`)时, 该函数形参可以绑定任意类型的实参: 一个对象(`const`或非`const`), 一个临时对象或一个字面值常量值

* 当函数实参本身是`const`时, 并不能推断出`T`是`const`类型

### 右值引用函数形参与引用折叠

在函数模板中, 当一个函数形参是一个指向模板类型形参的右值引用(`T&&`)时, 根据正常的引用绑定规则, 可以传递给它一个右值

* 当函数实参是`const`时, `T`被推断为`const`类型; 实参不是`const`, `T`也不是

正常引用绑定规则的两个例外

* 当将一个左值传递给一个指向模板类型形参的右值引用函数形参(`T&&`), 模板类型形参被推断为实参的左值引用类型	

* 如果间接创建了一个引用的引用, 则这些引用会"折叠"; 
  * 除了一个例外, 引用会折叠成一个普通的左值类型
    * C++11之前的标准中, 所有引用的引用, 都会折叠成一个左值类型
  * (C++11)例外: 右值引用的右值引用, 会折叠成一个右值引用
  * 也就是说, 对于一个给定类型`X`
    * `X& &`, `X& &&`, `X&& &`这三个形式都会折叠成类型`X&`
    * `X&& &&`折叠成类型`X&&`
  * 注意: 引用折叠只能应用于间接创建的引用的引用, 如类型别名或模板形参

```c++
template <typename T> void f(T&&);
int i = 1;
f(i);	
```

* `i`是一个`int`左值; 当调用`f(i)`时, `T`的类型被推断为`int&`
* 此时`f`的函数形参类型是`int& &&`, 折叠成`int&`

根据上面提到的例外和正常引用绑定规则, 可以得到: 当一个函数形参是一个指向模板类型形参的右值引用(`T&&`)时, 该函数形参可以绑定任意类型的实参

* 当实参为右值时, 推断出的模型实参类型是一个非引用类型, 对应的函数形参是右值引用
* 当实参为左值时, 推断出的模型实参类型是一个左值引用类型, 对应的函数形参是左值引用

## 2.6理解`std::move`p690

标准库 `move`函数是一个使用右值引用的模板; 研究`move`是如何工作的可以帮助我们巩固对模板的理解和使用

标准库`move`函数接受任何类型的实参, 返回一个对应的右值引用

* 实参是左值, 则返回一个绑定到左值的右值引用
* 实参是右值, 则返回一个绑定到右值的右值引用

### `std::move`的定义

```c++
template <typename T>
typename remove_reference<T>::type&& move(T&& t)
{
    return static_cast<typename reremove_reference<T>::type&&>(t);
}
```

关于右值引用的类型转换

* 左值不能隐式地转换为右值引用

* 可以使用`static_cast`显式地将一个左值转换为一个右值引用

## 2.7转发

转发(**forward**), 是指以下的过程

* 一个函数`A`将它的一个或多个实参传递给另一个函数`B`; 传递后, 在函数`B`中的实参的所有性质都不变, 包括实参的`const`属性以及实参的左右值属性等等

### 定义能保持类型信息的函数形参

通过将一个函数形参定义为一个指向模板类型形参的右值引用(`T&&`), 可以保持其对应实参的所有类型信息

* 实参的`const`属性: 因为是引用类型, 所以`const`是底层的, 从而保持`const`属性
* 实参的左右值属性: 参考s16.2.5, p688的右值引用函数形参与引用折叠
* 不过, 传递普通类型的左值给形参, 形参会变为引用类型

问题: 函数形参是一个变量; 而变量是左值, 即使其类型是右值引用

* 也就是说, 当转发一个右值时, 对应的函数形参是一个类型为右值引用的左值

### 在调用中使用`std::forward`保持类型信息(C++11)

`forward`, 定义在`utility`头文件中

* 必须提供一个显式模板实参, 返回该显式模板实参类型的右值引用, 即, `forward<T>`的返回类型是`T&&`
* 当用于定义为模板类型形参的右值引用的函数形参时, 通过引用折叠, `forward`可以保持给定的实参类型的所有信息, 包括左右值属性和`const`属性

函数调用表达式的性质

* 当一个函数返回左值引用类型时, 该结果是左值; 函数返回右值引用类型或非引用类型时, 该结果是右值

例子:

```c++
template <typename Type> 
void intermediary(Type &&arg)
{
    finalFcn(st::forward<Type>(arg));
    //...
}
```

# 3.重载和模板

函数模板可以被另一个函数模板或普通函数重载; 与以往一样, 重载函数时, 名字相同的函数必须具有不同的形参列表

涉及函数模板的函数匹配规则在以下方面受到影响

* 候选函数: 包括所有模板实参推断成功的函数模板实例
* 可行函数: 候选的函数模板实例总是可行的, 因为模板实参推断的过程会排除任何不可行的模板
* 与往常一样, 可行函数按类型转换来排序; 值得注意的是, 函数模板可调用的类型转换是非常有限的
* 与往常一样, 如果只有一个函数提供比任何其他函数都更好的匹配, 则选择此函数; 
  * 如果有多个函数提供同样好的匹配, 则
    * 如果其中只有一个非模板函数, 则选择此函数
    * 如果其中没有非模板函数, 而有多个函数模板, 且某一个模板比其他模板更特例化, 则选择此模板
    * 否则, 该调用有歧义的

## 示例: 编写重载模板p695

# 4.可变模板

可变模板(**variadic template**), 是模板形参数目可变的模板函数或模板类

参数包(或形参包, **parameter pack**)是指可变模板中可变数目的形参

* 有两种参数包
  * 模板参数包(或模板形参包, **template parameter pack**), 表示零个或多个模板形参
  * 函数参数包(或函数形参包, **function parameter pack**), 表示零个或多个函数形参
* 在模板形参列表中使用省略号`...`指出一个模板形参或函数形参表示一个包

  *  `class...`或`typename...`指出接下来的形参表示一个包含零个或多个类型的列表
  * 类型后跟`...`指出接下来的形参表示一个包含零个或多个给定类型的非类型形参的列表
* 在函数模板的函数形参列表中, 如果一个函数形参的类型是模板参数包, 则该函数形参是一个函数参数包;
  * 编译器在推断模型形参的类型时, 也会推断参数包中形参的数目

`sizeof...`运算符(C++11)

* 返回一个常量表达式, 表示参数包中形参的数目
  * 用于模板参数包时, 表示模板形参的数目
  * 用于函数参数包时, 表示函数形参的数目

## 4.1编写可变函数模板

> 当我们不知道要处理的实参的数目和类型时, 可变参数函数是很有用的

可变参数函数通常是递归的; 

* 在每次递归中处理包中的一个实参
* 为了终止递归, 还会定义一个非可变的函数版本

```c++
template <typename T>
ostream &print(ostream &os, const T &t)
{
    return os << t;
}

template <typename T, typename... Args>
ostream &print(ostream &os, const T &t, const Args&... rest)
{
    os << t << ", ";
    return print(os, rest...);
}
```

* `rest...`表示包扩展

注意:

* 非可变形参版本的声明放在可变形参版本前; 这样的话, 调用可变形参版本时, 非可变形参版本是可见的

## 4.2包扩展

对于一个参数包, 除了能够获取其大小外, 还能扩展(**expand**)它.

包扩展

* 扩展一个包, 是指将包分解为一个个元素, 然后在每个元素上使用模式
  * 当扩展一个包时, 需要提供一个在每个元素上使用的模式(**pattern**)
* 在模式右边放一个省略号, 表示扩展操作

例子

```c++
template <typename T, typename... Args>
ostream &print(ostream &os, const T &t, const Args&... rest)
{
    os << t << ", ";
    return print(os, rest...);
}
```

* 第一次扩展发生在函数形参列表中的`const Args&... rest`处, 为了`print`生成零个或多个函数形参
  * `Args`是模板类型形参包; 扩展`Args`时, `Args`中的每个元素(即类型)被用于模式`const Args&`中
  * 因此, `const Args&...`的扩展结果是一个逗号分隔的零个或多个类型的列表, 且每个类型都形如`const type&`
* 第二次扩展发生在返回语句中的 `print(os, rest...)`处, 为`print`调用生成了实参列表
  * `rest`是一个函数形参包, 表示零个或多个函数形参
  * 扩展`rest..`的结果是一个逗号分隔的零个或多个实参的列表, 且每个实参都形如`arg`

### 理解包扩展p703

## 4.3转发形参包(C++11)

C++11允许组合使用可变模板和`forward`, 实现将参数包转发给其他函数

例子: 在`StrVec`中实现一个类似容器的`emplace_back`成员的函数

```c++
template <class... Args>
inline
void StrVec::emplace_back(Args&&... args)
{
    chk_n_alloc;
    alloc.construct(first_free++, std::forward<Args>(args)...);
}
```

* `std::forward<Args>(args)...`, 同时扩展了`Args`和`args`两个包

可变参数函数通常将它们的形参转发给其他函数, 这种函数通常具有与上述函数一样的形式:

# 5.模板特例

模板特例(**template specialization**)是某个模板的一个分离的定义, 在该定义中一个或多个模板形参已被指定为特定的类型

## 函数模板特例

当我们特例化一个函数模板时, 必须为原模板中的每个模板形参提供一个实参

* 为了表明这是一个函数模板特例, 应使用关键字`template`并后跟一个空尖括号`<>`; 空尖括号表示已经为原模板的所有模板形参提供实参

定义一个函数模板特例时, 原模板的声明必须在作用域中, 而且函数形参的类型必须与原模板中对应的类型匹配

如: 

```c++
template <typename T> int compare (const T&, const T&);
template <> int compare (const char * const &, const char * const &)
{
    return strcmp(p1, p2);
}
```

### 函数重载与模板特例

当定义函数模板特例时, 我们其实是为原模板的一个特定实例提供定义; 也就是说, 一个特例本质上是原模板的实例, 而不是该函数名的一个重载版本

因此, 模板特例不会影响函数匹配

## 模板特例与声明

建议: 模板及其特例应该声明在同一个头文件中; 所有同名模板的声明应该放在前面, 然后是这些模板的特例放在原模板的后面

说明:

* 在任何使用模板实例的代码之前, 特例的声明也应该在作用域中

* 在打算使用模板特例的代码中, 如果当前作用域中只有原模板的声明, 没有特例的声明, 编译器将使用原模板生成实例

## 类模板特例

当特例化一个类模板时, 可以为原模板中的每个模板形参提供一个实参, 也可以只为部分的模板形参提供实参

* 在类名后中的`<>`尖括号中指定实参, 实参与原始模板中的形参按位置对应
* 如果为原模板中的每个模板形参提供一个实参, 则特例的模板形参列表为空

* 在定义类模板的特例时, 可以在特例的外部或内部定义成员

例子: 为`Sales_data`定义一个标准库`hash`的特例

```c++
namespace std {
	template <>
	struct hash<Sales_data>
	{
 	   typedef size_t result_type;
 	   typedef Sales_data argument_type;
	   size_t operator()(const Sales_data& s) const;
	};
	size_t
	hash<Sales_data>::operator()(const Sales_data& s) const
	{
 	   return hash<string>()(s.bookNo) ^
	          hash<unsigned>()(s.units_sold) ^
 	          hash<double>()(s.revenue);
	}	
}

template <class T> class std::hash;
class Sales_data {
    friend class std::hash<Sales_data>;
    //other member as before
};
```

### 类模板的局部特例化

特例化类模板时, 可以只指定一部分模板形参, 或者形参的一部分特性

* 在类名后的`<>`中指定实参, 实参与原始模板中的形参按位置对应
* 在局部特例的模板形参列表中必须包含每个未完全确定类型的模板形参

类模板局部特例(**class template partial specialization**), 本身也是一个模板; 因此用户在使用局部特例时, 必须为未被指定的模板形参提供实参

例子: 标准库中的`remove_reference`类型

```c++
//原始的, 通用的版本
template <class T> struct remove_reference {
    typedef T type;
};
//用于左值和右值引用的局部特例
template <class T> struct remove_reference<T&> {
    typedef T type;
};
template <class T> struct remove_reference<T&&> {
    typedef T type;
};
```

### 特例化成员

可以只特例化成员函数而不是特例化整个类模板