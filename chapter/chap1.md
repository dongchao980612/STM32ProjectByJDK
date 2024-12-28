
# 第一章 概述

## 1.1 课程简介

## 1.2 STM32简介

### 1.2.1 STM32简介

- STM32 是ST公司推出的ARM Cortex-M3内核开发的32位的微控制器

- STM32具有丰富的外设接口，如GPIO、ADC、DAC、PWM、I2C、SPI、USART、USB等，可满足不同应用的需求

- STM32常应用于嵌入式领域，如智能车、无人机、机器人、无线通信、物联网、工业控制、娱乐电子产品等

- STM32功能强大、性能优异、片上资源丰富、功耗低，是一款经典的嵌入式微控制器
  

![STM32分类](https://www.st.com.cn/content/dam/category-pages/stm32-32-bit-arm-cortex-mcus/arm-cortex-mcu-portfolio-1.jpg)

### ARM

ARM即指ARM公司，也指ARM内核处理器

![ARM分类](https://i-blog.csdnimg.cn/blog_migrate/37069480cdfb018de918886e1500607f.jpeg)

- Cortex-M系列：该系列处理器包括Cortex-M0、Cortex-M0+、Cortex-M1、Cortex-M3、Cortex-M4共5个子系列。该系列主要针对成本和功耗敏感的应用，如智能测量、人机接口设备、汽车和工业控制系统、家用电器、消费性产品和医疗器械等。

- Cortex-R系列：该系列处理器包括Cortex-R4、Cortex-R5、Cortex-R7共3个子系列。面向如汽车制动系统、动力传动解决方案、大容量存储控制器等深层嵌入式实时应用。

- Cortex-A系列：该系列处理器包括Cortex-A5、Cortex-A7、Cortex-A8、Cortex-A9、Cortex-A12和Cortex-A15共6个子系列，用于具有高计算要求、运行丰富操作系统及提供交互媒体和图形体验的应用领域，如智能手机、平板电脑、汽车娱乐系统、数字电视等。

### 1.2.2 STM32F103C8T6

- 系列: STM32F103系列
- 内核: Cortex-M3
- 主频: 72MHz
- ROM: 64KB
- RAM: 20KB
- 工作电压: 2.0V-3.6V
- 接口: 10x 10-pin 串行连接器
- 调试接口: JTAG/SWD
- 片上资源: 168KB Flash, 64KB SRAM
- 封装: LQFP64
- 外设:
  - NVIC:嵌套向量中断控制器
  - SysTick:系统计时器
  - RCC:时钟和RESET控制器
  - GPIO:通用IO口
  - AFIO:复用IO口
  - EXTI:外部中断
  - TIM:定时器
  - ADC:模拟转换器
  - DMA:直接内存访问
  - USART:串行通讯
  - I2C:集成电路总线
  - SPI:串行外设接口
  - CAN:控制器局域网
  - USB:全速设备接口
  - RTC:实时时钟
  - CRC:循环冗余校验
  - PWR:电源管理
  - BKP:备份寄存器
  - IWDG:独立看门狗
  - WWDG:窗口看门狗
  - DAC:数模转换器
  - SDIO:SD卡接口
  - fSMC:可变静态存储控制器
  - USB OTG:usb主机接口

### 1.2.3 命名规则

- STM32：这是产品系列名称，代表的是STMicroelectronics制造的一系列32位微控制器。

- F：指代产品线。在这里，“F”表示的是STM32系列中的高性能产品线，通常基于ARM Cortex-M3或M4核心。

- 103：进一步细分了产品线。“103”特别指的是基于ARM Cortex-M3处理器核心的具体子系列。

- C：这表示内存容量级别。不同的字母代表不同大小的Flash存储器配置。“C”一般表示较小容量版本之一；对于STM32F103系列来说，"C"可能对应于64KB Flash。

- 8：直接指示了Flash存储器大小。在STM32F103C系列中，“8”意味着拥有64KB的Flash存储空间。

- T：封装类型。这里“T”通常指的是LQFP-48引脚封装。

- 6：温度范围。数字“6”在此处指定工作温度范围为-40°C到+85°C。

### 1.2.4 启动配置

| BOOT0 | BOOT1 |     MODE     |       说明       |
| :---: | :---: | :----------: | :--------------: |
|   X   |   0   | 主闪存存储器 | 主闪存存储器启动 |
|   X   |   0   |  系统存储器  |  系统存储器启动  |
|   X   |   0   |   内置SRAM   |   内置SRAM启动   |
