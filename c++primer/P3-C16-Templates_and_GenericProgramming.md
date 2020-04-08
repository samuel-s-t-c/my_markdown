# 总结

泛型编程与模板

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

模板形参遵循普通的作用域规则.

一个模板形参名的可用范围是在其声明之后, 直到模板声明或定义结束之前

模板形参会隐藏外层作用域中声明的相同名字

## 1.4成员模板

## 1.5控制实例化

## 1.6效率与柔性

# 2.模板实参推断

## 2.1类型转换与模板类型形参

## 2.2函数模板的显式实参

## 2.3尾置返回类型与类型转换

## 2.4函数指针与实参推断

## 2.5模板实参推断与引用

## 2.6`std::move`

## 2.7转发

# 3.重载和模板

# 4.可变模板

## 4.1编写可变函数模板

## 4.2包扩展

## 4.3转发形参包

# 5.模板特例化