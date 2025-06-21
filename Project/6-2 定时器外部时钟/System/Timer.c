#include "stm32f10x.h"                  // Device header
#include "timer.h"


static timer2Cfg_t g_timer2Cfg = {
		/* GPIO */
	GPIOA,
	RCC_APB2Periph_GPIOA,
	GPIO_Pin_0,
	
	/* Timer2 */
	TIM2,
	RCC_APB1Periph_TIM2,
	
	TIM_CKD_DIV1,
	TIM_CounterMode_Up,
	
	10-1,
	1-1,
	0,


	/* NVIC */
	NVIC_PriorityGroup_2,
	TIM2_IRQn,
};

void Timer2_Init(void)
{
	RCC_APB1PeriphClockCmd(g_timer2Cfg.timerClock,ENABLE); 
	RCC_APB2PeriphClockCmd(g_timer2Cfg.outClock, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = g_timer2Cfg.outPin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(g_timer2Cfg.outPort, &GPIO_InitStructure);
	
	
	TIM_ETRClockMode2Config(g_timer2Cfg.timer, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0x0F);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = g_timer2Cfg.timerClockDivision;
	TIM_TimeBaseInitStructure.TIM_CounterMode = g_timer2Cfg.timerCounterMode;
	TIM_TimeBaseInitStructure.TIM_Period = g_timer2Cfg.timerPeriod;
	TIM_TimeBaseInitStructure.TIM_Prescaler = g_timer2Cfg.timerPrescaler;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = g_timer2Cfg.timerRepetitionCounter;
	TIM_TimeBaseInit(g_timer2Cfg.timer,&TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(g_timer2Cfg.timer,TIM_FLAG_Update); 					// 清空更新中断标志位
	TIM_ITConfig(g_timer2Cfg.timer,TIM_IT_Update,ENABLE);
	
	
	/*NVIC中断分组*/
	NVIC_PriorityGroupConfig(g_timer2Cfg.nvicIRQChannel);				//配置NVIC为分组2
																															//即抢占优先级范围：0~3，响应优先级范围：0~3
																															//此分组配置在整个工程中仅需调用一次
																															//若有多个中断，可以把此代码放在main函数内，while循环之前
																															//若调用多次配置分组的代码，则后执行的配置会覆盖先执行的配置
	
	/*NVIC配置*/
	NVIC_InitTypeDef NVIC_InitStructure;																		//定义结构体变量
	NVIC_InitStructure.NVIC_IRQChannel = g_timer2Cfg.nvicIRQChannel;				//选择配置NVIC的TIM2_IRQn线
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;													//指定NVIC线路使能
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;								//指定NVIC线路的抢占优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;											//指定NVIC线路的响应优先级为1
	NVIC_Init(&NVIC_InitStructure);		
	
	
	TIM_Cmd(g_timer2Cfg.timer,ENABLE);

}

uint16_t Timer2_GetCounter(void)
{
	return TIM_GetCounter(TIM2);
}


// 推荐写在主函数
// 中断函数模板
//void TIM2_IRQHandler(void)
//{
//	if(TIM_GetITStatus(g_timer2Cfg.timer2,TIM_IT_Update)==SET)
//	{
//		TIM_ClearITPendingBit(g_timer2Cfg.timer2,TIM_IT_Update);
//	}	
//}
