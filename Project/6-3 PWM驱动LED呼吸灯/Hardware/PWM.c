#include "stm32f10x.h"                  // Device header
#include "pwm.h"

static pwmCfg_t g_pwmCfg = {
		/* GPIO */
	GPIOA,
	RCC_APB2Periph_GPIOA,
	GPIO_Pin_0,
	
	/* Timer2 */
	TIM2,
	RCC_APB1Periph_TIM2,
	
	TIM_CKD_DIV1,
	TIM_CounterMode_Up,
	
	100-1,
	720-1,
	0,
	0
};


void PWM_Init(void)
{
	RCC_APB2PeriphClockCmd(g_pwmCfg.pwmClock, ENABLE);
	RCC_APB1PeriphClockCmd(g_pwmCfg.timerClock, ENABLE);
	
	
//  引脚复用
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//  引脚重映射
//	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);   // 部分重映射
	//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  //解除调试端口复用
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
	GPIO_InitStructure.GPIO_Pin = g_pwmCfg.pwmPin;		//或者 GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(g_pwmCfg.pwmPort, &GPIO_InitStructure);
	
	// 定时器初始化
	TIM_InternalClockConfig(g_pwmCfg.timer);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = g_pwmCfg.timerClockDivision;
	TIM_TimeBaseInitStructure.TIM_CounterMode = g_pwmCfg.timerCounterMode;
	TIM_TimeBaseInitStructure.TIM_Period = g_pwmCfg.timerPeriod;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = g_pwmCfg.timerPrescaler;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = g_pwmCfg.timerRepetitionCounter;
	TIM_TimeBaseInit(g_pwmCfg.timer, &TIM_TimeBaseInitStructure);
	
	// 输出比较单元
	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCStructInit(&TIM_OCInitStructure); //初始化其他未配置的结构体成员
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;  //输出比较模式
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//输出比较极性
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = g_pwmCfg.timerPulse;		// CCR
	
	TIM_OC1Init(g_pwmCfg.timer, &TIM_OCInitStructure);
	
	TIM_Cmd(g_pwmCfg.timer, ENABLE);
}

void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(g_pwmCfg.timer, Compare);
}
