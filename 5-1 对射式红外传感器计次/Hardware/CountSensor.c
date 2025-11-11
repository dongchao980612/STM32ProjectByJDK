#include "stm32f10x.h"                  // Device header
#include "CountSensor.h"


static CountSensorCfg_t g_countsensorCfg = {
	/* CountSensor */
	GPIOB,
	RCC_APB2Periph_GPIOB,
	GPIO_Pin_14,

	/* AFIO */
	GPIO_PortSourceGPIOC,
	RCC_APB2Periph_AFIO,
	GPIO_PinSource10,

	/* EXIT */
	EXTI_Line14,

	/* NVIC */
	NVIC_PriorityGroup_2,
	EXTI15_10_IRQn
};



uint16_t CountSensor_Count = 0;

void CountSensor_Init(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(g_countsensorCfg.countsensorClock, ENABLE);		//开启GPIOB的时钟
	RCC_APB2PeriphClockCmd(g_countsensorCfg.afioClock, ENABLE);		//开启AFIO的时钟，外部中断必须开启AFIO的时钟
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = g_countsensorCfg.countsensorPin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(g_countsensorCfg.countsensorPort, &GPIO_InitStructure);						//将PB14引脚初始化为上拉输入
	
	/*AFIO选择中断引脚 PB14 */
	GPIO_EXTILineConfig(g_countsensorCfg.afioPortSource, g_countsensorCfg.afioPinSource);//将外部中断的14号线映射到GPIOB，即选择PB14为外部中断引脚
	
	/*EXTI初始化*/
	EXTI_InitTypeDef EXTI_InitStructure;						//定义结构体变量
	EXTI_InitStructure.EXTI_Line = g_countsensorCfg.exitLine;					//选择配置外部中断的14号线
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//指定外部中断线为下降沿触发
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//指定外部中断线为中断模式
	EXTI_Init(&EXTI_InitStructure);								//将结构体变量交给EXTI_Init，配置EXTI外设
	
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;					//指定外部中断线使能

	
	/*NVIC中断分组*/
	NVIC_PriorityGroupConfig(g_countsensorCfg.nvicPriorityGroup);				//配置NVIC为分组2
																//即抢占优先级范围：0~3，响应优先级范围：0~3
																//此分组配置在整个工程中仅需调用一次
																//若有多个中断，可以把此代码放在main函数内，while循环之前
																//若调用多次配置分组的代码，则后执行的配置会覆盖先执行的配置
	
	/*NVIC配置*/
	NVIC_InitTypeDef NVIC_InitStructure;						//定义结构体变量
	NVIC_InitStructure.NVIC_IRQChannel = g_countsensorCfg.nvicIRQChannel;		//选择配置NVIC的EXTI15_10线
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//指定NVIC线路使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//指定NVIC线路的抢占优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//指定NVIC线路的响应优先级为1
	NVIC_Init(&NVIC_InitStructure);								//将结构体变量交给NVIC_Init，配置NVIC外设
}


uint16_t CountSensor_Get(void)
{
	return CountSensor_Count;
}


void EXTI15_10_IRQHandler(void)
{
	if (EXTI_GetITStatus(g_countsensorCfg.exitLine) == SET)		//判断是否是外部中断14号线触发的中断
	{
		/*如果出现数据乱跳的现象，可再次判断引脚电平，以避免抖动*/
		if (GPIO_ReadInputDataBit(g_countsensorCfg.countsensorPort, g_countsensorCfg.countsensorPin) == 0)
		{
			CountSensor_Count++;					//计数值自增一次
		}

		EXTI_ClearITPendingBit(g_countsensorCfg.exitLine);		//清除外部中断14号线的中断标志位
													//中断标志位必须清除
													//否则中断将连续不断地触发，导致主程序卡死
	}
	
}
