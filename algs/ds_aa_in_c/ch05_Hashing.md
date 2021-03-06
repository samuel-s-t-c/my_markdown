# 总结

* 哈希表/散列表

[toc]

# 1 基本概念

散列表数据结构

* 理想的散列表是一个数组, 其元素是某种固定大小的数据结构
  * 该数据结构包含键(key); 通常, key是带有相关值的字符串

* 散列表的大小, 是散列表数据结构的一部分
  * 若表大小为$TableSize$, 则通常的惯例是表在$0$到${TableSize} - 1$之间工作

散列函数

* 将键key映射到某个数值中, 然后利用该数值在散列表中选择合适的单元
* 要求: 该函数在一定的范围中均匀地分配键key

问题:

* 如何选择散列函数
* 若两个键散列到同一值的时候(称为冲突collision), 如何处理
* 如何确定散列表的大小

# 2 散列函数

键Key是整数时, 

* 通常情况下, 散列函数可以直接返回${Key}\mod {TableSize}$的结果
* 若Key具有某些不理想的属性, 需要仔细考虑如何选择散列函数
  * 如表的大小是10, 而Key都是以0为个位数
* 建议的做法: 保证表的大小是素数, 然后使用上面的函数
  * 当输入的键是随机整数时, 该散列函数不仅计算简单, 而且键的分配很均匀

键Key是字符串时, 

* 将字符串中字符的ASCII码值相加, 然后使用整数的散列方式

  * 容易实现, 计算简单; 但若表很大, 该函数不能很好地分配键Key

  * ```c
    typedef unsigned int Index;
    Index Hash(const char *Key, int TableSize)
    {
        unsigned int HashVal = 0;
        while (*Key != '\0')
            HashVal += *Key++;
        return HashVal % TableSize;
    }
    ```

* 这种方法假设键至少有两个字符和一个空终止字符, 然后只检验前三个字符

  * 容易实现, 计算简单; 但分配不均匀

  * $27$表示26个英语字母加上1个空白符, $729$是${27}^2$

  * ```c
    Index Hash(const char *Key, int TableSize)
    {
        return (Key[0] + 27 * Key[1] + 729 * Key[2]) % TableSize;
    }
    ```

* 第三种方法: 使用Horner法则计算$\sum_{i = 0}^{{KeySize}-1}Key[{KeySize}-i-1]\times32^i$(基于32的多项式), 并将结果限制在适当的范围内

  * 一般情况可以很好地分配键
  * 若键很长, 该散列函数会花费过多的时间; 因此, 通常的做法是只使用部分的字符, 但这样做对键的分布有影响
  * 使用32代替27, 是为了提高速度, 因为使用移位操作

  * ```c
    Index Hash(const char*Key, int TableSize)
    {
        unsigned int HashVal = 0;
        while (*Key != '\0')
            HashVal = (HashVal << 5) + *Key++;
        return HashVal % TableSize;
    }
    ```

# 3 分离链接法Separate Chaining

解决冲突的一种方法: 分离链接法

* 做法: 将散列到相同值的所有元素放在一个列表中
  * 具体实现通常是放在链表中
  * 除了链表外, 也可以使用二叉查找树甚至另外一个散列表
* 如果散列表很大, 散列函数很好, 那么里面的列表应该很短

散列表的装填因素(load factor)$\lambda$

* 定义: 散列表中的元素数量与表大小的比值, $\frac{ElementNumber}{TableSize}$

* 使用分离链接散列时, 尽量让$\lambda \approx 1$, 即尽量让表的大小等于元素的数量

分析:

* 在一次不成功的查找中, 遍历的链接的数量是$\lambda$(不包括最后的NULL链接)
* 在一次成功的查找中, 遍历的链接的数量大概是$1+(\lambda/2)$

# 4 开放定址法Open Addressing

解决冲突的一种方法: 开放定址法

* 若发生冲突, 尝试为其中一个键选择另外的单元, 直到找出空单元或者确定没有空单元为止
  * 更正式地说, 若发生冲突时, 依次尝试$h_0(X), h_1(X), h_3(X),..$等单元
  * 其中$h_i(X)=(Hash(X)+F(i))\mod TableSize$, 且$F(0) = 0$
  * 函数$F$是某种冲突解决策略
* 一般来说, 使用开放定制的散列表的装填因素$\lambda \le 0.5$

## 4.1 线性探测法

在线性探测法中, 函数$F$是$i$的线性函数

* 典型的函数有: $F(i)=i$

* 存在primary clustering(一次聚集)问题: 即使散列表中有相对多的空单元, 所选择的单元会聚集在一起, 导致性能降低

分析

* 对于插入和不成功的查找来说, 使用线性探测的次数的预期值是$\frac{1}{2}(1+1/(1-\lambda)^2)$
* 对于成功的查找来说, 使用线性探测的次数的预期值是$\frac{1}{2}(1+1/(1-\lambda))$

## 4.2 平方探测法

* 典型的平方探测函数是: $F(i)=i^2$
* 平方探测消除了一次聚集问题,  但存在secondary clustering(二次聚集)问题: 散列成相同值的那些元素将探测相同的备选单元
  * 模拟结果指出, 每次查找一般会需要额外的少于一半的探测

定理:

* 若使用平方探测法,  且表的大小是素数, 那么当表至少有一半是空的时候, 总能够插入一个新的元素

## 4.3 双散列

* 典型的双散列函数是: $F(i)=i\cdot {hash}_2(X)$
  * ${hash}_2(X)$表示对$X$的第二个散列函数
* ${hash}_2(X)$的要求: 
  * 不能得出0值
  * 如果可能, 保证所有的单元都能被探测到
  * 例子: ${hash}_2(X)=R-(X\mod R)$; 其中, $R$是一个比$TableSize$小的素数

# 5 再散列Rehashing

问题: 若散列表填得太满, 操作的运行时间将会开始变长; 对于使用平方探测的开放定址的散列表, 插入操作可能失败

* 可能发生在有太多的移除和插入混合使用的场合

一种解决方法--再散列

* 做法: 建立另外一个大约原来两倍大的散列(使用相对应的新的散列函数), 然后扫描整个原来的散列表, 计算新的散列值并将其插入到新的散列表中

* 分析
  * 运行时间是$O(N)$

# 6 可扩展散列Extendible Hashing

问题:

* 数据量太大, 大到不能放进主存; 此时, 主要关心的是检索数据所需的磁盘存取次数
* 假设在任一时刻都有N个记录要存储, N的值会随时间而变化; 此外, 最多可把M个记录放入一个磁盘区块中
  * 以下, 假设M=4

一种解决方案: 可扩展散列

* 做法: P169

* 分析:

  * 查找操作需要两次磁盘访问; 插入操作也只需很少的磁盘访问

  * 叶节点的期望个数是$(N/M)\log_2e$

  * 目录的期望个数是$O(N^{1+1/M}/M)$

    