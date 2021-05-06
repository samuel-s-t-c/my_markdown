# SPI, Serial Peripheral Interface

SPI, 是一种高速的, 全双工的, 同步的通信总线

* 主要应用在EEPROM, FLASH, 实时时钟, AD转换器, 还有数字信号处理器和数字信号解码器之间

特点

* 主从(master-slave)控制: master给slave提供clock; master通过slave select信号控制多个slave
* 同步: 时钟信号控制如何对数据信号采样
* 数据交换: 每个clock周期, spi设备都会发送并接收1 bit的数据

接口定义

* MOSI: master用此接口输出数据; slave用此接口接收数据
* MISO: slave用此接口输出数据; master用此接口接收数据
* $\overline{SS}$: master用此接口输出选择信号(select signal);

