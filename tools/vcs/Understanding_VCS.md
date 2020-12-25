[原文](http://www.catb.org/~esr/writings/version-control/version-control.html)

# 1. 为什么使用版本控制

版本控制系统(version control system, VCS), 是一种管理程序代码集的工具; 它有三种重要的功能

* 可逆(reversibility): 当你所做的某些修改是不适当时, 它能够回到已保存的良好状态
* 并发(concurrency): 当多人修改同一代码集时, 能够检测并解决有冲突的部分
* 注解(annotation): 每次修改都会记录其作者, 并且可以记录关于本次修改的解释说明

VCS中, 在repository中存储了文件的master副本, 和相关的修改历史(change history)

* 你不会直接对repository进行操作; 而是先从repository内容中生成工作副本(working copy), 然后对工作副本进行编辑

VCS的三种基本操作

* check out: 从repository中取出工作副本
* check in / commit: 将工作副本中的修改放入repository的对应master副本中
* view history: 浏览文件的修改历史

# 2. 版本控制系统的分类

三种分类方式

* 集中式或分布式(centralized or decentralized)
* locking, merge-before-commit or commit-before-merge
* 可以执行文件操作, 或文件集操作

# 3. 术语说明

# 4. 简要历史

# 5. 比较