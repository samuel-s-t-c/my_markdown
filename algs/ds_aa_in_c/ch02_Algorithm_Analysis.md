# 总结

* 算法分析的数学基础
* 算法分析的系统构架

[toc]

# 1.数学基础

四个定义

* $T(N) = O(f(N))$ if there are positive constants $c$ and $n_0$ such that $T(N) \le cf(N)$ when $N \ge n_0$
* $T(N) = \Omega(g(N))$ if there are positive constants $c$ and $n_0$ such that $T(N) \ge cg(N)$ when $N \ge n_0$
* $T(N) = \Theta(h(N))$ if and only if $T(N) = O(h(N))$ and $T(N) = \Omega(h(N))$
* $T(N) = o(p(N))$ if  $T(N) = O(p(N))$ and $T(N) \ne \Theta(p(N))$

三个法则

* If $T_1(N) = O(f(N))$ and $T_2(N) = O(g(N))$, then
  * $T_1(N) + T_2(N) = max(O(f(N)), O(g(N)))$
  * $T_1(N) * T_2(N) = O(f(N) * g(N))$
* If $T(N)$ is a polynomial of degree $k$, then $T(N) = \Theta(N^k)$
* $log_2^kN = O(N)$ for any constant k

使用洛必达法则确定两个函数的相对增长率: ${\lim_{N\to\infty}} \frac{f(N)}{g(N))}$

* 极限是0: $f(N) = o(g(N))$
* 极限是c$\ne 0$: $f(N) = \Theta(g(N))$
* 极限是$\infty$: $g(N) = o(f(N))$
* 极限摆动: 二者无关

# 2.计算模型

* 标准的简单指令集合, 如加法, 乘法, 比较和赋值
* 每一个时间单元执行一条简单指令
* 按顺序执行指令
* 有定长的整数
* 没有需要多个时间单元的指令, 如矩阵求逆, 排序等等
* 磁盘读取的速度消耗一个时间单元
* 无限的内存

# 3.分析要点

需要分析的通常是运行时间, 而影响程序的运行时间的几种因素有

* 如编译器和计算机等的因素(不在本书的讨论范围之内)
* 算法和输入
  * 对于输入, 最主要的关注点是输入的大小

在大小为$N$的输入下使用某种算法时, 

* $T_{avg}(N)$表示平均运行时间
* $T_{worst}(N)$表示最坏情况下的运行时间
* 可得 ,$T_{avg}(N) \le T_{worst}(N)$

* 若有多个输入时, 这些函数可以多个相对应的参数

在分析算法时, 通常所需要的是最坏情况下的运行时间; 除非另有说明

* 原因:
  * 该运行时间提供了一个所有输出的上界, 包括特别差的输入
  * 相对来说, 很难计算得出平均情况下的运行时间; 而且"平均"的定义也影响结果

# 4.计算运行时间

有几种估计程序运行时间的方式

* 经验法
* 直接运行并记录时间

为了简化分析, 本书没有特定的时间单位, 因此

* 忽略了首项的常量系数
* 忽略了低价项
* 其实就是计算大O运行时间

## 4.1简单例子

计算$\sum_{i=1}^{N}i^3$的程序段如下

```c
		int Sum(int N) {
   			int i, PatialSum;
/*1*/		PartialSum = 0;
/*2*/		for (i = 1; i <= N; i++)
/*3*/			PartialSum += i * i * i;
/*4*/		return PartialSum;
		}
```

分析:

* 声明语句在不占用运行时间
* 行1和4都消耗一个时间单位
* 行3在每次执行时消耗4个时间单位(两个乘法, 一个加法, 一个赋值); 一共执行了N次; 因此共消耗了4N个时间单位
* 行2消耗了$2N+2$个时间单位; 初始化变量为1, 测试操作为$N+1$, 递增操作为$N$

总结

* 忽略函数调用和返回的开销, 该函数共消耗$6N+4$个时间单位; 因此, 该函数是$O(N)$

## 4.2常用法则

1. for循环
   * for循环的运行时间至多是该循环内语句(包括测试语句)的运行时间乘以迭代次数
2. 嵌套for循环
   * 从里向外分析; 若一条语句在一组嵌套循环内部中, 其运行时间是该语句的运行时间乘以所有for循环的大小
3. 连续语句
   * 各语句的运行时间相加即可; 也就是说, 由关键语句决定
4. if/else语句
   * 至多是判断语句的运行时间加上最耗时的分支的运行时间

## 4.3最大子序列和

最大子序列和

* 给定(可能为负的)整数$A_1, A_2, ..., A_N$, 找出$\sum_{k =i}^{j}A_k$的最大值

* 方便起见, 若所有整数为负, 最大子序列和应为0
* 示例: 
  * 输入: $-2, 11, -4, 13, -5, -2$
  * 输出: $20$($A_2\sim A_4$)

算法1: 穷举, $O(N^3)$

```c
int MaxSubsequenceSum1(const int A[], int N)
{
  int ThisSum, MaxSum, i, j, k;
  MaxSum = 0;
  for (i = 0; i < N; i++) {
    for (j = i; j < N; j++) {
      ThisSum = 0;
      for (k = j; k <= j; k++)
        ThisSum += A[k];
      if (ThisSum > MaxSum)
        MaxSum = ThisSum;
    }
  }
  return MaxSum;
}
```

算法2: 对算法1的改进, $O(N^2)$

```c
int MaxSubSequenceSum2(const int A[], int N)
{
  int ThisSum, MaxSum, i, j;
  MaxSum = 0;
  for (i = 0; i < N; i++) {
    ThisSum = 0;
    for (j = i; j < N; j++) {
      ThisSum += A[j];
      if (ThisSum > MaxSum)
        MaxSum = ThisSum;
    }
  }
  return MaxSum;
}
```

算法3: 分治, $O(N\log_2 N)$

```c
static int MaxSubSum(const int A[], int Left, int Right)
{
  int MaxLeftSum, MaxRightSum;
  int MaxLeftBorderSum, MaxRightBorderSum;
  int LeftBorderSum, RightBorderSum;
  int Center, i;

  if (Left == Right) {
    if (A[Left] > 0)
      return A[Left];
    else
      return 0;
  }

  Center = (Left + Right) / 2;
  MaxLeftSum = MaxSubSum(A, Left, Center);
  MaxRightSum = MaxSubSum(A, Center + 1, Right);

  MaxLeftBorderSum = 0; LeftBorderSum = 0;
  for (i = Center; i >= Left; i--) {
    LeftBorderSum += A[i];
    if (LeftBorderSum > MaxLeftBorderSum)
      MaxLeftBorderSum = LeftBorderSum;
  }

  MaxRightBorderSum = 0; RightBorderSum = 0;
  for (i = Center + 1; i <= Right; i++) {
    RightBorderSum += A[i];
    if (RightBorderSum > MaxRightBorderSum)
      MaxRightBorderSum = RightBorderSum;
  }
    
//The pseudoroutine Max3 returns the largest of the three possibilities
  return Max3(MaxLeftSum, MaxRightSum,				
              MaxRightBorderSum + MaxLeftBorderSum);
}

int MaxSubSequenceSum3(const int A[], int N)
{
  return MaxSubSum(A, 0, N-1);
}

```

算法4: online algorithm联机算法

```c
int MaxSubSequenceSum4(const int A[], int N)
{
  int ThisSum, MaxSum, i;
  for (i = 0; i < N; i++) {
    ThisSum += A[i];
    if (ThisSum > MaxSum)
      MaxSum = ThisSum;
    else if (ThisSum < 0)
      ThisSum = 0;
  }
  return MaxSum;
}
```

## 4.4运行时间中的对数

某些分治算法将以$O(\log_2 N)$时间运行

除了分支算法外, 可将对数最常出现的规律概况如下

* 如果一个算法用常数时间($O(1)$)将问题的大小划分为其一部分(通常是$\frac{1}{2}$), 那么该算法是$O(N\log_2 N)$的
* 如果一个算法使用常数时间只是将问题的大小减少一定的数量, 则该算法是$O(N)$的
* 此外, 只有一些特殊种类的问题才能有$O(\log_2 N)$的算法; 对于这类问题, 通常都是假设已经提前读入数据.

以下三个例子都是具有对数特点的算法

### 对分查找/二分查找binary search

问题:

* 给出一个整数$X$和一组已排序的整数$A_0, A_1, ..., A_{N-1}$(已在内存中), 求使得$A_i = X$时的$i$; 若$X$不在数据中, 返回$i = -1$

二分查找算法, $O(\log_2 N)$

```c
#define NotFound -1
int BinarySearch(const int A[], int x, int N)
{
  int Low, Mid, High;
  Low = 0; High = N-1;
  while (Low <= High)
    {
      Mid = (Low + High) / 2;
      if (A[Mid] < x)
        Low = Mid + 1;
      else if (A[Mid] > x)
        High = Mid - 1;
      else return Mid;
    }
  return NotFound;
}
```

### 欧几里德算法Euclid's Algorithm

问题:

* 计算两个整数的最大公因数(the greatest common divisor, Gcd)

算法: $O(\log_2 N)$

```c
unsigned int Gcd(unsigned int M, unsigned int N)
{
  unsigned int Rem;
  while (N > 0) {
    Rem = M % N;
    M = N;
    N = Rem;
  }
  return M;
}
```

定理:

* 如果$M \gt N$, 则$M\mod N \lt M/2$

### 幂运算

问题:

* 给定整数$X$和$N$, 计算$X^N$

算法: 假设该机器能够存储足够大的整数

```c
long int Pow(long int X, unsigned int N)
{
    if (N == 0)
        return 1;
    if (N == 1)
        return X;
    if (IsEven(N))
        return Pow(X * X, N / 2);
    else
        return Pow(X * X, N / 2) * X;
}
```

## 4.5检验方法

一种检验分析的方法

* 编程并记录实际的运行时间

* 比较观察实际的运行时间与分析所描述的运行实际是否相匹配
* 将N扩大一倍, 然后观察实际运行实际的变化
  * $O(N)$: 运行时间乘以2
  * $O(N^2)$: 乘以4; $O(N^3)$: 乘以3
  * $O(\log_2 N)$: 运行时间多加一个常数
  * $O(N\log_2 N)$: 运行时间是原来的两倍多一些
* 若低价项的系数相对地大, 而$N$又不是足够大, 那么运行时间的变化量很难观察清楚

另一种常用的技巧: 对于$N$的某个范围, 计算比值$T(N)/f(N)$

* $T(N)$是凭经验观察到的运行时间, $f(N)$是分析的运行时间

* 若$f(N)$是程序的运行时间的理想近似, 则比值收敛于一个正的常数
* 若$f(N)$估计过大, 则比值收敛于0
* 若$f(N)$估计过低且错误, 则比值发散