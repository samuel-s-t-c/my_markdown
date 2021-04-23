# Part A: Architecture

## 1 概述

蓝牙无线技术系统的两种形式

* Basic RateJ(BR)系统
  * 设备发现, 连接建立, 连接机制

  * 可选: Enhanced Data Rate(EDR) , Alternate Media Access Control(MAC) and Physical(PHY) layer extension
  * 数据速率: 721.2kb/s for br, 2.1Mb/s for edr, 54Mb/s with the 802.11 AMP

* Low Energy(LE)系统
  * 设备发现, 连接建立, 连接机制

  * 相对BR/EDR而言, 降低电流消耗, 复杂度和费用
  * 针对用例和应用, LE可以提供更低数据速率和duty cycle
  * 可选的2Mb/s物理层数据速率
  * 等时数据传输

蓝牙核心系统由一个host和一个或多个controller组成

* host: 是一个逻辑实体, 由在non-core profile之下或在HCI之上的所有layer组成
* controller: 是一个逻辑实体, 由在HCI之下的所以layer组成
  * primary controller: 蓝牙核心只能有一个primary controller
  * secondary controller: 蓝牙核心可以有零个或多个secondary controller
* HCI: Host Controller interface

### 1.1 BR/EDR操作概述

蓝牙BR/EDR的通讯基本形式: piconet(微微网)

* 一个设备, 作为master, 提供synchronization reference和frequency hopping pattern(跳频模式)
  * 一种跳频模式: 将ISM频带以1MHz分割成79个频道(channel), 然后伪随机排列这些频道
  * 自适应跳频技术: 改变跳频模式, 从而避开正在被使用的频率
* 其他设备, 作为slave, 会与master同步

#### Hierarchy

自下而上: 物理频道, 物理链路, 逻辑传输, 逻辑链路, L2CAP频道

#### 物理层

slot: 物理频道被进一步分成slot

* 在蓝牙设备之间传输的数据, 被封装成数据包, 然后放在这些slot中传送的
* 当条件允许时, 连续slot会被分配到同一数据包

蓝牙通过Time-Division Duplex(TDD, 时分双工)实现全双工传输

#### 1.2 LE操作概述

* LE使用两种多址(multiple access)方案:
  * Frequency division multiple access(FDMA)
    * 将ISM频带以2MHz分割成40个物理频道; 其中3个作为主要广播频道; 其他37个作为通用用途频道
  * Time division multiple access(TDMA)
    * 在传送数据包时, 某个设备在预定事件传送数据包;  在预定的时间间隔后, 相应设备传送响应数据包
* 物理频道在时间上被分割为多个事件; 数据在事件中传送.
  * 事件的种类有: 广播, 扩展广播, 周期广播, 连接, 等时事件

广播

* 广播者(advertiser), 扫描者(scanner), 发起者(initiator)
  * 广播者: 传送广播包的设备
  * 扫描者: 接受广播包, 但不会尝试连接广播者的设备
  * 发起者: 在接受到可连接广播包后发起连接请求的设备

* 主要广播频道只能有广播事件

* 广播事件

  * 在广播事件开始时, 广播者发送一个广播包(根据不同的广播事件类型而不同); 根据广播包的类型, 扫描者会在同一物理频道向广播者发送请求; 而后, 广播者可以在同一物理频道作出响应
  * 在同一广播事件中, 广播者发送下一个广播包时会选择不同的广播物理频道
  * 广播者可以在广播事件中任一时间停止广播
  * 在某种广播事件开始时, 广播者总在特定的物理频道发送第一个广播包

  * ![advertising_event_f_1_3](vol_01.assets/advertising_event_f_1_3.png)

* 连接事件
  * 用于在master和slave之间发送数据包
  * master可以发送连接事件, 也可以终止连接事件
  * 在每个连接事件的开始时都会channel hop(跳频道)
  * 在连接事件中, master和slave在同一物理频道中交替地发送数据包
  * ![connection_event_f_1_4](vol_01.assets/connection_event_f_1_4.png)
* 建立ACL连接的过程
  1. 广播者在可连接广播事件中发送可连接广播包
  2. 在收到可连接广播包后, 发起者在相同物理频道中发起连接请求
  3. 广播者收到并同意该连接请求; 此时, 广播事件结束, 连接事件开始
  4. 说明: 一旦建立连接, 发起者成为master, 广播者成为slave