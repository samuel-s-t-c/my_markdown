# 1. Linux账号管理与ACL权限设置

## 1.1 Linux的账号与用户组

文件`/etc/passwd`中记录

* 账号名称
* 密码: 早期unix的密码记录在该文件中; 现以将密码改放在`/etc/shadow`
* UID
* GID

* 用户信息说明栏
* 家目录
* shell

文件`/etc/shadow`中记录

* 账号名称
* 经过加密的密码
* 最近修改密码的日期
* 可修改密码的间隔 (以日为单位): 在修改密码后, 经过多少天才能够再次修改密码
* 需要修改密码的间隔 (以日为单位): 每经过多少天, 需要重新修改密码
* 需要修改密码前的警告天数
* 密码过期后的账号宽限天数
* 账号失效日期
* 保留的字段

UID的说明

| ID范围 | 说明                                            |
| ------ | ----------------------------------------------- |
| 0      | 表示该账号是系统管理员                          |
| 1~999  | 保留给系统使用的ID, 即系统账号                  |
| 1000~? | 一般用户的ID; Linux内核3.10可最大支持2^31-1的ID |

* 在Linux中, 有些账号是为了系统的正常运行, 这些账号被称为系统账号; 
  * 作用: 用来执行系统所需复位的权限设置

### 用户组相关

> 命令: `usermod`, `gpasswd`, `groups`, `newgrp`, `chgrp`
>
> 配置文件: `/etc/group`, `/etc/gshadow`

初始用户组initial group

* 每个用户的GID就是初始用户组
* 当用户登录系统时, 该用户立即拥有对应的初始用户组的相关权限 

有效用户组effective group

* 是指当前用户所在的用户组
* 命令`groups`: 显示的第一个名字就是当前用户的有效用户组
* 命令`newgrp`: 将有效用户组设为给定的用户组
  * 给定的用户组必须是支持该用户的

`/etc/group`文件记录

* 组名
* 用户组密码: 早期unix系统的做法; 现已将密码放在`/etc/gshadow`
* GID
* 此用户组支持的账号名称

`/etc/gshadow`文件中记录

* 组名
* 密码: 若为空或`!`, 则表示该用户组没有用户组管理员
* 用户组管理员的账号
* 此用户组支持的账号名称

## 1.2 账号管理

增删用户

* 命令: `useradd`, `passwd`, `chage`, `usermod`, `userdel`

* 配置文件: `/etc/default/useradd`, `/etc/login.defs`, `/etc/skel/*`

用户功能: 作为一般用户, 查看/修改自身的数据

* 命令: `id`, `finger`, `chfn`, `chsh`

增删用户组

* 命令: `groupadd`, `groupmod`, `groupdel`, `gpasswd`, 

外部身份认证系统

* 在windows下有名的身份验证系统, 称为活动目录(active directory)
* 在Linux下的身份验证系统有LDAP, NIS

## 1.3 详细权限规划与ACL

ACL(Access Control List, 访问控制表)

* 作用: 提供详细权限设置
  * ACL表中的每一项都确定某一用户或用户组对相应文件的`rwx`访问权限
* 如何启动ACL功能: 
  * 原本, ACL是UNIX-like操作系统的额外支持选项; 由于需求, 目前在大多数的Linux文件系统中, 默认将ACL加入挂载参数中
  * 查看是否支持ACL: `dmesg | grep -i acl`
* 设置与查看ACL: `getfacl`, `setfacl`

ACL类型

* 访问ACL: 每个文件都有一个关联的访问ACL, 用于控制对文件的访问权
* 默认ACL: 目录特有的ACL; 当在目录创建文件时, 使用默认ACL来确定文件的初始文件ACL

ACL entry

* 每条ACL entry指明某一用户或用户组对相应文件的`rwx`权限
* 每条ACL entry包含一个entry tag类型, 一个可选的entry tag限定符和一组权限

## 1.4 用户身份切换

命令: `su`, `sudo`, `visudo`

配置文件: `/etc/sudoers`

## 1.5 特殊shell与PAM模块

特殊的shell: `/sbin/nologin`

* 当用户的初始shell是`/sbin/nologin`时, 该用户是无法登录系统的, 但是可以使用其他系统资源
* 系统账号的shell通常是`/sbin/nologin`

### PAM

PAM(Pluggable Authentication Modules, 插入式验证模块)

* PAM是一组API, 提供了一系列的验证机制; 只要用户将验证阶段的需求告知PAM, PAM就能够返回用户验证的结果
* PAM有多种模块, 以提供不同的验证机制

以`passwd`命令调用PAM为例子; 该程序调用PAM的流程是

1. 用户执行`/usr/bin/passwd`, 并输入密码
2. `passwd`命令调用PAM进行验证
3. PAM会到`/etc/pam.d`目录中寻找与程序同名的配置文件, 即`passwd`
4. 根据`/etc/pam.d/passwd`文件内的设置, 引用相关的PAM模块逐步进行验证分析
5. 将验证结果(成功, 失败以及其他信息)返回给`passwd`命令
6. `passwd`命令会根据PAM返回的结果决定下一个操作

关于PAM的文件

* `/etc/pam.d/*`: 每个程序的PAM配置文件
* `/lib64/security/*`: PAM模块文件
* `/etc/security/*`: 其他PAM环境的配置文件
* `/usr/share/doc/pam/*`: 详细的PAM说明文件

#### PAM模块设置语法

> 如何编写`/etc/pam.d`中的配置文件

注释: 以`#`开头

每一行都是一个独立的验证流程, 包含三个字段

* 验证类型(type)
  * `auth`: 主要用来检验用户的身份; 通常需要密码来验证
  * `account`: 主要用来检验用户是否具有正确的权限
  * `session`: 主要用于记录用户登录与注销时的信息
    * 该类别管理的是, 在本次登录(或使用这个命令)期间, PAM所给予的环境设置
  * `password`: 主要用于提供验证的修订任务, 如修改密码
* 控制标识(control flag)
  * `required`:
    * 若验证成功, 返回带有success的标志, 并继续后续的验证流程
    * 若验证失败, 返回带有failure的标志, 并继续后续的验证流程
  * `requisite`
    * 若验证成功, 返回带有success的标志, 并继续后续的验证流程
    * 若验证失败, 返回带有failure的标志, 并终止后续的验证流程
  * `sufficient`
    * 若验证成功, 返回带有success的标志, 并终止后续的验证流程
    * 若验证失败, 返回带有failure的标志, 并继续后续的验证流程
  * `optional`
    * 用于显示信息
* PAM模块及其参数

## 1.6 Linux的用户信息传递

查询用户: `w`, `who`, `last`, `lastlog`

用户间的交流: `write`, `mesg`, `wall`

用户邮箱: `mail`

# 2. 磁盘配额与高级文件系统管理

# 3. 计划任务crontab

# 4. 进程管理与SELinux

​	