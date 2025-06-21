#include "stm32f10x.h"                  // Device header
#include "encoder.h"

static EncoderCfg_t g_encoderCfg = {
	/* Encoder */
	GPIOB,
	RCC_APB2Periph_GPIOB,
	GPIO_Pin_0,
	GPIO_Pin_1,

	/* AFIO */
	GPIO_PortSourceGPIOC,
	RCC_APB2Periph_AFIO,
	GPIO_PinSource0,
	GPIO_PinSource1,

	/* EXIT */
	EXTI_Line0,
	EXTI_Line1,

	/* NVIC */
	NVIC_PriorityGroup_2,
	EXTI15_10_IRQn,
	EXTI15_10_IRQn
};


int16_t Encoder_Count;


void Encoder_Init(void){
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(g_encoderCfg.encoderClock, ENABLE);		//开启GPIOB的时钟
	RCC_APB2PeriphClockCmd(g_encoderCfg.afioClock, ENABLE);		//开启AFIO的时钟，外部中断必须开启AFIO的时钟
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = g_encoderCfg.encoderPin1|g_encoderCfg.encoderPin2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(g_encoderCfg.encoderPort, &GPIO_InitStructure);						// 将PB14引脚初始化为上拉输入
	
	/*AFIO选择中断引脚 PB14 */
	GPIO_EXTILineConfig(g_encoderCfg.afioPortSource, g_encoderCfg.afioPinSource1);// 选择PB0为外部中断引脚
	GPIO_EXTILineConfig(g_encoderCfg.afioPortSource, g_encoderCfg.afioPinSource2);// 选择PB1为外部中断引脚
	
	/*EXTI初始化*/
	EXTI_InitTypeDef EXTI_InitStructure;						//定义结构体变量
	EXTI_InitStructure.EXTI_Line = g_encoderCfg.exitLine1|g_encoderCfg.exitLine2;		//选择配置外部中断的0/1号线
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		//指定外部中断线为下降沿触发
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			//指定外部中断线为中断模式
	EXTI_Init(&EXTI_InitStructure);								//将结构体变量交给EXTI_Init，配置EXTI外设
	
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;					//指定外部中断线使能

	
	/*NVIC中断分组*/
	NVIC_PriorityGroupConfig(g_encoderCfg.nvicPriorityGroup);				//配置NVIC为分组2
																//即抢占优先级范围：0~3，响应优先级范围：0~3
																//此分组配置在整个工程中仅需调用一次
																//若有多个中断，可以把此代码放在main函数内，while循环之前
																//若调用多次配置分组的代码，则后执行的配置会覆盖先执行的配置
	
	/*NVIC配置*/
	NVIC_InitTypeDef NVIC_InitStructure;						//定义结构体变量
	NVIC_InitStructure.NVIC_IRQChannel = g_encoderCfg.nvicIRQChannel1;		//选择配置NVIC的EXTI0线
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//指定NVIC线路使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//指定NVIC线路的抢占优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//指定NVIC线路的响应优先级为1
	NVIC_Init(&NVIC_InitStructure);		
	
	NVIC_InitStructure.NVIC_IRQChannel = g_encoderCfg.nvicIRQChannel2;		//选择配置NVIC的EXTI1线
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//指定NVIC线路使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	//指定NVIC线路的抢占优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//指定NVIC线路的响应优先级为1
	NVIC_Init(&NVIC_InitStructure);	
}


int16_t Encoder_Get(void)
{
	int16_t Temp=Encoder_Count;
	Encoder_Count=0;
	return Temp;
}


void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(g_encoderCfg.exitLine1)==SET)
	{
		if(GPIO_ReadInputDataBit(g_encoderCfg.encoderPort,g_encoderCfg.encoderPin1)==0)
		{
			Encoder_Count--;
		}
		EXTI_ClearITPendingBit(g_encoderCfg.exitLine1);
	}
	
		
}
void EXTI1_IRQHandler()
{
	if(EXTI_GetITStatus(g_encoderCfg.exitLine2)==SET)
	{

		if(GPIO_ReadInputDataBit(g_encoderCfg.encoderPort,g_encoderCfg.encoderPin2)==0)
		{
			Encoder_Count++;
		}
		EXTI_ClearITPendingBit(g_encoderCfg.exitLine2);
	}
}
