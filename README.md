# self_os

在cortex m3和m4核上写的小的操作系统,适配了stm32和s32k144.

目前实现的功能如下:

1.基于优先级的抢占式调度和同优先级的时间片轮训调度

2.信号量

3.队列

4.简单的内存管理

后续需要将上述的几个基本功能都进行改进.

主要如下:

1.优化调度的逻辑和实时性

2.锁

3.伙伴算法的内存管理

4.tcp/ip

5.文件系统

目前想到的是这些.

现在selfos正在一个测试设备上调试运行.
