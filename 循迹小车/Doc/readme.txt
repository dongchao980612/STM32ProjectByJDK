一、使用的传感器
	1. 循迹传感器
	从代码中read()函数可知，使用了 8 路红外对管传感器（OUT1-OUT8）
	硬件连接：
	OUT1: PB7, OUT2: PB6, OUT3: PB5, OUT4: PB4
	OUT5: PB3, OUT6: PA15, OUT7: PA12, OUT8: PB9
	工作原理：通过读取 8 路传感器的高低电平（组合成 8 位数据）判断黑线位置
	
	2. 姿态传感器 JY61P
	通过串口通信（USART2 或 USART3）获取姿态数据
	解析的数据包括：
	Roll（横滚角）
	Pitch（俯仰角）
	Yaw（偏航角）- 主要用于转向控制
	数据帧格式：以 0x55 和 0x53 为帧头的 11 字节数据包，包含校验和验证
	
	3. 电机编码器（代码中部分实现）
	通过 TIM2 和 TIM3 定时器计数
	用于测量电机转速（V_A 和 V_B 变量）
	预留了速度 PID 控制函数（PID_A 和 PID_B）
	
二、驱动模块
	1. 电机驱动
	采用 PWM 控制两个直流电机
	硬件连接：
	电机 1 方向控制：PB12、PB13
	电机 2 方向控制：PB14、PB15
	PWM 输出：TIM1_CH1（PA8）、TIM1_CH2（PA9）
	驱动函数：
	Set_PWM()：设置电机方向和占空比
	PWM_Limiting()：限制 PWM 输出范围（±7000）
	
	2. 舵机驱动（推测用于机械臂或转向辅助）
	通过 TIM2_CH1（PA0）和 TIM2_CH2（PA1）输出 PWM
	控制函数：__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, value)