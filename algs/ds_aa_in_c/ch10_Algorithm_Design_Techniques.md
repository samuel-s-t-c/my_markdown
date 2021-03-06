# 总结

五种常见的算法类型

[toc]

# 1.贪婪算法Greedy algorithm

贪婪算法

* 贪婪算法的策略是: 使用当前所有信息来选择

* 贪婪算法分阶段工作: 在每个阶段中, 选择该阶段上最优的决定, 而不考虑将来的后果, 也就是说选择局部最优(local optimum)的决定
* 若最终得到的局部最优就是全局最优(global optimum), 该算法是正确的; 否则, 该算法得到一个次优解(suboptimal solution)
  * 如果不要求绝对最佳的答案, 使用简单的贪婪算法以得到近似答案, 不妨是不错的选择

* 算法示例: Dijkstra算法, Prim算法, Kruskal算法

接下来考查使用贪婪算法的应用

## 调度问题: 平均完全时间

问题: 

* 给定作业$j_1,j_2,...j_N$和对应的运行时间$t_1,t_2,...t_N$, 还有一个处理器; 问: 怎么样调度使得平均完成时间最小
* 一个作业的完成时间, 是作业的等待时间加上运行时间

假设: 

* 非抢占调度(nonpreemptive scheduling): 一旦开始某个作业, 该作业将一直运行至完成

最优的调度: 按照最短运行时间进行安排

* 若两个作业运行时间相同, 这两个作业的顺序不会影响平均完成时间

* 证明: 
  * 假设有作业调度队列$j_{i_1},j_{i_2},...,j_{i_N}$和对应的运行时间$t_{i_1},t_{i_2},...,t_{i_N}$; 那么, 第一个作业的完成时间是$t_{i_1}$, 第二个作业的完成时间是$t_{i_1}+t_{i_2}$, 依次类推.
  * 可得: 完成时间之和$C=\sum_{k=1}^N(N-k+1)t_{i_k}=(N+1)\sum_{k=1}^Nt_{i_k}-\sum_{k=1}^Nk\cdot t_{i_k}$
  * 其中$(N+1)\sum_{k=1}^Nt_{i_k}$部分不受作业顺序影响; $\sum_{k=1}^Nk\cdot t_{i_k}$部分表示运行时间大的作业越靠后, 该部分的结果越大, 从而$C$越小

### 多处理器下的情形

问题: 

* 给定作业$j_1,j_2,...j_N$和对应的运行时间$t_1,t_2,...t_N$, 还有$P$个处理器; 问: 怎么样调度使得平均完成时间最小

假设:

* 非抢占调度(nonpreemptive scheduling): 一旦开始某个作业, 该作业将一直运行至完成

最优的调度: 按照最短运行时间进行安排, 处理器之间轮换分配作业

* 若有$N$个作业, 对于每一个$0\le i\lt N/P$, 从$j_{i\cdot P+1}$到$j_{(i+1)\cdot P}$的作业分别被分配到不同的处理器中

#### 最后完成时间

问题: 

* 给定作业$j_1,j_2,...j_N$和对应的运行时间$t_1,t_2,...t_N$, 还有$P$个处理器; 问: 怎么样调度使得最后一个作业的结束时间最小

说明

* 该问题是NP完全的, 只是背包问题或装箱问题的另一种表述方式

## 文件压缩: 哈夫曼编码Huffman Codes

背景:

* ASCII字符集: 一个ASCII字符需要8位, 其中第8位是奇偶校验位, 有7位用于区分字符
* 若一个字符集有$C$种字符, 则标准字符编码需要$\lceil\log_2C\rceil$个比特
* 文件的字母表(alphabet, 符号表), 是指该文件所包含的字符集合; 每种字符在字母表上出现一次

问题:

* 压缩文件

* 在现实中, 许多大文件是某些程序的输出, 而在使用频率最高和最低的字符之间通常存在很大的差别

策略: 

* 编码长度在不同字符之间是不等的, 保证使用频率高的字符的编码短

字典树(trie, 前缀树)

* 根节点不包含信息, 其他节点都只包含一份信息
  * 信息, 可以是字符, 或数字等
* 从根节点到某一节点, 路径上所经过的信息合并起来, 得到该节点所表示的数据
  * 如果信息是字符, 该节点表示字符串
  * 如果信息是0或1, 该节点可表示一个二进制数
* 每个节点的所有子节点所包含的信息都不相同

文件字母表的二进制编码表示成字典树

* 该字典树的节点最多只有两个子节点; 左分支表示0, 右分支表示1
* 每个字符用叶节点表示; 从根节点到某个叶节点的路径, 表示该字符的二进制编码
  * 使用叶节点表示的原因: 若某个字符$c_1$是另一字符$c_2$的父节点, 则$c_1$的二进制编码是$c_2$的二进制编码的前缀,  导致编码有二义性
* 设字符$c_i$对应的叶节点在深度$d_i$, 且在文件中出现$f_i$次, 则文件编码的成本是$\sum d_if_i$; 此时, 文件压缩问题, 在于找到总成本最小的满字典树

* 最优编码对应的字典树总是满树(full tree), 也就是说, 所有节点要么是叶节点, 要么有两个子节点; 原因是: 
  * 叶节点表示字符; 而有子节点的节点是二进制编码的一部分;
  * 若某个节点只有一个子节点, 可以移除该节点, 使得路径变短的同时不会有二义性

### 哈夫曼算法

算法描述: 假设文件字母表大小为$C$, 维护一个森林; 其中, 树的权重值等于它的叶节点的频率之和

* 初始时, 有$C$棵单节点的树, 即每个字符有一棵树

* 以下过程进行$C-1$次: 以最小权重来选取两棵树$T_1$和$T_2$; 合并成一个新树, 其子树为$T_1$和$T_2$
  * 若有多个树的权重值最小且相同, 任意选取其中两个
* 在结束时得到一棵树, 也就是最优哈夫曼编码树

注意:

* 在压缩文件的开头必须要传送编码信息, 用于译码
* 该算法需要扫描两遍, 第一遍是搜集频率数据, 第二遍进行编码

## 装箱问题

装箱问题(bin packing problem)的基本问题

* 给定$N$件物品, 大小分别为$s_1,s_2,...,s_N$, 所有大小满足$0\lt s_i\le1$
* 已知每个箱子的容量是1个单位, 问怎么把这些物品装箱使得箱子的数量最小

装箱问题的两种版本

* 联机装箱问题: 必须在前一件物品放入箱子后才能处理下一件物品
* 脱机装箱问题: 可以在所有物品都能够装箱后, 才进行装箱
  * 可以在所有输入数据全被读入后才进行操作

### 联机算法

> 由于联级算法无法知道输入何时结束, 它提供的任何性能保证必须在任一时刻成立

定理: 若最优箱子数为$M$, 存在一些输入, 使得任一联机装箱算法的箱子数至少是$\frac{4}{3}M$

* 证明:  p358

其中有三种策略和算法:

* next fit: 在把一件物品装进一个箱子$b$后, 检查下一件物品是否能够装进刚刚那个箱子$b$; 若能, 放入箱子$b$; 若不能, 创建一个新箱子
  * 线性运行时间
  * 若最优箱子数为$M$, 则next fit算法不会使用超过$2M$个箱子; 在某些输入下会使用$2M-2$个箱子
* first fit: 在处理一件物品时, 按序扫描箱子, 并放入第一个有足够空间的箱子; 若没有这样的箱子, 创建一个新箱子
  * $O(N\log_2N)$
  * 若最优箱子数为$M$, 则first fit算法不会使用超过$\lceil\frac{17}{10}M\rceil$箱子; 在某些输入下会使用$\frac{17}{10}(M-1)$个箱子
* best fit: 在处理一件物品时, 放入有足够空间且最满的箱子; 若没有这样的箱子, 创建一个新箱子
  * $O(N\log_2N)$
  * 若最优箱子数为$M$, 则best fit算法不会使用超过$\lceil\frac{17}{10}M\rceil$箱子

### 脱机算法

其中有两种算法

* first fit decreasing: 将物品从大到小排序, 然后依次应用first fit
* best fit decreasing: 将物品从大到小排序, 然后依次应用best fit

# 2.分而治之Divide and Conquer(未完)

分治算法由两部分组成

* 分(divide): 递归解决更小的问题, 除了基础情形
* 治(conquer): 根据子问题的答案得出父问题的答案

说明:

* 通常, 分治算法是指至少包含两个递归调用的例程; 而只含有一个递归调用的例程, 不是分治算法

* 通常认为分治过程中的子问题是不相交的

分治算法的例子

* 在2.4.3节中最大子序列和问题的$O(N\log_2N)$算法
* 第4章中, 线性时间的树遍历策略
* 第7章中, 归并排序和快速排序

使用分治思路但不算是分治算法的例子

* 在1.3节中打印数字的简单例程
* 第2章中, 使用递归来执行高效求幂
* 第4章中, 一种简单的二叉查找树的查找例程

## 分治算法的运行时间

定理: 方程$T(N)=aT(\frac{N}{b})+\Theta(N^k)$, 其中$a\ge1, b\gt1$, 的解是
$$
T(N)=\left\{
\begin{aligned}
&O(N^{log_ba})&if\;a\ge b_k\\
&O(N^k\log_2N)&if\;a=b^k\\
&O(N^k)&if\;a\lt b^k
\end{aligned}
\right.
$$
定理: 方程$T(N)=aT(\frac{N}{b})+\Theta(N^k\log_2^pN)$, 其中$a\ge1, b\gt1,p\ge0$, 的解是
$$
T(N)=\left\{
\begin{aligned}
&O(N^{log_ba})&if\;a\ge b_k\\
&O(N^k\log_2^{p+1}N)&if\;a=b^k\\
&O(N^k\log_2^pN)&if\;a\lt b^k
\end{aligned}
\right.
$$

## 最近点问题

## 选择问题

## 一些运算问题的理论改进

# 3.动态规划Dynamic Programming(未完)

> 在数学上可以递归表示的问题, 也可以表示成一个递归算法; 而且在许多情况下, 该递归算法的性能比朴素的穷举搜索的有显著改进

问题:

* 任何数学递归公式都可以直接翻译成递归算法, 但是编译器常常不能正确对待递归算法, 从而生成低效程序

动态规划

例子: 计算
$$
\left\{
\begin{aligned}
&C(0)=1\\
&C(N)=(2/N)\sum_{i=0}^{N-1}C(i)+N &if\;N\gt0\\
\end{aligned}
\right .
$$

```c
// 递归实现
double Eval(int N)
{
    int i;
    double Sum;
    
    if (N == 0)
        return 1.0;
    else {
        Sum = 0.0;
        for (i = 0; i < N; i++)
            Sum += Eval(i);
        return 2.0 * Sum / N + N;
    }
}

// 使用一个表
double Eval(int N)
{
    int i, j;
    double Sum, Answer;
    double *C;
    
    C = malloc(sizeof(double) * (N + 1));
    if (C == NULL)
        FatalError("Out of space!!!");
    
    C[0] = 1.0;
    Sum = 1.0;
    for (i = 1; i <= N; i++) {
        C[i] = 2.0 * Sum / i + i;
        Sum += C[i];
    }
    
    Answer = C[N];
    free(C);
    
    return Answer;
}
```

# 4.随机化算法Randomized Algorithm(未完)

随机化算法

* 在算法过程中, 把随机数用于决策
* 随机化算法的运行时间既取决于具体输入, 也取决于得到的随机数

大致可分为

* 数值随机化算法
* 蒙特卡罗算法
* 拉斯维加斯算法
* 舍伍德算法

# 5.回溯算法Backtracking Algorithm(未完)

回溯算法

* 一种选优搜索算法; 按选优条件向前搜索, 以达到目标
* 若当探索到某一步时, 发现原先选择并不优或达不到目标, 就退回一步重新选择, 这样的技术被称为回溯
  * 满足回溯条件的某个状态的点称为回溯点