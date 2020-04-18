使用Rust语言编写基于RISCV架构的简单的操作系统

[toc]

# RUST

对于一个普通的链接`std RUST`库的rust程序, 它的执行路径是

1. C运行时库`crt0`: 进入点默认为`_start`函数
   * 用于设置C应用的执行环境, 包括创建一个栈, 寄存器中放入合适的参数等等
2. RUST运行时库: 进入点是`start`语义项, 默认为`lang_start`函数
3. 用户编写的程序: 进入点是`main`函数

`panic_handler`函数

* 用于处理`panic`

`eh_personality`语义项

* 用于实现栈展开(**stack unwinding**)

## rust编译器

## rust链接器

基于目标三元组(**target triple**)来选择不同的链接策略

### 链接脚本

## 内联汇编

# Makefile

# 汇编

汇编指示符

# ELF

